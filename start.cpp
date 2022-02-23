#include "start.h"
#include "ui_start.h"
#include <QPainter>
#include <QProgressDialog>
#include <QPushButton>
#include "maker.h"

Start::Start(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);
    make = new Maker;
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Start::~Start()
{
    delete ui;
}


void Start::paintEvent(QPaintEvent *)
{
    QPixmap background(":/image/backgroundnew.png");
    QPainter painter(this);
    painter.drawPixmap(0,0,background);
}

void Start::on_pushButton_clicked()
{
     QProgressDialog pd(tr("Loading"),tr("Cancle"),0,50000,this);
        pd.setWindowTitle(tr("Game"));
        pd.setWindowModality(Qt::WindowModal);
        pd.show();
        for(int i = 0;i < 50000;++i)
        {
            pd.setValue(i);
            QCoreApplication::processEvents();
            if(pd.wasCanceled()) break;
        }
        pd.setValue(50000);
        accept();
        widget = new Widget;
        widget->show();
}
void Start::on_pushButton_2_clicked()
{
    reject();
    make->exec();
}

