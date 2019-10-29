#ifndef IROOTAXISEDITOR_H
#define IROOTAXISEDITOR_H

#include <QWidget>
#include <IROOTPad.h>
#include <QMenu>
#include <QDebug>
#include <QMouseEvent>

namespace Ui {
class IROOTAxisEditor;
}

class IROOTAxisEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTAxisEditor(QWidget *parent = 0);
    void setIROOTPadObject(IROOTPad *object);
    ~IROOTAxisEditor();
    
private slots:

//    void on_xaxistitle_textChanged(const QString &arg1);

//    void on_yaxistitle_textChanged(const QString &arg1);

//    void on_zaxistitle_textChanged(const QString &arg1);

    void on_centreTitle_clicked(bool checked);



    void on_xmajorTicks_valueChanged(int arg1);
    void on_xminorTicks_valueChanged(int arg1);
    void on_xsemiTicks_valueChanged(int arg1);
    void on_ymajorTicks_valueChanged(int arg1);
    void on_yminorTicks_valueChanged(int arg1);
    void on_ysemiTicks_valueChanged(int arg1);
    void on_zmajorTicks_valueChanged(int arg1);
    void on_zminorTicks_valueChanged(int arg1);
    void on_zsemiTicks_valueChanged(int arg1);

    void on_xMin_valueChanged(double arg1);
    void on_xMax_valueChanged(double arg1);
    void on_yMin_valueChanged(double arg1);
    void on_yMax_valueChanged(double arg1);
    void on_zMin_valueChanged(double arg1);
    void on_zMax_valueChanged(double arg1);

    void on_timeDispX_clicked(bool checked);
    void on_timeDispY_clicked(bool checked);

    void on_scinot_valueChanged(int arg1);

    void on_xaxistitle_returnPressed();
    void on_yaxistitle_returnPressed();
    void on_zaxistitle_returnPressed();

    void on_labelOffset_sliderReleased();
    void on_titleOffset_sliderReleased();

    void on_labelOffset_sliderPressed();
    void on_titleOffset_sliderPressed();

    void on_labelOffset_valueChanged(int value);
    void on_titleOffset_valueChanged(int value);



    void on_close_clicked();

    void on_applyToAll_clicked();

private:
    Ui::IROOTAxisEditor *ui;
    IROOTPad            *iPad;
    int                 previousLabelOffset;
    int                 previousTitleOffset;
    bool                labelOffset_slider_pressed;
    bool                titleOffset_slider_pressed;
    bool                applyToAll;
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
};

#endif // IROOTAXISEDITOR_H
