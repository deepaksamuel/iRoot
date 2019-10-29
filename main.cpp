#include <QApplication>
#include "IROOT.h"
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    //system("setupvs2010.bat"); // set up variable for 2010 compiler
    QApplication a(argc, argv);
    QPixmap pixmap(":/icons/icons/others/splash.png");
    QSplashScreen splash(pixmap,Qt::SplashScreen|Qt::WindowStaysOnTopHint);
    //splash.setWindowFlags(QtFl);
    splash.show();
    IROOT w;

    w.show();
    
    return a.exec();
}
