#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include "widget.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setTextFont(15);

    //    this->setWindowFlags(Qt::CustomizeWindowHint);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.drawPixmap(45,50,QPixmap(":/image/BTNTran.jpg"));
    painter.drawPixmap(165,50,QPixmap(":/image/BTNInnerFire.png"));
    painter.drawPixmap(285,50,QPixmap(":/image/BTNStun.png"));

}

void Dialog::setWaRB(Widget *a)
{
    w=a;
    if(w->m_playerHp<20)
    {
        ui->radioButton->setEnabled(false);
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
    }
}

void Dialog::on_pushButton_clicked()
{
    if(ui->radioButton->isChecked())
        w->m_skill=1;
    else if(ui->radioButton_2->isChecked())
        w->m_skill=2;
    else if(ui->radioButton_3->isChecked())
        w->m_skill=3;
    accept();
}

void Dialog::setTextFont(int f)
{
    QFont font("宋体");
    font.setPixelSize(f);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
    ui->radioButton->setFont(font);
    ui->radioButton_2->setFont(font);
    ui->radioButton_3->setFont(font);
    ui->pushButton->setFont(font);
    ui->pushButton_2->setFont(font);
}
