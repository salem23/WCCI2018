/***************************************************************************
 
    file                 : SM_Driver.cpp
    copyright            : (C) 2007 Daniele Loiacono
 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "SM_Driver.h"
#include <fstream>
#include <sstream>
#include <windows.h>
class CTriangle 
{
private: 
	double dMiddle,dMiddle1;
		double dLeft, dRight;
	char   cType;
	string sName;
	
	
public:
       
   	CTriangle(){};
	 void setInterval(double l, double r){
		dLeft = l; 
		dRight = r;
	}
	// void setMiddle(double dL = 0, double dR = 0) = 0;
	 void setType(char c){cType = c;}
	 void setName(const string s){ 
	  sName = s;
	}
	bool isDotInInterval(double t){
		if((t >= dLeft) && (t <= dRight)) 
			return true; 
		return false;
	}
	char getType( void)const{ 
		return cType;	
	}
    string  getName() const{
		return sName;
	}


	void setMiddle1(double dL){
		dMiddle1 = dL;
	}
	double getMiddle1(){
		return dMiddle1;
	}
    
	void setMiddle(double dL){
		dMiddle = dL;
	}
	double getMiddle(){
		return dMiddle;
	}
	double getRight(){
		return dRight;
	}	
	double getLeft(){
		return dLeft;
	}		
	
	
double getValue(double t)
{
           double r;
//if (getType()=='t')
{
r=fmax(fmin(fmin(((t-dLeft)/(dMiddle-dLeft)),1),((dRight-t)/(dRight-dMiddle1))),0);
};
//if (getType()=='h')
{
                   
//r=fmax(fmin(1,((dRight-t)/(dRight-dMiddle))),0);                      
 };
 // if (getType()=='b')
           {
                     
 //r=fmax(fmin(((t-dLeft)/(dMiddle-dLeft)),1),0);  
           }  
           
           return r; 
}          
};


struct Member
{
	CTriangle* fuzzySet;
	int numberOfValue;
};
struct Rule
{
	int leftSide[3][2];
	int rightSide[2];
	int fuzzyOperator;
};
struct DataInput
{
	double input[3];
};
struct Member * members;
struct Rule * rules;
struct DataInput  dataInput;
double  output;



//double ruleEvaluation(struct Rule, struct DataInput );
//double defuzzyfication(struct Rule * , int , struct DataInput);




/* Gear Changing Constants*/
const int SM_Driver::gearUp[6]=
    {
        5000,6000,6000,6500,7000,0
    };
const int SM_Driver::gearDown[6]=
    {
        0,2500,3000,3000,3500,3500
    };

/* Stuck constants*/
const int SM_Driver::stuckTime = 25;
const float SM_Driver::stuckAngle = .523598775; //PI/6

/* Accel and Brake Constants*/
const float SM_Driver::maxSpeedDist=100;
const float SM_Driver::maxSpeed=320;
const float SM_Driver::sin5 = 0.08716;
const float SM_Driver::cos5 = 0.99619;

/* Steering constants*/
const float SM_Driver::steerLock=0.366519 ;
const float SM_Driver::steerSensitivityOffset=80.0;
const float SM_Driver::wheelSensitivityCoeff=1;

/* ABS Filter Constants */
const float SM_Driver::wheelRadius[4]={0.3306,0.3306,0.3276,0.3276};
const float SM_Driver::absSlip=2.0;
const float SM_Driver::absRange=3.0;
const float SM_Driver::absMinSpeed=3.0;

/* Clutch constants */
const float SM_Driver::clutchMax=0.5;
const float SM_Driver::clutchDelta=0.05;
const float SM_Driver::clutchRange=0.82;
const float SM_Driver::clutchDeltaTime=0.02;
const float SM_Driver::clutchDeltaRaced=10;
const float SM_Driver::clutchDec=0.01;
const float SM_Driver::clutchMaxModifier=1.3;
const float SM_Driver::clutchMaxTime=1.5;
float targetSpeed;



