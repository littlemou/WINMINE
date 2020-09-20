#include "rank_list.h"
#include "ui_rank_list.h"

rank_list::rank_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rank_list)
{
    ui->setupUi(this);
}

rank_list::~rank_list()
{
    delete ui;
}
