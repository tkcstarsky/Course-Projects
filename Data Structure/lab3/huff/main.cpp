#include "hfcom.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    hfcom w;
    w.show();

    return a.exec();
}
