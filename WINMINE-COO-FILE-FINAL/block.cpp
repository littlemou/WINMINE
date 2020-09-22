#include "block.h"

block::block(int row,int col,int boomnum)
{
    p=new int* [row];//创建数组
    for(int i=0;i<row;i++)
    {
        p[i]=new int[col];
    }
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            p[i][j]=0;
        }
    }
    srand((unsigned)time(0));
    for(int i=0;i<boomnum;i++)//埋放地雷
    {
        int rowseed=rand()%(row-1);
        int colseed=rand()%(col-1);
        if(p[rowseed][colseed]==99)
        {
            i--;
            continue;
        }
        else
        {
            p[rowseed][colseed]=99;
        }
    }
    for(int i=0;i<row;i++)//设置各个方块的周边雷数
    {
        for(int j=0;j<col;j++)
        {
            if(p[i][j]!=99)
            {
            int blockBoomNum=0;
            for(int blockRow=i-1;blockRow<=i+1;blockRow++)
            {
                for(int blockCol=j-1;blockCol<=j+1;blockCol++)
                {
                    if(blockRow<0||blockCol<0||blockRow>=row||blockCol>=col)//判断是否出界
                    {
                        continue;
                    }
                    if(p[blockRow][blockCol]==99)
                    {
                        blockBoomNum++;
                    }
                }
            }
            p[i][j]=blockBoomNum;//设置数字
            }
        }
    }
    this->row=row;
    this->col=col;
    this->boomnum=boomnum;
    this->flagnum=boomnum;
    this->timenum=0;

}
block::~block()
{
   for(int i =0;i<row;i++)
   {
    delete[]  p[i];
   }
   delete [] p;
}
int block::getrow()
{
    return row;
}
int block::getcol()
{
    return col;
}
int block::getflagnum()
{
    return flagnum;
}
int block::gettimenum()
{
    return timenum;
}
void block::settimenum(int timex){
    timenum=timex;
}
int block::getboomnum()
{
    return boomnum;
}
void block::subflagnum()
{
    flagnum=flagnum-1;
}
void block::addflagnum()
{
    flagnum=flagnum+1;
}
void block::addtimeNum()
{
    timenum=timenum+1;
}
void block::subboomnum()
{
    boomnum=boomnum-1;
}void block::addboomnum()
{
    boomnum=boomnum+1;
}
