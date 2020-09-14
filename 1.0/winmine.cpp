#include "winmine.h"
#include "ui_winmine.h"

int initial_row=9;
int initial_col=9;
int initial_boomnum=10;
int click_x,click_y;

QPainter *paintmywindow;
block *mine=new block(initial_row,initial_col,initial_boomnum);
int length=mine->getcol()*35;
int height=mine->getrow()*35+title_height;


//炸弹为99
//插旗+50，拔旗-50
//遍历显示+100
Winmine::Winmine(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Winmine)
{
    ui->setupUi(this);

}
void Winmine::mousePressEvent(QMouseEvent *event)
{
    click_x=event->x();
    click_y=event->y();
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

            showblock(click_y,click_x);
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
            mine->p[click_y][click_x]=mine->p[click_y][click_x]+50;
            if(mine->p[click_y][click_x]==149)
            {
                mine->subboomnum();
            }
            if(mine->getboomnum()==0)//判断输赢
            {
                QMessageBox::warning(this,"YOU WIN","YOU WIN");
            }
            mine->subflagnum();//插旗后总旗数减少
        }
        else if((mine->p[click_y][click_x]>=50&&mine->p[click_y][click_x]<60)||(mine->p[click_y][click_x]>110))//判断插旗方块，表示去掉旗
        {
            mine->p[click_y][click_x]=mine->p[click_y][click_x]-50;
            if(mine->p[click_y][click_x]==99)
            {
                mine->addboomnum();
            }
            mine->addflagnum();//拔旗后总旗数增多
        }
        update();
        paint_flag="flag";
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
}

void Winmine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    paintmywindow=&painter;

    QPixmap pixmap("://images/initial.png");
    if(paint_flag=="none")//画出初始界面
    {
        for(int i=0;i<mine->getrow()*block_length;i+=block_length)
        {
            for(int j=title_height;j<mine->getcol()*block_height;j+=block_height)
            {
                painter.drawPixmap(i,j,pixmap,0,0,block_length,block_length);
            }
        }
    }
    else
    if(paint_flag=="boom")
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

        for(int i=0;i<mine->getrow();i++)//重新绘制雷区
        {
            for(int j=0;j<mine->getcol();j++)
            {
                if((mine->p[i][j]<10))
                {
                    painter.drawPixmap(j*block_length,i*block_height+title_height,initial,0,0,block_length,block_height);
                }
                else if((mine->p[i][j]==99))
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

Winmine::~Winmine()
{
    delete ui;
}
