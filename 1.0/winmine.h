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
#include "block.h"


#define block_length 35
#define block_height 35
#define element_length 20
#define expression_length 24
#define expression_height 24
#define title_begin 23
#define title_height  (title_begin+40)
#define primary_length  189
#define middle_length   336
#define top_length      630
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
private:
    Ui::Winmine *ui;
    QTimer *runtime;

private slots:


protected:

};
void showblock(int x,int y);
void paintboom(QPainter *painter);

#endif // WINMINE_H
