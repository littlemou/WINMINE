#include "winmine.h"
#include "ui_winmine.h"
#include <iostream>
#include<QTextStream>
#include <QGraphicsWidget>
#include <QPropertyAnimation>

int initial_row=9;
int initial_col=9;
int initial_boomnum=10;
int click_x,click_y,timeend=1;

QPainter *paintmywindow;
block *mine=new block(initial_row,initial_col,initial_boomnum);
int length=mine->getcol()*35;
int height=mine->getrow()*35+title_height;
int boom_number=mine->getboomnum();
int flag_number=boom_number;
int win();

//炸弹为99
//插旗+50，拔旗-50
//遍历显示+100
//问号+200
//row:列
//col:行
//height和row连，length和col连

Winmine::Winmine(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Winmine)
{
    ui->setupUi(this);
    connect(ui->Easy,&QAction::triggered,this,&Winmine::easy);
    connect(ui->Middle,&QAction::triggered,this,&Winmine::middle);
    connect(ui->Hard,&QAction::triggered,this,&Winmine::hard);
    connect(ui->Customize,&QAction::triggered,this,&Winmine::window_show);

    void(customize_window::*funslot)(int row,int col,int boomnum)=&customize_window::slot;
    connect(&w,funslot,this,&Winmine::setcustomize);

    connect(ui->View_Information,&QAction::triggered,this,&Winmine::list_show);

    connect(ui->save,&QAction::triggered,this,&Winmine::write_setting);
    connect(ui->last_game,&QAction::triggered,this,&Winmine::read_setting);
    connect(ui->Clear,&QAction::triggered,this,&Winmine::clearfile);
    connect(ui->actionClear,&QAction::triggered,this,&Winmine::clearlist);

    //新信号
    void(dialog::*okclicked)(QString s)=&dialog::slotname;
    connect(&w3,okclicked,this,&Winmine::readplayername);


    runtime = new QTimer(this);
    connect(runtime,SIGNAL(timeout()),this,SLOT(second()));
    connect(runtime,SIGNAL(timeout()),this,SLOT(update()));
}

void Winmine::readplayername(QString s)
{
    w2.setplayername(s);
    int zzcj=mine->gettimenum();
    w2.settime(zzcj);
    w2.px();
}

void Winmine::mousePressEvent(QMouseEvent *event)
{
    click_x=event->x();
    click_y=event->y();
    if(click_x>block_length*mine->getcol()/2-block_length/2&&click_x<block_length*mine->getcol()/2-block_length/2+block_length
       &&click_y>title_begin+title_height/3&&click_y<title_begin+title_height/3+block_height)
    {
        restart_clicked();
        paint_flag="none";
        end=0;
        runtime->stop();
        update();
        return;
    }
    if(click_y<title_height)
    {
        return;
    }
    else
    if(end==1)
    {
        return;
    }
    click_x=click_x/block_length;
    click_y=(click_y-title_height)/block_height;
    if(event->buttons()==(Qt::LeftButton))
    {
        if(timeend==1)
           runtime->start(1000);
        if(mine->p[click_y][click_x]==99)
        {
            paint_flag="boom";
            mine->p[click_y][click_x]=199;
            update();
            end=1;
            timeend=1;   //游戏结束，停止计时
            runtime->stop();
            QMessageBox::warning(this,"END","YOU LOSE");
        }
        else
        if(mine->p[click_y][click_x]<10)
        {
            if(mine->p[click_y][click_x]!=0)
            {
                mine->p[click_y][click_x]=mine->p[click_y][click_x]+100;
            }
            else
            {
                showblock(click_y,click_x);
            }
            paint_flag="number";
            update();
            if(win())
            {
                timeend=1;
                end = 1;
                runtime->stop();
                QMessageBox::warning(this,"END","YOU WIN");
                dialogshow();
            }
        }
        else
        {
            return;
        }
    }
    else
    if(event->buttons()==(Qt::RightButton))
    {
        if((mine->p[click_y][click_x]<10)||(mine->p[click_y][click_x]==99))//判断未插旗方块，表示插旗
        {
            if(flag_number==0)
            {
                return;
            }
            mine->p[click_y][click_x]=mine->p[click_y][click_x]+50;
            flag_number--;//控制插旗数量
            if(boom_number==0)
            {
                QMessageBox::warning(this,"YOU WIN","YOU WIN");
            }
            paint_flag="flag";
        }
        else
        if((mine->p[click_y][click_x]>=50&&mine->p[click_y][click_x]<60)||(mine->p[click_y][click_x]==149))//判断插旗方块，表示打问号
        {
            mine->p[click_y][click_x]=mine->p[click_y][click_x]+150;
            paint_flag="question";
            flag_number++;
        }
        else
        if((mine->p[click_y][click_x]>=200&&mine->p[click_y][click_x]<210)||(mine->p[click_y][click_x]==299))//判断问号方块，去掉问号
        {
            mine->p[click_y][click_x]=mine->p[click_y][click_x]-200;
            paint_flag=="flag";
        }
        update();
        if(win()==1)
        {
            timeend=1;
            end = 1;
            runtime->stop();
            QMessageBox::warning(this,"END","YOU WIN");
            dialogshow();
        }
    }
}

