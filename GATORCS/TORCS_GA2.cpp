#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#define x 100

typedef int vect[x];
using namespace std;
const int taille_pop= 20;
int tournamentSize=2;
int mutationRate=5;
int crossoverRate = 90;
int iteration_number=10;
const int NG= 36;
int pop[taille_pop][NG];
double fit[taille_pop];
vect ind; 
double CR=0.7;
double best;
int besti[NG];
int tab[x][x];
float tabfit[x];
float sommeFitness=0;
string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}

void gen_individu(vect ind){
/*fstream f("fichier.txt",ios_base::in | ios_base::out | ios_base::trunc);
if (f.is_open()){     
srand(time(NULL));       */          
                     int c= 0;
                     ind[0]=0;
                     //f<<ind[0]; 
                     //f<<" "; 
                     for(int j=1;j<11;j++){ ind[j] = rand() % (100 - c + 1) + c; 
                     
                                            if (j%4!=0) {
                                            while (ind[j]< c) {ind[j] = rand() % (101 - c + 1) + c;}
                                          c = ind[j];} else
                                            {
                                            while (ind[j]>= c) {ind[j] = rand() % (c );}
                                          c = ind[j];} ;                                        
                                          
                                          }
                      ind[11]=100;
                      //f<<ind[5];
                      //f<<" "; 
                      ind[12]=0;
                      //f<<ind[6];
                      //f<<" ";  
                      c= 0;                                                             
                      for(int j=13;j<23;j++){ ind[j] = rand() % (100 - c + 1) + c;
                                           if (j%4!=0) {
                                            while (ind[j]< c) {ind[j] = rand() % (101 - c + 1) + c;}
                                          c = ind[j];}else
                                            {
                                            while (ind[j]>= c) {ind[j] = rand() % (c );}
                                          c = ind[j];} ; 
                                          } 
                      ind[23]=100;
                      //f<<ind[11];
                      //f<<" "; 
                      ind[24]=0;
                      //f<<ind[12]; 
                      //f<<" ";  
                      c= 0;                                                         
                      for(int j=25;j<35;j++){ ind[j] = rand() % (100 - c + 1) + c; 
                                       if (j%4!=0) {
                                            while (ind[j]< c) {ind[j] = rand() % (101 - c + 1) + c;}
                                          c = ind[j];} else
                                            {
                                            while (ind[j]>= c) {ind[j] = rand() % (c );}
                                          c = ind[j];} ; } 
                      ind[35]=100;
                      //f<<ind[17];                       
//f.close();}
//system("AT2\\client.exe" );
                                                 //for(int j=0; j<18;j++)tab[i][j]=ind[j];}                                                  
   }

      
   
     
//calcule de fitness qui ne permettra d'évaluer l'individu 
float fitness(vect ind) {  float fitnes=0;
//char* path=ExePath()+"\\results\\fichier.txt";
string z,a=ExePath();
const char *b = "\\WCCI\\results\\genes.txt";
z=a;
z += b;
const char *path = z.c_str();

 std::ofstream f(path, ios_base::trunc);
 if (f.is_open()){ for(int j=0; j<NG; j++) f << ind[j]<<" "; f <<endl;  f.close();}
b = "\\tt.bat";
z=a;
z += b;
path = z.c_str();
WinExec(path,SW_SHOWMAXIMIZED);   

a=ExePath();
b = "\\WCCI\\results\\res.txt";
z=a;
z += b;
path = z.c_str();
ofstream fitest(path ,ios_base::trunc); 
fitest.close();


                         
b = "\\WCCI\\client.exe";
z=a;
z += b;
path = z.c_str();

SHELLEXECUTEINFO ShExecInfo = {0};
ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
ShExecInfo.hwnd = NULL;
ShExecInfo.lpVerb = NULL;
ShExecInfo.lpFile = path;        
ShExecInfo.lpParameters = "";   
ShExecInfo.lpDirectory = NULL;
ShExecInfo.nShow = SW_HIDE;
ShExecInfo.hInstApp = NULL; 
ShellExecuteEx(&ShExecInfo);
WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
                         

b = "\\WCCI\\results\\res.txt";
z=a;
z += b;
path = z.c_str();

 std::ifstream myfile(path, ios::in); 
 std::vector<double> results;
 std::vector<double> Vspeed;
std::vector<double> speed;
std::vector<double> damage;
float num;
//std::string line;    double i;
//while(std::getline(myfile, line))
//{
  //  std::stringstream ss(line);

    //while( ss >> i ) 

    //results.push_back(i);
//}

while(myfile>>num)
{
  //  std::stringstream ss(line);

    //while( ss >> i ) 

    results.push_back(num);
}
for(long ei=0; ei<results.size(); ei+=2) { speed.push_back(results[ei]); damage.push_back(results[ei+1]);}
double  result2 = *max_element(damage.begin(), damage.end());

size_t ii=0;
while (ii<speed.size())
{      
  if ((speed[ii]>speed[ii+1])&&(speed[ii]>20)){//cout<<speed[ii]<<endl;
  Vspeed.push_back(speed[ii]);};
  ii++; }
double result = *max_element(Vspeed.begin(), Vspeed.end());
double result1 = *min_element(Vspeed.begin(), Vspeed.end());
fitnes=(result*result1)/(result2+1);
//fitnes=1/fitnes;

 b = "\\WCCI\\results\\hist.txt";
z=a;
z += b;
path = z.c_str(); 
std::ofstream fiche(path, ios::app); 
for(int j=0; j<NG; j++)
fiche<<ind[j]<<" ";

fiche << result<<" "<<result1<<" "<<result2<<" "<<fitnes<<endl;      
 fiche.close();
                           return fitnes;                                                 
 }
