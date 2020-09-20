#ifndef ENTER_H
#define ENTER_H

#include <QWidget>

namespace Ui {
class enter;
}

class enter : public QWidget
{
    Q_OBJECT

public:
    explicit enter(QWidget *parent = 0);
    ~enter();

private:
    Ui::enter *ui;
};

#endif // ENTER_H
