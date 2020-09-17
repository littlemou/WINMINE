#include "winmine.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Winmine w;

    w.show();
    return a.exec();
}
