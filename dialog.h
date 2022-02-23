#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class Widget;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void paintEvent(QPaintEvent *event);
    void setWaRB(Widget *a);
    void setTextFont(int f);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    Widget *w;
};

#endif // DIALOG_H