void Winmine::paintEvent(QPaintEvent *event)
{
    this->setFixedSize(mine->getcol()*block_length,mine->getrow()*block_height+title_height);
    QPainter painter(this);
    paintmywindow=&painter;
    painttitle(paintmywindow);
    QPixmap initial("://images/initial.png");
    if(paint_flag=="none")//画出初始界面
    {
        for(int i=0;i<mine->getcol()*block_length;i+=block_length)
        {
            for(int j=title_height;j<mine->getrow()*block_height+title_height;j+=block_height)
            {
                painter.drawPixmap(i,j,initial,0,0,block_length,block_length);
            }
        }
    }
    else
    if(paint_flag=="boom")
    {
        QPixmap initial("://images/initial.png");
        QPixmap explode("://images/explode.png");
        QPixmap flag("://images/flag.png");
        QPixmap flag_explode("://images/flag_explode.png");
        QPixmap blank("://images/blank.png");
        QPixmap question("://images/question.png");
        QPixmap blk_1("://images/1.png");
        QPixmap blk_2("://images/2.png");
        QPixmap blk_3("://images/3.png");
        QPixmap blk_4("://images/4.png");
        QPixmap blk_5("://images/5.png");
        QPixmap blk_6("://images/6.png");
        QPixmap blk_7("://images/7.png");
        QPixmap blk_8("://images/8.png");

        QPixmap flagblank("://images/flagblank.png");
        QPixmap wrongflag("://images/wrongflag");

        for(int i=0;i<mine->getrow();i++)//重新绘制雷区
        {
            for(int j=0;j<mine->getcol();j++)
            {
                if(mine->p[i][j]<10)
                {
                    painter.drawPixmap(j*block_length,i*block_height+title_height,initial,0,0,block_length,block_height);
                }
                else if(mine->p[i][j]==99)
                {
                    painter.drawPixmap(j*block_length,i*block_height+title_height,explode,0,0,block_length,block_height);
                }
                else if(mine->p[i][j]==149)
                {
                    painter.drawPixmap(j*block_length,i*block_height+title_height,flag_explode,0,0,block_length,block_height);
                }
                else if(mine->p[i][j]==299||(mine->p[i][j]>=200&&mine->p[i][j]<210))
                {
                    painter.drawPixmap(j*block_length,i*block_height+title_height,question,0,0,block_length,block_height);
                }
                else if((mine->p[i][j]<60)&&(mine->p[i][j]>=50))
                {
                      painter.drawPixmap(j*block_length,i*block_height+title_height,wrongflag,0,0,block_length,block_height);
                }
                else if((mine->p[i][j])>99&&(mine->p[i][j]<110))
                {
                   int blk_num=mine->p[i][j]-100;
                   switch (blk_num)
                   {
                    case 0: painter.drawPixmap(j*block_length,i*block_height+title_height,blank,0,0,block_length,block_height);break;
                    case 1: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_1,0,0,block_length,block_height);break;
                    case 2: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_2,0,0,block_length,block_height);break;
                    case 3: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_3,0,0,block_length,block_height);break;
                    case 4: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_4,0,0,block_length,block_height);break;
                    case 5: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_5,0,0,block_length,block_height);break;
                    case 6: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_6,0,0,block_length,block_height);break;
                    case 7: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_7,0,0,block_length,block_height);break;
                    case 8: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_8,0,0,block_length,block_height);break;
                   }
                }
                else if(mine->p[i][j]==199)
                {
                    painter.drawPixmap(j*block_length,i*block_height+title_height,explode,0,0,block_length,block_height);
                }
             }
         }
         painter.drawPixmap(click_x*block_length,click_y*block_height+title_height,explode,0,0,block_length,block_height);
    }
    else
    if(paint_flag=="number")
    {
        paintboom(paintmywindow);
    }
    else
    if(paint_flag=="flag")
    {
        paintboom(paintmywindow);
    }
    else
    if(paint_flag=="question")
    {
        paintboom(paintmywindow);
    }
}

