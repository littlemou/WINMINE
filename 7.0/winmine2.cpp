#include "winmine2.h"
#include "ui_winmine2.h"

winmine2::winmine2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::winmine2)
{
    ui->setupUi(this);
}

winmine2::~winmine2()
{
    delete ui;
}