////////******************* fuzzy


double SM_Driver::ruleEvaluation(struct Rule rule, struct DataInput dataInput)
{
       
	int memberIndex, fuzzySetIndex;
	double leftValue1, leftValue2, leftValue3,res;
	
	memberIndex = rule.leftSide[0][0];
	fuzzySetIndex = rule.leftSide[0][1];
	leftValue1 = members[memberIndex].fuzzySet[fuzzySetIndex].getValue(dataInput.input[0]);
	res=leftValue1;
	memberIndex = rule.leftSide[1][0];
	fuzzySetIndex = rule.leftSide[1][1];
	if (memberIndex>-1)
	{
	leftValue2 = members[memberIndex].fuzzySet[fuzzySetIndex].getValue(dataInput.input[1]);
		if(rule.fuzzyOperator == 0)
		res= fmin(leftValue1, leftValue2);
	else res= fmax(leftValue1, leftValue2);
    }
	memberIndex = rule.leftSide[2][0];
	fuzzySetIndex = rule.leftSide[2][1];
	if (memberIndex>-1)
	{
	leftValue3 = members[memberIndex].fuzzySet[fuzzySetIndex].getValue(dataInput.input[2]);
		if(rule.fuzzyOperator == 0)
		res= fmin(res, leftValue3);
	else res= fmax(res, leftValue3);
         }
return res;
}
double SM_Driver::defuzzyfication(struct Rule * rule, int ruleNumber, struct DataInput dataInput)
{
	double dtemp1 = 0;
	double dtemp2 = 0;
	for(int i=0; i< ruleNumber; i++)
	{
		double temp = ruleEvaluation(rule[i], dataInput);
		
	//	double output = members[rules[i].rightSide[0]].fuzzySet[rules[i].rightSide[1]].getMiddle();
	double output1 = rules[i].rightSide[0];
   /// cout <<i << "eval : " << temp << "right : " << output1 <<"\n";
		dtemp1 += temp;
		dtemp2 += temp * output1;
	}
	if(dtemp1 == 0)
		return 0;
	return dtemp2/dtemp1;
}

/////////////////////*****************
int
SM_Driver::getGear(CarState &cs)
{

    int gear = cs.getGear();
    int rpm  = cs.getRpm();

    // if gear is 0 (N) or -1 (R) just return 1 
    if (gear<1)
        return 1;
    // check if the RPM value of car is greater than the one suggested 
    // to shift up the gear from the current one     
    if (gear <6 && rpm >= gearUp[gear-1])
        return gear + 1;
    else
    	// check if the RPM value of car is lower than the one suggested 
    	// to shift down the gear from the current one
        if (gear > 1 && rpm <= gearDown[gear-1])
            return gear - 1;
        else // otherwhise keep current gear
            return gear;
}

