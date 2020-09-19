#include "customize_window.h"
#include "ui_customize_window.h"
#include <QMessageBox>

customize_window::customize_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::customize_window)
{
    ui->setupUi(this);
    connect(ui->ok,&QPushButton::clicked,this,&customize_window::senddata);
}

void customize_window::senddata()
{
    QString str1=ui->lineEdit1->text();
    QString str2=ui->lineEdit2->text();
    QString str3=ui->lineEdit3->text();
    if(str1.isEmpty()||str2.isEmpty()||str3.isEmpty())
    {
        QMessageBox::warning(this,"wrong","请正确输入所有数值");
        return;
    }
    emit slot(ui->lineEdit1->text().toInt(),ui->lineEdit2->text().toInt(),ui->lineEdit3->text().toInt());
}
customize_window::~customize_window()
{
    delete ui;
}
