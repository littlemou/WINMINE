#include "customize_window.h"
#include "ui_customize_window.h"
#include <QMessageBox>

customize_window::customize_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::customize_window)
{
    ui->setupUi(this);
    setFixedSize(300,350);
    connect(ui->ok,&QPushButton::clicked,this,&customize_window::senddata);
}

void customize_window::senddata()
{
    QString str1=ui->lineEdit1->text();
    QString str2=ui->lineEdit2->text();
    QString str3=ui->lineEdit3->text();
    if(str1.isEmpty()||str2.isEmpty()||str3.isEmpty())
    {
        QMessageBox::warning(this,"wrong","请输入所有数值");
        return;
    }
    if(str3.toInt()<0||str3.toInt()>999)
    {
        QMessageBox::warning(this,"wrong","超出范围");
        return;
    }
    if(str1.toInt()<2||str1.toInt()>24)
    {
        QMessageBox::warning(this,"wrong","超出范围");
        return;
    }
    if(str2.toInt()<2||str2.toInt()>54)
    {
        QMessageBox::warning(this,"wrong","超出范围");
        return;
    }
    emit slot(str1.toInt(),str2.toInt(),str3.toInt());
}
customize_window::~customize_window()
{
    delete ui;
}
