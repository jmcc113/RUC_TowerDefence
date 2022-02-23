#include "maker.h"
#include "ui_maker.h"
#include <QPainter>
#include <QProgressDialog>
#include <QPushButton>
#include "start.h"

Maker::Maker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Maker)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Maker::~Maker()
{
    delete ui;
}

void Maker::paintEvent(QPaintEvent *)
{
    QPixmap background(":/image/maker.png");
    QPainter painter(this);
    painter.drawPixmap(0,0,background);
}

void Maker::on_pushButton_clicked()
{
    reject();
    start = new Start;
    start -> exec();
}