float
SM_Driver::getSteer(CarState &cs)
{
                                
// set angles as {-90,-75,-60,-45,-30,       20,   15,    10,    5, 0,    5,   10,      15,    20,   30,   45,60,75,90}
//                 1    1   1   1  .80      .60   .40    .20    .10 0   -.10   -.20   -.40    -.60   -.80 -1 -1 -1 -1 
//float SM[19]=      {1, 1,   1,  1, 1,0.80,0.60,0.40,0.20, 0,-0.20, -0.40,-0.60,-0.80,-1,  -1, -1, -1, -1 };
float SM[19]=      {1, 1,   1,  1, 0.60,0.40,0.30,0.20,0.10, 0,-0.10, -0.20,-0.30,-0.40,-0.60,  -1, -1, -1, -1 };
//float SM[19]={1,1, 1,  1,  0.80,0.60,0.40,0.20,0.10, 0,-0.10, -0.20,-0.40,-0.60,-0.80,-1, -1, -1, -1 };
int angles[19] = {-90,-75,-60,-45,-30,20,15,10,5,0,5,10,15,20,30,45,60,75,90};
//int angles[19] = {-90,-80,-70,-60,-50,40,30,20,10,0,10,20, 30,40,50,60,70,80,90};

// m the max of track sensors, M>=40 steer=1
// M<= -40 steer=-1
// s0= 0, s10=  , s=20=  , 
    
                               
float mx=cs.getTrack(9);
 int ind_mx=9; float SMAP,SMAV;
for(int i=10;i<19;i++)
{
       if (mx<cs.getTrack(i)) {mx=cs.getTrack(i); ind_mx=i;}; 
        if (mx<cs.getTrack(18-i)) {mx=cs.getTrack(18-i); ind_mx=18-i;}; 
}
   float M=angles[ind_mx];    
 if (M<=-40) return 1;  else if (M>=40) return -1;   
 else
 {             if (ind_mx==0){
               
              return SM[ind_mx]+(-cs.getTrack(ind_mx+1)*(SM[ind_mx]-SM[ind_mx+1]))/(mx);           
              }
               else if (ind_mx==19){
      
       return SM[ind_mx]+(cs.getTrack(ind_mx-1)*(SM[ind_mx]-SM[ind_mx-1]))/(mx);           
}
 else {return SM[ind_mx]+(cs.getTrack(ind_mx-1)*SMAV-cs.getTrack(ind_mx+1)*SMAP)/(mx); }           

}       
	// steering angle is compute by correcting the actual car angle w.r.t. to track 
	// axis [cs.getAngle()] and to adjust car position w.r.t to middle of track [cs.getTrackPos()*0.5]
 ////   float targetAngle=(cs.getAngle()-cs.getTrackPos()*0.5);
    // at high speed reduce the steering command to avoid loosing the control
 ///   if (cs.getSpeedX() > steerSensitivityOffset)
  ///      return targetAngle/(steerLock*(cs.getSpeedX()-steerSensitivityOffset)*wheelSensitivityCoeff);
 ///   else
    ////    return (targetAngle)/steerLock;
 //   return (steer);
        
        

}

