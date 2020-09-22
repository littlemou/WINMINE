#ifndef DIALOG_H
#define DIALOG_H

#include <QMainWindow>
#include <QPushButton>


namespace Ui {
class dialog;
}

class dialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit dialog(QWidget *parent = nullptr);
    void okclicked();
    void cancelclicked();
    ~dialog();

private:
    Ui::dialog *ui;
signals:
    void slotname(QString s);
};

#endif // DIALOG_H
