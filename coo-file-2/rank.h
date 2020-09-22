#ifndef RANK_H
#define RANK_H

#include <QMainWindow>
#include <QFile>
#include "dialog.h"
namespace Ui {
class rank;
}
struct idinfo{
    QString playername;
    int time;
};
class rank : public QMainWindow
{
    Q_OBJECT

public:
    explicit rank(QWidget *parent = nullptr);


    void setplayername(QString s);

    void settime(int sj);
    void px();
    void writelist();
    void readlist();


    ~rank();

private:
    Ui::rank *ui;
    dialog w;
};

#endif // RANK_H
