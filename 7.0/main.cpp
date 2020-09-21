#include "winmine.h"
#include <QApplication>
#include <QPalette>
#include <QPainter>
#include <QScrollArea>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Winmine w;
    QPalette pal=w.palette();
    pal.setColor(QPalette::Background,Qt::black);

    w.show();
    return a.exec();
}
