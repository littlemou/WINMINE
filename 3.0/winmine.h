#ifndef WINMINE_H
#define WINMINE_H

#include <QMainWindow>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QString>
#include <QApplication>
#include <ctime>
#include <QTimer>
#include <QPushButton>
#include "block.h"
#include "customize_window.h"
#include "rank_list.h"

#define block_length 35
#define block_height 35
#define title_begin 23
#define title_height  (title_begin+100)
namespace Ui {
class Winmine;
}

class Winmine : public QMainWindow
{
    Q_OBJECT

public:
    explicit Winmine(QWidget *parent = 0);
    ~Winmine();
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    QString paint_flag="none";
    int end=0;
    bool showimage=1;
    void painttitle(QPainter *painter);
    void easy();
    void middle();
    void hard();
    void setcustomize(int row,int col,int boomnum);

private:
    Ui::Winmine *ui;
    QTimer *runtime;
    customize_window w;
    rank_list w2;//排名

private slots:
    void restart_clicked();
    void window_show();
    void list_show();


protected:

};
void showblock(int x,int y);
void paintboom(QPainter *painter);

#endif // WINMINE_H