void showblock(int x, int y)
{
    for(int row=x-1;row<=x+1;row++)
    {
        for(int col=y-1;col<=y+1;col++)
        {
            if(row<0||col<0||row>=mine->getrow()||col>=mine->getcol())
            {
                continue;
            }
            if(mine->p[row][col]<10)
            {
                if(mine->p[row][col]==0)
                {
                    mine->p[row][col]=100;
                    showblock(row,col);
                }
                else
                {
                    mine->p[row][col]=mine->p[row][col]+100;
                }
            }
        }
    }
}

void Winmine::painttitle(QPainter *painter)
{
    QPixmap initial("://images/initial.png");
    QPixmap explode("://images/explode.png");
    QPixmap flag("://images/flag.png");
    QPixmap blank("://images/blank.png");
    QPixmap question("://images/question.png");
    QPixmap blk_1("://images/1.png");
    QPixmap blk_2("://images/2.png");
    QPixmap blk_3("://images/3.png");
    QPixmap blk_4("://images/4.png");
    QPixmap blk_5("://images/5.png");
    QPixmap blk_6("://images/6.png");
    QPixmap blk_7("://images/7.png");
    QPixmap blk_8("://images/8.png");
    QPixmap title("://images/title.bmp");
    QPixmap again("://images/again.png");
    QPixmap again2("://images/again2.png");
    QPixmap lose("://images/lose.png");
    QPixmap digital0("://images/time_digital0.png");
    QPixmap digital1("://images/time_digital1.png");
    QPixmap digital2("://images/time_digita2.png");
    QPixmap digital3("://images/time_digita3.png");
    QPixmap digital4("://images/time_digita4.png");
    QPixmap digital5("://images/time_digita5.png");
    QPixmap digital6("://images/time_digita6.png");
    QPixmap digital7("://images/time_digita7.png");
    QPixmap digital8("://images/time_digita8.png");
    QPixmap digital9("://images/time_digita9.png");



    painter->drawPixmap(block_length*mine->getcol()/2-block_length/2,title_begin+title_height/3,again2,0,0,block_length,block_height);

    if(paint_flag=="boom")
    {
        painter->drawPixmap(block_length*mine->getcol()/2-block_length/2,title_begin+title_height/3,lose,0,0,block_length,block_height);
    }
    //显示旗子
    switch  (flag_number/100)
    {
    case 0:painter->drawPixmap(0,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(0,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(0,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(0,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(0,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(0,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(0,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(0,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(0,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(0,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    switch  ((flag_number/10)%10)
    {
    case 0:painter->drawPixmap(35,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(35,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(35,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(35,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(35,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(35,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(35,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(35,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(35,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(35,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    switch  (flag_number%10)
    {
    case 0:painter->drawPixmap(70,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(70,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(70,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(70,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(70,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(70,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(70,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(70,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(70,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(70,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    //显示时间
    int timenum = mine->gettimenum();
    length=mine->getcol()*35;
    switch  (timenum/100)
    {
    case 0:painter->drawPixmap(length-105,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(length-105,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(length-105,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(length-105,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(length-105,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(length-105,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(length-105,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(length-105,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(length-105,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(length-105,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    switch  ((timenum/10)%10)
    {
    case 0:painter->drawPixmap(length-70,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(length-70,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(length-70,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(length-70,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(length-70,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(length-70,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(length-70,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(length-70,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(length-70,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(length-70,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    switch  (timenum%10)
    {
    case 0:painter->drawPixmap(length-35,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(length-35,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(length-35,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(length-35,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(length-35,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(length-35,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(length-35,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(length-35,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(length-35,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(length-35,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
}

int win()
{

    int sz= 0,boompc=0;
    for(int row = 0; row <mine->getrow();row++)
        for(int col =0;col<mine->getcol();col++)
        {
            if((mine->p[row][col]>=100)&&(mine->p[row][col]<110))
                sz++;
            if(mine->p[row][col]==149)boompc++;
        }
    if(sz>=((mine->getcol()*mine->getrow())-mine->getboomnum())||boompc==boom_number)
        return 1;
    else
        return 0;
}

void Winmine::dialogshow()
{
    w3.show();
}

void Winmine::second()
{
    mine->addtimeNum();
}

void paintboom(QPainter *painter)
{
    QPixmap initial("://images/initial.png");
    QPixmap explode("://images/explode.png");
    QPixmap flag("://images/flag.png");
    QPixmap blank("://images/blank.png");
    QPixmap question("://images/question.png");
    QPixmap blk_1("://images/1.png");
    QPixmap blk_2("://images/2.png");
    QPixmap blk_3("://images/3.png");
    QPixmap blk_4("://images/4.png");
    QPixmap blk_5("://images/5.png");
    QPixmap blk_6("://images/6.png");
    QPixmap blk_7("://images/7.png");
    QPixmap blk_8("://images/8.png");

    for(int i=0;i<mine->getrow();i++)
    {
        for(int j=0;j<mine->getcol();j++)
        {
            if(mine->p[i][j]<10||mine->p[i][j]==99)//未变化
            {
                painter->drawPixmap(j*block_length,i*block_height+title_height,initial,0,0,block_length,block_height);
            }
            else
            if((mine->p[i][j]>=50&&mine->p[i][j]<60)||mine->p[i][j]==149)//插旗
            {
                painter->drawPixmap(j*block_length,i*block_height+title_height,flag,0,0,block_length,block_height);
            }
            else
            if((mine->p[i][j]>=200&&mine->p[i][j]<210)||mine->p[i][j]==299)
            {
                painter->drawPixmap(j*block_length,i*block_height+title_height,question,0,0,block_length,block_height);
            }
            else
            if(mine->p[i][j]>99&&mine->p[i][j]<110)
            {
                int num=mine->p[i][j]-100;
                switch (num)
                {
                    case 0:painter->drawPixmap(j*block_length,i*block_height+title_height,blank,0,0,block_length,block_height);break;
                    case 1:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_1,0,0,block_length,block_height);break;
                    case 2:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_2,0,0,block_length,block_height);break;
                    case 3:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_3,0,0,block_length,block_height);break;
                    case 4:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_4,0,0,block_length,block_height);break;
                    case 5:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_5,0,0,block_length,block_height);break;
                    case 6:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_6,0,0,block_length,block_height);break;
                    case 7:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_7,0,0,block_length,block_height);break;
                    case 8:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_8,0,0,block_length,block_height);break;
                    default:break;
                }
            }
            else
            if(mine->p[i][j]==199)
            {
                painter->drawPixmap(j*block_length,i*block_height+title_height,explode,0,0,block_length,block_height);
            }
        }
    }
}

void Winmine::restart_clicked()
{
    mine=new block(mine->getrow(),mine->getcol(),mine->getboomnum());
    boom_number=mine->getboomnum();
    flag_number=boom_number;
}

void Winmine::easy()
{
    mine=new block(9,9,10);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    update();
}

void Winmine::middle()
{
    mine=new block(18,18,36);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    update();
    this->move(631,63);
}

void Winmine::hard()
{
    mine=new block(24,54,144);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    update();
    this->move(0,0);
}

void Winmine::window_show()
{
    w.setFixedSize(400,400);
    w.move(700,70);
    w.setWindowTitle("自定义");
    w.show();
}

void Winmine::list_show()
{
    w2.setWindowTitle("排行榜");
    w2.move(0,0);
    w2.show();
}

void Winmine::setcustomize(int row,int col,int boomnum)
{
    mine=new block(row,col,boomnum);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    w.hide();
    update();
}

void Winmine::write_setting()
{
    qDebug()<<"写入";
    QFile file("load.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream write(&file);
    char a[100000];
    int k=0;
    write<<mine->getrow()<<'/'<<mine->getcol()<<'/'<<mine->getboomnum()<<'/'<<end<<'/'<<boom_number<<'/'<<paint_flag<<'/'<<flag_number<<'/';
    for(int i=0;i<mine->getcol();i++)
    {
        for(int j=0;j<mine->getrow();j++)
        {
            write<<mine->p[i][j]<<'=';
        }
        write<<'!';
    }/*
    for(int i=0;i<mine->getcol();i++)
    {
        for(int j=0;j<mine->getrow();j++)
        {
            if(mine->p[i][j]<10)//未显示数字
            {
                char b=mine->p[i][j]+'0';
                write<<b<<'=';
                continue;
            }
            if(mine->p[i][j]==99)//未操作地雷
            {
                write<<'a'<<'=';
                continue;
            }
            if(mine->p[i][j]==149)//插旗地雷
            {
                write<<'b'<<'=';
                continue;
            }
            if(mine->p[i][j]==199)//已显示地雷
            {
                write<<'g'<<'=';
                continue;
            }
            if(mine->p[i][j]==299)//问号地雷
            {
                write<<'c'<<'=';
                continue;
            }
            if(mine->p[i][j]>=200&&mine->p[i][j]<210)//问号数字
            {
                switch (mine->p[i][j])
                {
                    case 200:write<<'d'<<'0'<<'=';break;
                    case 201:write<<'d'<<'1'<<'=';break;
                    case 202:write<<'d'<<'2'<<'=';break;
                    case 203:write<<'d'<<'3'<<'=';break;
                    case 204:write<<'d'<<'4'<<'=';break;
                    case 205:write<<'d'<<'5'<<'=';break;
                    case 206:write<<'d'<<'6'<<'=';break;
                    case 207:write<<'d'<<'7'<<'=';break;
                    case 208:write<<'d'<<'8'<<'=';break;
                    default:break;
                }
                continue;
            }
            if(mine->p[i][j]>=50&&mine->p[i][j]<60)//插旗数字
            {
                switch (mine->p[i][j])
                {
                    case 50:write<<'e'<<'0'<<'=';break;
                    case 51:write<<'e'<<'1'<<'=';break;
                    case 52:write<<'e'<<'2'<<'=';break;
                    case 53:write<<'e'<<'3'<<'=';break;
                    case 54:write<<'e'<<'4'<<'=';break;
                    case 55:write<<'e'<<'5'<<'=';break;
                    case 56:write<<'e'<<'6'<<'=';break;
                    case 57:write<<'e'<<'7'<<'=';break;
                    case 58:write<<'e'<<'8'<<'=';break;
                    default:break;
                }
                continue;
            }
            if(mine->p[i][j]>=100&&mine->p[i][j]<110)//已显示的数字
            {
                switch (mine->p[i][j])
                {
                    case 100:write<<'f'<<'0'<<'=';break;
                    case 101:write<<'f'<<'1'<<'=';break;
                    case 102:write<<'f'<<'2'<<'=';break;
                    case 103:write<<'f'<<'3'<<'=';break;
                    case 104:write<<'f'<<'4'<<'=';break;
                    case 105:write<<'f'<<'5'<<'=';break;
                    case 106:write<<'f'<<'6'<<'=';break;
                    case 107:write<<'f'<<'7'<<'=';break;
                    case 108:write<<'f'<<'8'<<'=';break;
                    default:break;
                }
                continue;
            }
        }
        write<<'!';
    }*/
    file.close();

}

void Winmine::read_setting()
{
    QFile file("load.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream read(&file);
    if(read.atEnd())//判断是否有存档
    {
        QMessageBox::warning(this,"wrong","无存档");
        return;
    }
    QString line=read.readLine();
    qDebug()<<line;
    QString row;
    QString col;
    QString boom;
    int end1;
    QString fn;
    QString boomn;
    QString flag;
    for(int i=0;;i++)//提取前几个初始值
    {
        if(line[i]=='/')
        {
            for(int j=i+1;;j++)
            {
                if(line[j]=='/')
                {
                    for(int n=j+1;;n++)
                    {
                        if(line[n]=='/')
                        {
                            n++;
                            if(line[n]=='0')
                                end1=0;
                            if(line[n]=='1')
                                end1=1;
                            n++;
                            for(int m=n+1;;m++)
                            {
                                if(line[m]=='/')
                                {
                                    for(int k=m+1;;k++)
                                    {
                                        if(line[k]=='/')
                                        {
                                            for(int u=k+1;;u++)
                                            {
                                                if(line[u]=='/')
                                                {
                                                    break;
                                                }
                                                fn.push_back(line[u]);
                                            }
                                            break;
                                        }
                                        flag.push_back(line[k]);
                                    }
                                    break;
                                }
                                boomn.push_back(line[m]);
                            }
                            break;
                        }
                        boom.push_back(line[n]);
                    }
                    break;
                }
                col.push_back(line[j]);
            }
            break;
        }
        row.push_back(line[i]);//
    }
    int row1=row.toInt();
    int col1=col.toInt();
    int boom1=boom.toInt();
    int k=0;

    mine=new block(row1,col1,boom1);//加载初始值
    end=end1;
    boom_number=boomn.toInt();
    paint_flag=flag;
    flag_number=fn.toInt();

    for(int i=0;i<line.length();i++)
    {
        if(k<7)
        {
            if(line[i]=='/')
                k++;
            continue;
        }
        else
        {
            int r=0,c=0;
            for(int j=i;j<line.length();j++)
            {
                if(line[j]=='=')
                    continue;
                else
                if(line[j]=='!')
                {
                    c=0;
                    r++;
                    continue;
                    if(r==row1)
                    {
                        break;
                    }
                }
                else
                {
                    QString temp;
                    for(int k=j;line[j]!='='&&line[j]!='!';j++)
                    {
                        temp.push_back(line[j]);
                    }
                    int temp1=temp.toInt();
                    mine->p[r][c]=temp1;
                    c++;
                    continue;
                }/*
                if(line[j]=='a')//未操作地雷
                {
                    mine->p[r][c]=99;
                    c++;
                    continue;
                }
                else
                if(line[j]=='b')//插旗雷
                {
                    mine->p[r][c]=149;
                    c++;
                    continue;
                }
                else
                if(line[j]=='g')//点击的地雷
                {
                    mine->p[r][c]=199;
                    c++;
                    continue;
                }
                else
                if(line[j]=='c')//问号雷
                {
                    mine->p[r][c]=299;
                    c++;
                    continue;
                }
                else
                if(line[j]=='d')//问号数
                {
                    j++;
                    if(line[j]=='0')
                    {
                        mine->p[r][c]=200;
                    }
                    else if(line[j]=='1')
                    {
                        mine->p[r][c]=201;
                    }
                    else if(line[j]=='2')
                    {
                        mine->p[r][c]=202;
                    }
                    else if(line[j]=='3')
                    {
                        mine->p[r][c]=203;
                    }
                    else if(line[j]=='4')
                    {
                        mine->p[r][c]=204;
                    }
                    else if(line[j]=='5')
                    {
                        mine->p[r][c]=205;
                    }
                    else if(line[j]=='6')
                    {
                        mine->p[r][c]=206;
                    }
                    else if(line[j]=='7')
                    {
                        mine->p[r][c]=207;
                    }
                    else if(line[j]=='8')
                    {
                        mine->p[r][c]=208;
                    }
                    c++;
                    continue;
                }
                else
                if(line[j]=='e')//插旗数
                {
                    j++;
                    if(line[j]=='0')
                    {
                        mine->p[r][c]=50;
                    }
                    else if(line[j]=='1')
                    {
                        mine->p[r][c]=51;
                    }
                    else if(line[j]=='2')
                    {
                        mine->p[r][c]=52;
                    }
                    else if(line[j]=='3')
                    {
                        mine->p[r][c]=53;
                    }
                    else if(line[j]=='4')
                    {
                        mine->p[r][c]=54;
                    }
                    else if(line[j]=='5')
                    {
                        mine->p[r][c]=55;
                    }
                    else if(line[j]=='6')
                    {
                        mine->p[r][c]=56;
                    }
                    else if(line[j]=='7')
                    {
                        mine->p[r][c]=57;
                    }
                    else if(line[j]=='8')
                    {
                        mine->p[r][c]=58;
                    }
                    c++;
                    continue;
                }
                else
                if(line[j]=='f')//显示的数
                {
                    j++;
                    if(line[j]=='0')
                    {
                        mine->p[r][c]=100;
                    }
                    else if(line[j]=='1')
                    {
                        mine->p[r][c]=101;
                    }
                    else if(line[j]=='2')
                    {
                        mine->p[r][c]=102;
                    }
                    else if(line[j]=='3')
                    {
                        mine->p[r][c]=103;
                    }
                    else if(line[j]=='4')
                    {
                        mine->p[r][c]=104;
                    }
                    else if(line[j]=='5')
                    {
                        mine->p[r][c]=105;
                    }
                    else if(line[j]=='6')
                    {
                        mine->p[r][c]=106;
                    }
                    else if(line[j]=='7')
                    {
                        mine->p[r][c]=107;
                    }
                    else if(line[j]=='8')
                    {
                        mine->p[r][c]=108;
                    }
                    c++;

                    continue;
                }
                else//未显示数字
                {
                    if(line[j]=='0')
                    {
                        mine->p[r][c]=0;
                    }
                    else if(line[j]=='1')
                    {
                        mine->p[r][c]=1;
                    }
                    else if(line[j]=='2')
                    {
                        mine->p[r][c]=2;
                    }
                    else if(line[j]=='3')
                    {
                        mine->p[r][c]=3;
                    }
                    else if(line[j]=='4')
                    {
                        mine->p[r][c]=4;
                    }
                    else if(line[j]=='5')
                    {
                        mine->p[r][c]=5;
                    }
                    else if(line[j]=='6')
                    {
                        mine->p[r][c]=6;
                    }
                    else if(line[j]=='7')
                    {
                        mine->p[r][c]=7;
                    }
                    else if(line[j]=='8')
                    {
                        mine->p[r][c]=8;
                    }
                    c++;
                    continue;
                }*/
            }
            break;
        }
    }
    qDebug()<<line;
    file.close();
    update();
}

void Winmine::clearfile()
{
    QFile file("load.txt");
    file.open(QFile::WriteOnly | QFile::Truncate);
    file.close();
}

void Winmine::clearlist()
{
    QFile file("ranklist.txt");
    file.open(QFile::WriteOnly | QFile::Truncate);
    file.close();
}

Winmine::~Winmine()
{
    delete ui;
}