///////////////////////////
float
SM_Driver::getAccel(CarState &cs)
{
    // checks if car is out of track
    if (cs.getTrackPos() < 1 && cs.getTrackPos() > -1)
    {
        // reading of sensor at +5 degree w.r.t. car axis
        float rxSensor=cs.getTrack(10);
        // reading of sensor parallel to car axis
        float cSensor=cs.getTrack(9);
        // reading of sensor at -5 degree w.r.t. car axis
        float sxSensor=cs.getTrack(8);

        

     float max10=fmax(cs.getTrack(10),cs.getTrack(8));
        // reading of sensor parallel to car axis
        float fr=cs.getTrack(9);
        // reading of sensor at -5 degree w.r.t. car axis
        float max20=fmax(cs.getTrack(7),cs.getTrack(11));
	double output ;
	double sum = 0;
dataInput.input[0]=fr;
dataInput.input[1]=max10;
dataInput.input[2]=max20;
	int numberOfRule=7;
//targetSpeed= defuzzyfication(rules, numberOfRule, dataInput);
	


        // track is straight and enough far from a turn so goes to max speed
        if ((cSensor>=maxSpeedDist) || (cSensor>=maxSpeedDist) || (cSensor >= maxSpeedDist))
            targetSpeed = maxSpeed;
        else
        {
            
          targetSpeed= defuzzyfication(rules, numberOfRule, dataInput);  
            
            // approaching a turn on right
            /*
            if(rxSensor>sxSensor)
            {
                // computing approximately the "angle" of turn
                float h = cSensor*sin5;
                float b = rxSensor - cSensor*cos5;
                float sinAngle = b*b/(h*h+b*b);
                // estimate the target speed depending on turn and on how close it is
                targetSpeed = maxSpeed*(cSensor*sinAngle/maxSpeedDist);
            }
            // approaching a turn on left
            else
            {
                // computing approximately the "angle" of turn
                float h = cSensor*sin5;
                float b = sxSensor - cSensor*cos5;
                float sinAngle = b*b/(h*h+b*b);
                // estimate the target speed depending on turn and on how close it is
                targetSpeed = maxSpeed*(cSensor*sinAngle/maxSpeedDist);
            }
*/
        }

        // accel/brake command is expontially scaled w.r.t. the difference between target speed and current one
        return 2/(1+exp(cs.getSpeedX() - targetSpeed)) - 1;
    }
    else
        return 0.3; // when out of track returns a moderate acceleration command

}
string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}
CarControl
SM_Driver::wDrive(CarState cs)
{
 
string a=ExePath();
const char *b = "\\results\\res.txt";
a += b;
const char *path = a.c_str();


                          
              float acce,brak;
               int gear;
        //    ofstream fitest("res.txt",ios_base::app);  
      ofstream fitest(path ,ios_base::app);  
           
//        FILE * resultFile1 = fopen("results1.txt", "w");
        // compute steering
        float steer;
	// check if car is currently stuck
	if ( fabs(cs.getAngle()) > stuckAngle )
    {
		// update stuck counter
        stuck++;
    }
    else
    {
    	// if not stuck reset stuck counter
        stuck = 0;
    }

	// after car is stuck for a while apply recovering policy
    if (stuck > stuckTime)
    {
    	/* set gear and sterring command assuming car is 
    	 * pointing in a direction out of track */
    	
    	// to bring car parallel to track axis
        steer = - cs.getAngle() / steerLock; 
         gear=-1; // gear R
        
        // if car is pointing in the correct direction revert gear and steer  
        if (cs.getAngle()*cs.getTrackPos()>0)
        {
            gear = 1;
            steer = -steer;
        }

        // Calculate clutching
        clutching(cs,clutch);

        // build a CarControl variable and return it
        acce=1.0; brak=0.0;
      //  CarControl cc (1.0,0.0,gear,steer,clutch);
        //return cc;
    }

    else // car is not stuck
    {
    	// compute accel/brake command
        float accel_and_brake = getAccel(cs);
        // compute gear 
         gear = getGear(cs);
        // compute steering
         steer = getSteer(cs);
        

        // normalize steering
        if (steer < -1)
            steer = -1;
        if (steer > 1)
            steer = 1;
        
        // set accel and brake from the joint accel/brake command 
        float accel,brake;
        if (accel_and_brake>0)
        {
            accel = accel_and_brake;
            brake = 0;
        }
        else
        {
            accel = 0;
            // apply ABS to brake
            brake = filterABS(cs,-accel_and_brake);
        }

        // Calculate clutching
        clutching(cs,clutch);

        // build a CarControl variable and return it
        acce=accel; brak=brake;
     //   CarControl cc(accel,brake,gear,steer,clutch);
       // return cc;
    }
    

    
   CarControl cc(acce,brak,gear,steer,clutch);
     fitest<<cs.getSpeedX()<< endl<<cs.getDamage()<<endl;//<<","<<targetSpeed <<endl;
          
//fitest<<"in:"<<cs.getTrack(9)<<","<<fmax(cs.getTrack(8),cs.getTrack(10))<<","<<fmax(cs.getTrack(7),cs.getTrack(11))<<","<<cs.getAngle()<<","<<cs.getSpeedZ()<<","<<cs.getSpeedX()<<","<<cs.getSpeedY()<<"\n out:"<<steer<<","<<acce<<","<<targetSpeed <<endl;
 
 //	fprintf(resultFile1,"in: %f,%f,%f,%f,%f,%f,%f\n out:%f,%f,%f\n", cs.getTrack(9),max(cs.getTrack(8),cs.getTrack(10)),max(cs.getTrack(7),cs.getTrack(11)),cs.getAngle(),cs.getSpeedZ(),cs.getSpeedX(),cs.getSpeedY(),steer,acce,targetSpeed );
//  	fprintf(resultFile, buf,"\n");

  //  fitest<<"in: "<<cs.getTrack(9)<<","<<max(cs.getTrack(8),cs.getTrack(10))<<","<<max(cs.getTrack(7),cs.getTrack(11))<<","<<cs.getAngle()<<","<<cs.getSpeedZ()<<","<<cs.getSpeedX()<<",";
//fitest<<cs.getSpeedY()<<endl;
//fitest<<"out: "<<steer<<","<<acce<<","<<targetSpeed<<endl; 
        return cc;  
}

