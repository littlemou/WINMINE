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
#include <QPushButton>


#define block_length 35
#define block_height 35
#define title_begin 23
#define title_height  (title_begin+150)
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

private:
    Ui::Winmine *ui;
    QTimer *runtime;

private slots:
    void restart_clicked();


protected:

};
void showblock(int x,int y);
void paintboom(QPainter *painter);

#endif // WINMINE_H
