#ifndef IROOTHISTOGRAMBINNING_H
#define IROOTHISTOGRAMBINNING_H

#include <QWidget>
#include <QMouseEvent>
#include <IROOTPad.h>

namespace Ui {
class IROOTHistogramBinning;
}

typedef struct {
    double xmin;
    double xmax;
    double xbins;
    double ymin;
    double ymax;
    double ybins;
    double zmin;
    double zmax;
    double zbins;
    bool   useDefault;
} HistBin;
class IROOTHistogramBinning : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTHistogramBinning(QWidget *parent = 0);
    void UseDefault(bool d);
    HistBin GetCurrentBinning(){return currentBinning;}
    ~IROOTHistogramBinning();
    
signals:
    void binningChanged(HistBin);
private slots:
    void on_close_clicked();

    void on_useDefault_clicked(bool checked);
    void on_xmin_valueChanged(double arg1);

    void on_xmax_valueChanged(double arg1);

    void on_ymin_valueChanged(double arg1);

    void on_xbins_valueChanged(int arg1);

    void on_ymax_valueChanged(double arg1);

    void on_ybins_valueChanged(int arg1);

    void on_zmin_valueChanged(double arg1);

    void on_zmax_valueChanged(double arg1);

    void on_zbins_valueChanged(int arg1);

    void on_xBinWidth_valueChanged(double arg1);

    void on_yBinWidth_valueChanged(double arg1);

    void on_zBinWidth_valueChanged(double arg1);

    void on_xBinArrow_clicked(bool checked);

    void on_yBinArrow_clicked();

    void on_zBinArrow_clicked();

private:
    Ui::IROOTHistogramBinning *ui;
    HistBin currentBinning; //
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
};

#endif // IROOTHISTOGRAMBINNING_H
