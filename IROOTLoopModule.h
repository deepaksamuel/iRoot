#ifndef IROOTLOOPMODULE_H
#define IROOTLOOPMODULE_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class IROOTLoopModule;
}

class IROOTLoopModule : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTLoopModule(QWidget *parent = 0);
    int GetLoopType();
    int GetLoopBegin();
    int GetLoopEnd();
    int GetLoopIncrement();
    int GetPadIncrement();
    ~IROOTLoopModule();
    
private slots:


    void on_close_clicked();

private:
    Ui::IROOTLoopModule *ui;
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
};

#endif // IROOTLOOPMODULE_H
