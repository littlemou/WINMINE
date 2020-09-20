#include "enter.h"
#include "ui_enter.h"

enter::enter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::enter)
{
    ui->setupUi(this);
}

enter::~enter()
{
    delete ui;
}
