library(ggplot2)
library(reshape)
library(ggthemes2)

data <- read.csv("max speed and min damage .csv",sep=';')
data.factored <- melt(data,id=c('Drivers'))

ggplot(data.factored, aes(x=Drivers,y=value,fill=variable))+geom_bar(stat="Identity",position='dodge')+theme_tufte()+ theme(text = element_text(size=20),axis.text.x = element_text(angle = 90, hjust = 1))  + scale_fill_brewer(palette="Set2") + ylab("Races") + xlab("Controllers")

