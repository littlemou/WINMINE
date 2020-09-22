#include "rank.h"
#include "ui_rank.h"
#include <QDebug>
idinfo blbl[9];
int pn=0,tn=0,num;
rank::rank(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::rank)
{
    ui->setupUi(this);
    readlist();
    px();
}
void rank::setplayername(QString s)
{
    if(pn<8)
    {
        blbl[pn].playername=s;
        pn++;
    }
    else
    {
        blbl[8].playername=s;pn=9;
    }

}
void rank::settime(int sj)
{
    if(tn<8)
    {
        blbl[tn].time=sj;
        tn++;
    }
    else
    {
        blbl[8].time=sj;tn=9;
    }

}
void rank::px()
{
        num=tn-1;
     for(int i=0;i<=num-1;i++)
     {
        for(int j=i+1;j<=num;j++)
        {
            QString temp;int tem;
            if(blbl[j].time<blbl[i].time)
            {
                temp=blbl[j].playername;blbl[j].playername=blbl[i].playername;blbl[i].playername=temp;
                tem=blbl[j].time;blbl[j].time=blbl[i].time;blbl[i].time=tem;
            }
        }
    }
     QString s;
     switch (num)
     {
         case 8:
         case 7:ui->name8->setText(blbl[7].playername);
            s=QString::number(blbl[7].time,10);
            ui->time8->setText(s);
          case 6:ui->name7->setText(blbl[6].playername);
            s=QString::number(blbl[6].time,10);
            ui->time7->setText(s);
          case 5:ui->name6->setText(blbl[5].playername);
                 s=QString::number(blbl[5].time,10);
                 ui->time6->setText(s);
          case 4: ui->name5->setText(blbl[4].playername);
                  s=QString::number(blbl[4].time,10);
                  ui->time5->setText(s);

          case 3:ui->name4->setText(blbl[3].playername);
                 s=QString::number(blbl[3].time,10);
                 ui->time4->setText(s);

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
}
void rank::writelist(){
    qDebug()<<"鍐欏叆";
    QFile file("ranklist.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream write(&file);

    char a[100000];
    int k=0;
    write<<pn<<'/'<<tn<<'/'<<num<<'/';
    for(int i =0;i<=num;i++)
    {
        write<<'^'<<blbl[i].playername<<'|'<<blbl[i].time<<'!';
    }
    file.close();


}
void rank::readlist(){

    QFile file("ranklist.txt");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream read(&file);
    QString line =read.readLine();
    QString pn1;
    QString tn1;
    QString num1;qDebug()<<"杩涘叆";
    for(int i =0;;i++)
    {
        if(line[i]=='/')
        {
            for(int j=i+1;;j++)
            {
                if(line[j]=='/')
                {
                    for(int k=j+1;;k++)
                    {
                        if(line[k]=='/')
                            break;
                        num1.push_back(line[k]);
                    }
                    break;
                }
                tn1.push_back(line[j]);
            }
            break;
        }
        pn1.push_back(line[i]);
    }
    pn=pn1.toInt();tn=tn1.toInt();num=num1.toInt();
    qDebug()<<pn<<" "<<tn<<" "<<num<<" ";
    qDebug()<<line;
    int k=0,namec=0,timec=0;QString zh;
    for(int i=0;i<line.length();i++)
    {
        if(line[i]=='^')
        {
            namec=1;timec=0;continue;
        }
        if(line[i]=='|')
        {
            namec=0;timec=1;continue;
        }
        if(line[i]=='!')
        {
            namec=0;
            timec=0;blbl[k].time=zh.toInt();
            zh="";
            k++;
            continue;
        }
        if(namec)
        {
            blbl[k].playername+=line[i];
        }
        if(timec)
        {
            zh+=line[i];
        }
    }
    file.close();
    update();
}
rank::~rank()
{
    writelist();
    delete ui;
}
