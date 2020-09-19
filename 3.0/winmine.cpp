#include "winmine.h"
#include "ui_winmine.h"
#include <iostream>
#include<QTextStream>
#include <QGraphicsWidget>
#include <QPropertyAnimation>

int initial_row=9;
int initial_col=9;
int initial_boomnum=10;
int click_x,click_y;

QPainter *paintmywindow;
block *mine=new block(initial_row,initial_col,initial_boomnum);
int length=mine->getcol()*35;
int height=mine->getrow()*35+title_height;
int boom_number=mine->getboomnum();
int flag_number=boom_number;
//炸弹为99
//插旗+50，拔旗-50
//遍历显示+100

Winmine::Winmine(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Winmine)
{
    ui->setupUi(this);
    qDebug()<<mine->getcol()<<" "<<mine->getrow();
    connect(ui->Easy,&QAction::triggered,this,&Winmine::easy);
    connect(ui->Middle,&QAction::triggered,this,&Winmine::middle);
    connect(ui->Hard,&QAction::triggered,this,&Winmine::hard);
    connect(ui->Customize,&QAction::triggered,this,&Winmine::window_show);

    void(customize_window::*funslot)(int row,int col,int boomnum)=&customize_window::slot;
    connect(&w,funslot,this,&Winmine::setcustomize);

    connect(ui->View_Information,&QAction::triggered,this,&Winmine::list_show);
}

void Winmine::mousePressEvent(QMouseEvent *event)
{
    click_x=event->x();
    click_y=event->y();
    if(click_x>block_length*mine->getrow()/2-block_length/2&&click_x<block_length*mine->getrow()/2-block_length/2+block_length
       &&click_y>title_begin+title_height/3&&click_y<title_begin+title_height/3+block_height)
    {
        restart_clicked();
        paint_flag="none";
        end=0;
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
        if(mine->p[click_y][click_x]==99)
        {
            paint_flag="boom";
            mine->p[click_y][click_x]=199;
            update();
            end=1;
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
            update();
            qDebug()<<"结束检索";
            paint_flag="number";
            if(end)
            {
                if(showimage)
                {
                    QMessageBox::warning(this,"END","YOU WIN");
                    showimage=!showimage;
                }
                else
                {
                    QMessageBox::warning(this,"END","YOU WIN");
                    showimage=!showimage;
                }
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
            if(mine->p[click_y][click_x]==149)
            {
                boom_number--;
            }
            if(boom_number==0)
            {
                QMessageBox::warning(this,"YOU WIN","YOU WIN");
            }
        }
        else if((mine->p[click_y][click_x]>=50&&mine->p[click_y][click_x]<60)||(mine->p[click_y][click_x]==149))//判断插旗方块，表示去掉旗
        {
            mine->p[click_y][click_x]=mine->p[click_y][click_x]-50;
            flag_number++;
            if(mine->p[click_y][click_x]==99)
            {
                boom_number++;
            }
        }
        update();
        paint_flag="flag";
//        if(end)
//        {
//            if(showimage)
//             {
//                QMessageBox::warning(this,"END","YOU WIN");
//                showimage=!showimage;
//             }
//            else
//            {
//                QMessageBox::warning(this,"END","YOU WIN");
//                showimage=!showimage;
//            }
//        }
    }
}

void Winmine::paintEvent(QPaintEvent *event)
{
    this->setFixedSize(mine->getrow()*block_length,mine->getcol()*block_height+title_height);
//    ui->horizontalScrollBar->setMaximumSize(block_length*mine->getcol(),35);//水平滑条
//    ui->horizontalScrollBar->setMinimumSize(block_length*mine->getcol(),35);
//    ui->verticalScrollBar->setMinimumSize(35,block_height*mine->getrow()+title_height);//竖直滑条
//    ui->verticalScrollBar->setMaximumSize(35,block_height*mine->getrow()+title_height);
//    ui->horizontalScrollBar->move(0,block_height*(mine->getrow()-1)+5+title_height);
//    ui->verticalScrollBar->move(block_length*mine->getcol(),0);
    qDebug()<<block_length*mine->getcol();
    QPainter painter(this);
    paintmywindow=&painter;
    painttitle(paintmywindow);
    QPixmap initial("://images/initial.png");
    if(paint_flag=="none")//画出初始界面
    {
        qDebug()<<"画出初始界面";
        for(int i=0;i<mine->getrow()*block_length;i+=block_length)
        {
            for(int j=title_height;j<mine->getcol()*block_height+title_height;j+=block_height)
            {
                qDebug()<<j;
                painter.drawPixmap(i,j,initial,0,0,block_length,block_length);
            }
        }

    }
    else
    if(paint_flag=="boom")
    {
        qDebug()<<"画出结束界面";
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
                else if(mine->p[i][j]>110)
                {
                    painter.drawPixmap(j*block_length,i*block_height+title_height,flag,0,0,block_length,block_height);
                }
                else if((mine->p[i][j]<60)&&(mine->p[i][j]>=50))
                {
                    painter.drawPixmap(j*block_length,i*block_height+title_height,flag,0,0,block_length,block_height);
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
}

void showblock(int x, int y)
{
    qDebug()<<"进入检索";
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
    QPixmap unknow("://images/unknow.png");
    painter->drawPixmap(block_length*mine->getrow()/2-block_length/2,title_begin+title_height/3,question,0,0,block_length,block_height);

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
            if(mine->p[i][j]>99&&mine->p[i][j]<110)
            {
                int num=mine->p[i][j]-100;
                if(num==0)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blank,0,0,block_length,block_height);
                }
                else
                if(num==1)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blk_1,0,0,block_length,block_height);
                }
                else
                if(num==2)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blk_2,0,0,block_length,block_height);
                }
                else
                if(num==3)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blk_3,0,0,block_length,block_height);
                }
                else
                if(num==4)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blk_4,0,0,block_length,block_height);
                }
                else
                if(num==5)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blk_5,0,0,block_length,block_height);
                }
                else
                if(num==6)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blk_6,0,0,block_length,block_height);
                }
                else
                if(num==7)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blk_7,0,0,block_length,block_height);
                }
                else
                if(num==8)
                {
                    painter->drawPixmap(j*block_length,i*block_height+title_height,blk_8,0,0,block_length,block_height);
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
    qDebug()<<this->x()<<" "<<this->y();
}

void Winmine::middle()
{
    mine=new block(18,18,35);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    update();
    this->move(631,63);
    qDebug()<<this->x()<<" "<<this->y();
}

void Winmine::hard()
{
    mine=new block(54,24,70);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    this->move(1,1);
    update();
    qDebug()<<this->x()<<" "<<this->y();
}

void Winmine::window_show()
{
    w.setFixedSize(400,400);
    w.move(block_length*mine->getcol()/2-90,block_height+title_height/2-50);
    w.setWindowTitle("自定义");
//    w.move(1100,150);
    w.show();
}

void Winmine::list_show()
{
    w2.setWindowTitle("排行榜");
    w2.move(0,0);

}

void Winmine::setcustomize(int row,int col,int boomnum)
{
    mine=new block(row,col,boomnum);
    paint_flag="none";
    end=0;
    w.hide();
    update();
}

Winmine::~Winmine()
{
    delete ui;
}
