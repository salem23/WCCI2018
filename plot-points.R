library(ggplot2)
library(reshape)
library(ggthemes2)

data <- read.csv("championship points.csv",sep=';')

data$score <- rowSums(data[,2:11])
)
data.factored <- melt(data,id=c('Drivers','score'))
ggplot(data.factored, aes(color=score,x=reorder(Drivers, - score),y=value,position="stack",fill=value))+geom_bar(stat="Identity")+theme_tufte()+ theme(text = element_text(size=16),axis.text.x = element_text(angle = 90, hjust = 1))  + scale_fill_gradientn(colours=rainbow(4)) + ylab("Total Score") + xlab("Controllers")

