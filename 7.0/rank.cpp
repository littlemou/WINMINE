#include "rank.h"
#include "ui_rank.h"
#include <QDebug>
idinfo blbl[11];
int pn=0,tn=0;
rank::rank(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::rank)
{
    ui->setupUi(this);

    for(int i=0;i<2;i++){
        for(int j=i+1;j<2;j++){
            QString temp;int tem;
            if(blbl[j].time<blbl[i].time){
                temp=blbl[j].playername;blbl[j].playername=blbl[i].playername;blbl[i].playername=temp;
                tem=blbl[j].time;blbl[j].time=blbl[i].time;blbl[i].time;
            }
        }
    }
    ui->name1->setText(blbl[0].playername);
    ui->name2->setText(blbl[2].playername);
    ui->name3->setText(blbl[3].playername);
    ui->name4->setText(blbl[4].playername);
    ui->name5->setText(blbl[5].playername);

}
void rank::setplayername(QString s){
    if(pn<10){
        blbl[pn].playername=s;
        pn++;
    }
    else {
        blbl[10].playername=s;
    }

}
void rank::settime(int sj){
    if(tn<10){
        blbl[tn].time=sj;
        tn++;
    }
    else {
        blbl[10].time=sj;
    }

}
void rank::px(){
    int num=tn;
    if(num!=10)num=tn-1;
     for(int i=0;i<tn-1;i++){
        for(int j=i+1;j<tn;j++){
            QString temp;int tem;
            if(blbl[j].time<blbl[i].time){
                temp=blbl[j].playername;blbl[j].playername=blbl[i].playername;blbl[i].playername=temp;
                tem=blbl[j].time;blbl[j].time=blbl[i].time;blbl[i].time=tem;
            }
        }
    }
     QString s;
     switch (num) {
     case 5:ui->name5->setText(blbl[4].playername);
         s=QString::number(blbl[4].time,10);
         ui->time5->setText(s);
     case 4: ui->name4->setText(blbl[3].playername);
         s=QString::number(blbl[3].time,10);
         ui->time4->setText(s);

     case 3:ui->name3->setText(blbl[2].playername);
         s=QString::number(blbl[2].time,10);
         ui->time3->setText(s);

     case 2:ui->name3->setText(blbl[2].playername);
            s=QString::number(blbl[2].time,10);
            ui->time3->setText(s);


          case 1:ui->name2->setText(blbl[1].playername);
                 s=QString::number(blbl[1].time,10);
                 ui->time2->setText(s);

          case 0:ui->name1->setText(blbl[0].playername);
                 s=QString::number(blbl[0].time,10);
                 ui->time1->setText(s);


     }

//     ui->name6->setText(blbl[5].playername);
//     ui->name7->setText(blbl[6].playername);
//     ui->name8->setText(blbl[7].playername);
//     ui->name9->setText(blbl[8].playername);
//     ui->name10->setText(blbl[9].playername);
}
rank::~rank()
{
    delete ui;
}