double somme(){                         
float sommeFitness=0;           
int i;
for (i=0;i<taille_pop;i++)sommeFitness+=fit[i];
return sommeFitness;
}
                                                                
  void gen_population()
   {
    for(int i=0;i<taille_pop;i++) {
      gen_individu(pop[i]); fit[i]=fitness(pop[i]);
                             
      }
      }
//float fitness(vect ind) { return 1./fitness1(ind);}
/*fonction qui retourne le meilleure individu de la population*/ 
int meilleur_ind(){ int id=0;
                                           for(int i=1;i<taille_pop;i++)if (fit[i]>fit[id])id=i;
                                           return id;}
/*fonction qui permet de sélectionner aléatoirement l'indice d'un individu de la populationon fera appelle a cette fonction dans selection_tournois*/ 

int pire_ind(){ int id=0;
                                           for(int i=1;i<taille_pop;i++)if (fit[i]<fit[id])id=i;
                                           return id;}

int selection_alia(){ int id; id= rand()%taille_pop; return id;}
/*la fonction selection_tournois selectionne le meilleure individu parmi des individu selectioner aliatiorement*/ 
int selection_tournois(){ int i1,i2,i3,imax;
                                         i1=selection_alia();i2=selection_alia();i3=selection_alia();
                                         imax=i1;
                                         if (fit[i2]>fit[imax])imax=i2;
                                         if (fit[i3]>fit[imax])imax=i3;
                                         return (imax);}
/*on utilise la selection_tournois pour selectionner des individus de telles sortent a ne pas dépasser le pourcentage des individus à sélectionner ici 0,7.*/ 
void selection_croisement(int *nc, int C[]){
                                     
                                         double som1=somme();        
                                         int j=0;
                                         float pc=0;
                                         while (pc<=CR){ int id;
                                                          id=selection_tournois();
                                                          pc+=fit[id]/som1;
/* ici on associe a chaque individu selectioner un segmentrelatif a sa fitness */ 
                                                          C[j]=id;
// on stok les indices des individus selectionner dans un tabeau pour les croiser aprés
                                                          *nc=j+1;
/*on incremente chaque fois la taille du tableau des ind selectioner que on selectionent pr savoir dans le croisment combien on va croiser*/ 
                                                          j++;}}
/*le même principe pour la selection pour la mutation*/ 
int *selection_mutation(int *nm, int M[]){
                         int j=0;
                         float pm=0;
                           double som1=somme();   
                         while (pm<=0.3){ int id;
                                          id=selection_tournois();
                                          pm+=fit[id]/som1;
                                          M[j]=id;*nm=j+1;j++;}
                         return M;}
/*on fait le croisement un point des ind sélectionner pour le croisement*/ 
/*
void croisement(int P1[], int P2[], int E1[], int E2[]){
               int k,k2;
               k=12; k2=24;
for(int i=0;i<k;i++)  { E1[i]= P1[i]; E2[i]= P2[i];} 
for(int i=k;i<k2;i++) { E1[i]= P2[i]; E2[i]= P1[i];}
for(int i=k2;i<NG;i++){ E1[i]= P1[i]; E2[i]= P2[i];}
 }
 */