float
SM_Driver::filterABS(CarState &cs,float brake)
{
	// convert speed to m/s
	float speed = cs.getSpeedX() / 3.6;
	// when spedd lower than min speed for abs do nothing
    if (speed < absMinSpeed)
        return brake;
    
    // compute the speed of wheels in m/s
    float slip = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        slip += cs.getWheelSpinVel(i) * wheelRadius[i];
    }
    // slip is the difference between actual speed of car and average speed of wheels
    slip = speed - slip/4.0f;
    // when slip too high applu ABS
    if (slip > absSlip)
    {
        brake = brake - (slip - absSlip)/absRange;
    }
    
    // check brake is not negative, otherwise set it to zero
    if (brake<0)
    	return 0;
    else
    	return brake;
}

void
SM_Driver::onShutdown()
{
    cout << "Bye bye!" << endl;
}

void
SM_Driver::onRestart()
{
    cout << "Restarting the race!" << endl;
}

void
SM_Driver::clutching(CarState &cs, float &clutch)
{
  double maxClutch = clutchMax;

  // Check if the current situation is the race start
  if (cs.getCurLapTime()<clutchDeltaTime  && stage==RACE && cs.getDistRaced()<clutchDeltaRaced)
    clutch = maxClutch;

  // Adjust the current value of the clutch
  if(clutch > 0)
  {
    double delta = clutchDelta;
    if (cs.getGear() < 2)
	{
      // Apply a stronger clutch output when the gear is one and the race is just started
	  delta /= 2;
      maxClutch *= clutchMaxModifier;
      if (cs.getCurLapTime() < clutchMaxTime)
        clutch = maxClutch;
	}

    // check clutch is not bigger than maximum values
	clutch = min(maxClutch,double(clutch));

	// if clutch is not at max value decrease it quite quickly
	if (clutch!=maxClutch)
	{
	  clutch -= delta;
	  clutch = max(0.0,double(clutch));
	}
	// if clutch is at max value decrease it very slowly
	else
		clutch -= clutchDec;
  }
}

