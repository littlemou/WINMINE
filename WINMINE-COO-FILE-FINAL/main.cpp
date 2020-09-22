#include "welcome.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    welcome w;
    w.show();
    QDesktopWidget *desk = QApplication::desktop();
    QRect deskRect = desk->availableGeometry();
    w.move((deskRect.width()-w.width())/2, (deskRect.height()-w.height())/2);
    return a.exec();
}
