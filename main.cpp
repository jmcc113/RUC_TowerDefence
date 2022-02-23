#include "widget.h"
#include <QApplication>
#include "start.h"
#include <QWidget>
#include <QSound>
#include "maker.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*Dialog s;
    s.show();*/
    Start md;
    md.show();
    QSound::play(":/sound/begin.wav");
    return a.exec();
}
