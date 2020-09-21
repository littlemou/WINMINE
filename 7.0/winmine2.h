#ifndef WINMINE2_H
#define WINMINE2_H

#include <QWidget>

namespace Ui {
class winmine2;
}

class winmine2 : public QWidget
{
    Q_OBJECT

public:
    explicit winmine2(QWidget *parent = 0);
    ~winmine2();

private:
    Ui::winmine2 *ui;
};

#endif // WINMINE2_H
