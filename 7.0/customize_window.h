#ifndef CUSTOMIZE_WINDOW_H
#define CUSTOMIZE_WINDOW_H

#include <QWidget>

namespace Ui {
class customize_window;
}

class customize_window : public QWidget
{
    Q_OBJECT

public:
    explicit customize_window(QWidget *parent = 0);
    ~customize_window();

private slots:
    void senddata();

private:
    Ui::customize_window *ui;

signals:
    void slot(int row,int col,int boomnum);

};

#endif // CUSTOMIZE_WINDOW_H
