#ifndef START_H
#define START_H

#include <QDialog>
#include "widget.h"
#include "maker.h"

namespace Ui {
class Start;
}

class Start : public QDialog
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = nullptr);
    ~Start();
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Start *ui;
    Maker *make;
    Widget *widget;
};

#endif // START_H
