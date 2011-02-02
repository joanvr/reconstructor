#include <QtGui/QApplication>
#include "widget.h"
#include "Reconstructor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