/*On fait maintenant la mutation*/ 
//void mutation(int P[], int E[]){int i;for(i=0;i<NG;i++)E[i]=P[i];int a,b,c;
// on choisi les point de mutation aléatoirement
//do{ a=rand()%NG;b=rand()%NG;} while(a==b);
//c=E[a];
//on pose le contenu de E[a] dans un variable pour ne pas le perdre
//E[a]=E[b];E[b]=c;}          
/*************************************************** main *************************************
**********************************************************************************************/
// Apply crossover to selected chromosome pairs


void Crossover( const int& index1, const int& index2,const int& point1, const int& point2 )
{
     int E1[NG], E2[NG],P1[NG], P2[NG];
for(int j=0;j<NG;j++)P1[j]=pop[index1][j];
for(int j=0;j<NG;j++)P2[j]=pop[index2][j];
if ( point1 < 0 || point1 >= NG ) return;
if ( point2 < 0 || point2 >= NG ) return;
int p1 = point1;
int p2 = point2;
if ( p1 > p2 ) {
int tmp = p1;
p1 = p2;
p2 = tmp;
}
// Do crossover on x portion of chromosome:
// either before or after point p1

// Do crossover on x portion of chromosome
// either before or after point p2

for(int i=0;i<p1;i++)  { E1[i]= P1[i]; E2[i]= P2[i];} 
for(int i=p1;i<p2;i++) { E1[i]= P2[i]; E2[i]= P1[i];}
for(int i=p2;i<NG;i++){ E1[i]= P1[i]; E2[i]= P2[i];}

for(int j=0;j<NG;j++)pop[index1][j]=E1[j];
for(int j=0;j<NG;j++)pop[index2][j]=E2[j];
fit[index1]=fitness(E1);
fit[index2]=fitness(E2);
}
// Apply mutation to selected chromosome: x part or y part


void Mutation( const int& index )
{
int a,f,e,u;srand(time(NULL));  
int E0[NG];
for(int j=0;j<NG;j++)E0[j]=pop[index][j]; 
a=rand()%(9)+1;
if (a%4==0){ e=0;f=a-1;} else{ e=a-1;f=a+1;};
u=rand()%(f)+e;
E0[a]=u;
a=rand()%(9)+13;
if (a%4==0){ e=0;f=a-1;} else{ e=a-1;f=a+1;};
u=rand()%(f)+e;
E0[a]=u;
a=rand()%(9)+25;
if (a%4==0){ e=0;f=a-1;} else{ e=a-1;f=a+1;};
u=rand()%(f)+e;
E0[a]=u;
fit[index]=fitness(E0);
for(int j=0;j<NG;j++)pop[index][j]=E0[j];

}

void Crossover()
{
for ( int i = 0; i < taille_pop; i++ ) {
int r = rand() % 100;
if ( r < crossoverRate ) {
int index1 = rand() % taille_pop;
int index2 = rand() % taille_pop;
while ( index1 == index2 ) {
index2 = rand() % taille_pop;
}
// Get crossover points
// Point1: 0 - 31
int point1 = rand() % NG / 2;
// Point1: 32 - 64
int point2 = NG / 2 +
rand() % ( NG / 2 );
while ( point1 == point2 ) {
point2 = NG / 2 +
rand() % ( NG / 2 );
}
if ( point1 > point2 ) {
int tmp = point1;
point1 = point2;
point2 = tmp;
}
// Do 1-point crossover
Crossover( index1, index2, point1, point2 );
}
}
}
// Mutate selected chromosomes
void Mutate()
{
for ( int i = 0; i < taille_pop; i++ ) {
int r = rand() % 100;
if ( r < mutationRate ) {
Mutation( i );
}
}
}
// Select population chromosomes according to fitness

// Using a pairwise tournament selection mechanism

void Select()
{
// For each pair of chromosomes selected
int i = 0;
while ( i < tournamentSize ) 
{
      // Get the chromosome pair for tournament selection
         int index1 = rand() % taille_pop;
         int index2 = rand() % taille_pop;
         while ( index1 == index2 ) {index2 = rand() % taille_pop;}
         // We seek to find [x,y] that minimizes this function
         // The bigget the value returned, the lower its fitness
         if ( fit[index1] > fit[index2])
             {
             // Copy chromosome 1 elements into chromosome 2
             for(int j=0;j<NG;j++)pop[index1][j]=pop[index2][j];fit[index1]=fit[index2];
             }
        else {
             // Copy chromosome 2 elements into chromosome 1
             for(int j=0;j<NG;j++)pop[index2][j]=pop[index1][j];fit[index2]=fit[index1];
             }
             i++;
        }
}










