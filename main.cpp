#include <QtGui/QApplication>
#include <QTextCodec>

#include "JtvLiveUiMain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*** Config ***/
    //UTF-8 support
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    //Locale
    //QLocale::setDefault(QLocale(QLocale::French, QLocale::France));
    /****************************************************/
    JtvLiveUiMain w;
    w.show();
    return a.exec();
}
