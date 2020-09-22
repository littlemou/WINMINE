#ifndef BLOCK_H
#define BLOCK_H
#include <QTimer>
#include <ctime>
#include <stdlib.h>
#include <QDebug>

class block
{
public:
    block(int row,int col,int boomnum);
    ~block();
    int **p;
    int getrow();
    int getcol();

    int getflagnum();
    int gettimenum();
    int getboomnum();

    void subflagnum();
    void addflagnum();
    void addtimeNum();

    void subboomnum();
    void addboomnum();



private:
    int row,col,boomnum;
    int flagnum,timenum;
};

#endif // BLOCK_H