int main(int argc, char *argv[]){
//time_t ti;srand ((unsigned) time(&ti));
vect ind;   
//time_t ti; srand ((unsigned) time(&ti));

int r;
//int solution [NG]={0, 50, 86, 87, 91, 100, 0, 40, 52, 74, 91, 100, 0, 97, 97, 99, 100, 100};
int h=0; 



string z,a=ExePath();
const char *b = "\\WCCI\\results\\genes.txt";
z=a;
z += b;
const char *path = z.c_str();
std::ofstream f(path, ios_base::trunc);
f.close();

b = "\\WCCI\\results\\res.txt";
z=a;
z += b;
path = z.c_str();
std::ofstream fiche1(path, ios::trunc); 
fiche1.close();

b = "\\WCCI\\results\\hist.txt";
z=a;
z += b;
path = z.c_str(); 
std::ofstream fiche(path, ios::trunc); 
fiche.close();

gen_population(); 
cout <<"Starting";

  
// system("D:\journal\\conferences\\2018\\torcs\\GA\\WCCI\\client.exe" );
 /// ShellExecute(NULL, "open", "D:\\journal\\conferences\\2018\\torcs\\GA\\WCCI\\client.exe", NULL, NULL, SW_SHOWDEFAULT);

while(h<iteration_number) 
{       
      //gen_population(pop);   
      int nc=0;
      int nm=0;
      int C[taille_pop];
      int M[taille_pop];
      selection_mutation(&nm,M);
      selection_croisement(&nc, C);
      int E[taille_pop];
///////////

Select();
Crossover();
Mutate();

//////////////////////////////
/*

      int E1[taille_pop];
      int E2[taille_pop];
      int i=0;
      cout <<"Evolution";
      while(i<nc-1){
//maintenant on vas faire la croisement de tt les individus de la population dans leur indices on étéselectioner par la sélection croisement  
                    croisement ( pop[C[i]], pop[C[i+1]], E1, E2);
                    double f1=fitness(E1);
                    double f2=fitness(E2);
                    
//on fait le remplacement pour garder que les meilleurs entre lesenfants est les parents*/ /* on a choisit de remplacer le pire des parents par le meilleur enfant si la fitness de ce dernier est plusgrande de celle du pire parent  
                    if(f1>=f2){
                         if((fit[C[i]]<f1)&&(fit[C[i]]<=fit[C[i+1]])){
                                int j; for(j=0;j<NG;j++)pop[C[i]][j]=E1[j];fit[C[i]]=f1;}
                         else if((fit[C[i+1]]<f1)&&(fit[C[i+1]]<fit[C[i]])){
                                int j; for (j=0;j<NG;j++)pop[C[i+1]][j]=E1[j];fit[C[i+1]]<f1;}}
                    else{if((fit[C[i]]<f2)&&(fit[C[i]]<=fit[C[i+1]])){
                              int j; for(j=0;j<NG;j++)pop[C[i]][j]=E2[j];fit[C[i]]=f2;}
                         else if((fit[C[i+1]]<f2)&&(fit[C[i+1]]<fit[C[i]])){
                              int j; for(j=0;j<NG;j++)pop[C[i+1]][j]=E2[j];fit[C[i+1]]=f2;}}
       i=i+2;}
// on mute la individu de la population sélectionner en sélection mutation et on remplace si enfant estmeilleur 
       while(i<nm){  mutation( pop[C[i]], E);
       double f=fitness(E);
                     if(fit[C[i]]<f){int j; for(j=0;j<NG;j++)pop[C[i]][j]=E[j];fit[C[i]]=f;}
                     i++;}
// a chaque itération on trouve l'indice de meilleur individu après tous les étapes

 */   
r=meilleur_ind();
 int t=pire_ind();   
if (best>fit[t]){ 
                   for(int j=0;j<NG;j++)pop[t][j]=besti[j]; }      

if (fit[r]>best){  for(int j=0;j<NG;j++)best=pop[r][j];best=fit[r]; }      
h++;
}                 
int i;
printf("%d\t", best);
printf("\n");printf("la soultion est "); printf("\n");
for(i=0;i<NG;i++)printf("%d\t",besti[i]);

getch(); return 0;    
}