void
SM_Driver::init(float *angles)
{
string a=ExePath();
const char *b = "\\results\\genes.txt";
a += b;
const char *path = a.c_str();


ifstream f1;
f1.open(path,ios_base::in);
double tra[36]; 
int counter = 0;
while (counter < 36)
{
   f1 >> tra[counter];
   counter++;
}     
f1.close();  
//   ofstream fitest;
//fitest.open("D:\\journal\\conferences\\2018\\torcs\\GA\\res.txt",ios_base::app);  

	// set angles as {-90,-75,-60,-45,-30,20,15,10,5,0,5,10,15,20,30,45,60,75,90}

	for (int i=0; i<5; i++)
	{
		angles[i]=-90+i*15;
		angles[18-i]=90-i*15;
	}

	for (int i=5; i<9; i++)
	{
			angles[i]=-20+(i-5)*5;
			angles[18-i]=20-(i-5)*5;
	}
    	angles[9]=0;
/*
McLaren F1
	for (int i=10; i<19; i++)
	{
			angles[i]=(i-9)*10;
			angles[18-i]=-1*(i-9)*10;
	}

*/	
	
	int numberOfMember=3;
	members = new struct Member[numberOfMember];  
 		int numberOfFuzzySet=3;
		double dtemp1, dtemp2,dtemp3;
		char ctemp;
		string  stemp;   
	/*
	Initialize member function
	*/
 ///ifstream f1;
////    f1.open("D:\\journal\\conferences\\2018\\torcs\\GA\\fichi.txt",ios_base::in);
 
///int nn[18]; 



   /// int counter = 0;
    ///while (counter < 18)
    ///{
     ///   f1 >> nn[counter];
     ///   counter++;
   /// }     
  ///for (int y=0;y<18;y++) std::cout << nn[y] <<  "\n";
///f1.close();   
     
     
     
///	0 20 50 80 100
 
  //ofstream fitest1("D:\\resultsnn1.txt",ios_base::trunc);  
///0 93 98 100 100 100
//0 88 92 98 98 100
//0 56 73 100 100 100
double lef[9], rig[9], mid[9], mid1[9];int t=0;
for (int r=0;r<36;r+=4)  {  lef[t++]=tra[r];}t=0;
for (int r=1;r<36;r+=4)  {  mid[t++]=tra[r];}t=0;
for (int r=2;r<36;r+=4)  {  mid1[t++]=tra[r];}t=0;
for (int r=3;r<36;r+=4)  {  rig[t++]=tra[r];}t=0;

//for (int t=0;t<9;t++)  { fitest1<<"in:"<<lef[t]<<" salem "<<mid[t]<<","<<mid1[t]<<","<<rig[t] <<endl;}    

 
//double lef[9]={0,20 ,50 ,0 , 10, 40, 0, 0,30 };
//double rig[9]={50, 80,100 ,40 ,70 ,100 ,30 ,60 ,100};
//double mid[9]={0 , 50, 80, 10,40 ,70 ,0,30 ,60};
//double mid1[9]={20 , 50, 80, 10,40 ,70 ,0,30 ,60};
string no[9]={"low_front","middle_front","high_front","low_max10","middle_max10","high_max10","low_max20","middle_max20","high_max20"};
char typ[9]={'h','t','b','h','t','b','h','t','b'};

/////////  front
int k=0;
for (int i=0;i<numberOfMember;i++)
{ 
members[i].numberOfValue = 3;
numberOfFuzzySet=3;
members[i].fuzzySet = new CTriangle[numberOfFuzzySet];
for (int j=0;j<numberOfFuzzySet;j++)
{

members[i].fuzzySet[j].setInterval(lef[k], rig[k]); 
members[i].fuzzySet[j].setMiddle(mid[k]);
members[i].fuzzySet[j].setMiddle1(mid1[k]);
members[i].fuzzySet[j].setType(typ[k]);
members[i].fuzzySet[j].setName(no[k]);  
k++;
    
}
    
}


/////////////////	
	
	
	int numberOfRule=7;
rules = new struct Rule[numberOfRule];	
	int itemp1, itemp2;
	
	
int itemp00[7]={ 0 ,  0 , 0 , 0 ,0 ,0  , 0};
int itemp01[7]={ 2 ,  1 , 0 , 0 ,0 ,0  , 0};
int itemp10[7]={-1 , -1 , 1 , 1 ,1 ,1  , 1};	
int itemp11[7]={-1 , -1 , 2 , 1 ,0 ,0  , 0};
int itemp20[7]={-1 , -1 ,-1 , -1,2 ,2  , 2};	
int itemp21[7]={-1 , -1 , 0 , 0 ,0 ,0  , 0};


int op[7]={-1 , -1 , 0  , 0 ,0 ,0  , 0};	
//int rs[7]={140 , 130 , 110 , 95 ,70 ,45  , 20 };	
int rs[7]={190 , 170 , 150 , 125 ,100 ,75  , 50 };	
	for(int i = 0; i < numberOfRule; i++)
	{	
		rules[i].leftSide[0][0] = itemp00[i]; rules[i].leftSide[0][1] = itemp01[i];
		rules[i].leftSide[1][0] = itemp10[i]; rules[i].leftSide[1][1] = itemp11[i];
		rules[i].leftSide[2][0] = itemp20[i]; rules[i].leftSide[2][1] = itemp21[i];
		rules[i].fuzzyOperator = op[i];
		rules[i].rightSide[0] = rs[i]; 
   }


	
	
}
