#include "welcome.h"
#include "ui_welcome.h"

welcome::welcome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::welcome)
{
    ui->setupUi(this);
    setFixedSize(900,1000);
    connect(ui->enter,&QPushButton::clicked,this,&welcome::enterin);
}
void welcome::enterin(){
    hide();
    w.show();
}
welcome::~welcome()
{
    delete ui;
}
