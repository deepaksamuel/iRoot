#ifndef IROOTFITOPTIONS_H
#define IROOTFITOPTIONS_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QMouseEvent>

namespace Ui {
class IROOTFitOptions;
}

class IROOTFitOptions : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTFitOptions(QWidget *parent = 0);
    ~IROOTFitOptions();
    
private slots:

    void on_fitOptions_itemClicked(QTreeWidgetItem *item, int column);

    void on_close_clicked();

signals:
    void selectionChanged(QList <QTreeWidgetItem*> );

private:
    Ui::IROOTFitOptions *ui;
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
};

#endif // IROOTFITOPTIONS_H
