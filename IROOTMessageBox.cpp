#include "IROOTMessageBox.h"
#include "ui_IROOTMessageBox.h"

IROOTMessageBox::IROOTMessageBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTMessageBox)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    hide();
    hideTimer = new QTimer();
    connect(hideTimer,SIGNAL(timeout()),this,SLOT(hideMessage()));
    hideTimer->setSingleShot(true);
    hideTimer->setInterval(4000);
    //setWindowFlags(Qt::Dialog);
}

IROOTMessageBox::~IROOTMessageBox()
{
    delete ui;
}

void IROOTMessageBox::ShowMessage(QString message, QString type,QWidget *w)
{
    //qApp->beep();
    raise();
    move(w->mapToGlobal( QPoint( 0, 0 ) ));
    resize(w->width(),height());
    show();
    ui->message->setText(message);
    hideTimer->start();
    show();
    raise();
}

//void IROOTMessageBox::Append(QString message,QString type, int x, int y)
//{
//    qApp->beep();
//    move(x,y+500);
//    show();
//    ui->message->setText(message);
//    if(type=="ERR"){
//        //    ui->messageIcon->setIcon(QIcon(":/icons/icons/windows8_icons/System/error.png");
//        setStyleSheet("background-color: green");
//    }
//    else if(type=="INFO"){
//        //    ui->messageIcon->setIcon(QIcon(":/icons/icons/windows8_icons/System/error.png");
//        setStyleSheet("background-color: orange");
//    }
//    hideTimer->start();
//}

void IROOTMessageBox::ShowMessages(int milliseconds)
{
    //    qApp->beep();
    //    show();
}

void IROOTMessageBox::hideMessage()
{
    hide();
}

void IROOTMessageBox::on_close_clicked()
{
    hide();
}
