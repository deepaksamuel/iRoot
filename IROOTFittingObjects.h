#ifndef IROOTFITTINGOBJECTS_H
#define IROOTFITTINGOBJECTS_H

#include <QWidget>
#include <TPad.h>
#include <IROOTPad.h>
#include <QString>
#include <QTreeWidgetItem>
#include <QMouseEvent>

namespace Ui {
class IROOTFittingObjects;
}

class IROOTFittingObjects : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTFittingObjects(QWidget *parent = 0);
    void Update();
    QString GetObjectId(QString s);
    QStringList GetListofSelectedObjects();
    ~IROOTFittingObjects();
signals:
    void itemsSelected(QStringList);
    void highLight(QString);
private slots:
    void on_close_clicked();

    void on_fittableObjects_itemClicked(QTreeWidgetItem *item, int column);



private:
    Ui::IROOTFittingObjects *ui;
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
};

#endif // IROOTFITTINGOBJECTS_H
