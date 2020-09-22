#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>
#include <QPushButton>
#include "winmine.h"

namespace Ui {
class welcome;
}

class welcome : public QMainWindow
{
    Q_OBJECT

public:
    explicit welcome(QWidget *parent = nullptr);
    void enterin();
    ~welcome();

private:
    Ui::welcome *ui;
    Winmine w;
};

#endif // WELCOME_H
