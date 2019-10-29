#ifndef IROOTPLOTOPTIONSENTRY_H
#define IROOTPLOTOPTIONSENTRY_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class IROOTPlotOptionsEntry;
}

class IROOTPlotOptionsEntry : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTPlotOptionsEntry(QWidget *parent = 0);
    ~IROOTPlotOptionsEntry();
    
private slots:
    void on_close_clicked();

    void on_plotOptions_textChanged(const QString &arg1);
    void setText(QString arg);

signals:
    void plotOptionsChanged(QString);

private:
    Ui::IROOTPlotOptionsEntry *ui;
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
};

#endif // IROOTPLOTOPTIONSENTRY_H
