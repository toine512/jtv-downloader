#include <QtGui/QApplication>
#include "JtvLiveUiMain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JtvLiveUiMain w;
    w.show();

    return a.exec();
}
