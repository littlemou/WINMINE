#include "dialog.h"
#include "ui_dialog.h"

dialog::dialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dialog)
{
    ui->setupUi(this);
    connect(ui->cancel,&QPushButton::clicked,this,&dialog::cancelclicked);
    connect(ui->ok,&QPushButton::clicked,this,&dialog::okclicked);
}
void dialog::cancelclicked(){
    close();
}
void dialog::okclicked(){

    QString s=ui->lineEdit->text();
    emit slotname(s);
    close();
}
dialog::~dialog()
{
    delete ui;
}
