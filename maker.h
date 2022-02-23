#ifndef MAKER_H
#define MAKER_H

#include <QDialog>
#include "widget.h"

class Start;

namespace Ui {
class Maker;
}

class Maker : public QDialog
{
    Q_OBJECT

public:
    explicit Maker(QWidget *parent = nullptr);
    ~Maker();
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Maker *ui;
    Start *start;
};
#endif // MAKER_H
