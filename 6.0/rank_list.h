#ifndef RANK_LIST_H
#define RANK_LIST_H

#include <QWidget>

namespace Ui {
class rank_list;
}

class rank_list : public QWidget
{
    Q_OBJECT

public:
    explicit rank_list(QWidget *parent = 0);
    ~rank_list();

private:
    Ui::rank_list *ui;
};

#endif // RANK_LIST_H
