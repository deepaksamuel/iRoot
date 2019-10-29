#include "IROOTAxisEditor.h"
#include "ui_IROOTAxisEditor.h"

IROOTAxisEditor::IROOTAxisEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTAxisEditor)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    previousTitleOffset=0;
    previousLabelOffset=0;
    labelOffset_slider_pressed=false;
    titleOffset_slider_pressed=false;
    applyToAll                  =false;

}

IROOTAxisEditor::~IROOTAxisEditor()
{
    delete ui;
}


//void IROOTAxisEditor::on_xaxistitle_textChanged(const QString &arg1)
//{
//    IROOTPad::GetAxisHistogram();
//    IROOTPad::SetXAxisTitle(arg1);
//}

//void IROOTAxisEditor::on_yaxistitle_textChanged(const QString &arg1)
//{
//    IROOTPad::GetAxisHistogram();
//    IROOTPad::SetYAxisTitle(arg1);
//}

//void IROOTAxisEditor::on_zaxistitle_textChanged(const QString &arg1)
//{
//    IROOTPad::GetAxisHistogram();
//    IROOTPad::SetZAxisTitle(arg1);
//}

void IROOTAxisEditor::on_xaxistitle_returnPressed()
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetXAxisTitle(ui->xaxistitle->text(),applyToAll);
}

void IROOTAxisEditor::on_yaxistitle_returnPressed()
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetYAxisTitle(ui->yaxistitle->text(),applyToAll);
}
void IROOTAxisEditor::on_zaxistitle_returnPressed()
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetZAxisTitle(ui->zaxistitle->text(),applyToAll);
}


void IROOTAxisEditor::on_centreTitle_clicked(bool checked)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::CentreTitle(checked,applyToAll);
}

//void IROOTAxisEditor::on_labelOffset_sliderMoved(int position)
//{
//    qDebug()<<"slider moved";
//    //    IROOTPad::GetAxisHistogram();
//    //    IROOTPad::SetLabelOffset(position*0.001);
//    //if(!labelOffset_slider_pressed){
//    //    qDebug()<<previousLabelOffset;
//    //    ui->labelOffset->setValue(previousLabelOffset); // prevent user from scrolling
//    //    //}
//}

void IROOTAxisEditor::on_labelOffset_sliderReleased()
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetLabelOffset(ui->labelOffset->value()*0.0015,applyToAll);
    previousLabelOffset=ui->labelOffset->value();
    labelOffset_slider_pressed=false;
}


void IROOTAxisEditor::on_labelOffset_sliderPressed()
{
    labelOffset_slider_pressed=true;
}
void IROOTAxisEditor::on_labelOffset_valueChanged(int value)
{
    if(!labelOffset_slider_pressed){
        ui->labelOffset->setValue(previousLabelOffset); // prevent user from scrolling
    }
}



void IROOTAxisEditor::on_titleOffset_sliderReleased()
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTitleOffset(ui->titleOffset->value()*0.02,applyToAll);
    previousTitleOffset=ui->titleOffset->value();
    titleOffset_slider_pressed=false;
}



void IROOTAxisEditor::on_titleOffset_sliderPressed()
{
    titleOffset_slider_pressed=true;
}


void IROOTAxisEditor::on_titleOffset_valueChanged(int value)
{
    if(!titleOffset_slider_pressed){
        ui->titleOffset->setValue(previousTitleOffset); // prevent user from scrolling
    }
}
void IROOTAxisEditor::on_xmajorTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksX(ui->xmajorTicks->value(),ui->xminorTicks->value(),ui->xsemiTicks->value(),applyToAll);
}

void IROOTAxisEditor::on_xminorTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksX(ui->xmajorTicks->value(),ui->xminorTicks->value(),ui->xsemiTicks->value(),applyToAll);
}

void IROOTAxisEditor::on_xsemiTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksX(ui->xmajorTicks->value(),ui->xminorTicks->value(),ui->xsemiTicks->value());
}

void IROOTAxisEditor::on_ymajorTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksY(ui->ymajorTicks->value(),ui->yminorTicks->value(),ui->ysemiTicks->value(),applyToAll);
}

void IROOTAxisEditor::on_yminorTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksY(ui->ymajorTicks->value(),ui->yminorTicks->value(),ui->ysemiTicks->value(),applyToAll);
}

void IROOTAxisEditor::on_ysemiTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksY(ui->ymajorTicks->value(),ui->yminorTicks->value(),ui->ysemiTicks->value(),applyToAll);
}

void IROOTAxisEditor::on_zmajorTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksZ(ui->zmajorTicks->value(),ui->zminorTicks->value(),ui->zsemiTicks->value(),applyToAll);
}

void IROOTAxisEditor::on_zminorTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksZ(ui->zmajorTicks->value(),ui->zminorTicks->value(),ui->zsemiTicks->value(),applyToAll);
}

void IROOTAxisEditor::on_zsemiTicks_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetTicksZ(ui->zmajorTicks->value(),ui->zminorTicks->value(),ui->zsemiTicks->value(),applyToAll);
}


void IROOTAxisEditor::on_xMin_valueChanged(double arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetXLimits(ui->xMin->value(),ui->xMax->value(),applyToAll);
}

void IROOTAxisEditor::on_xMax_valueChanged(double arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetXLimits(ui->xMin->value(),ui->xMax->value(),applyToAll);
}

void IROOTAxisEditor::on_yMin_valueChanged(double arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetYLimits(ui->yMin->value(),ui->yMax->value(),applyToAll);
}

void IROOTAxisEditor::on_yMax_valueChanged(double arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetYLimits(ui->yMin->value(),ui->yMax->value(),applyToAll);
}
void IROOTAxisEditor::on_zMin_valueChanged(double arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetZLimits(ui->zMin->value(),ui->zMax->value(),applyToAll);
}

void IROOTAxisEditor::on_zMax_valueChanged(double arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetZLimits(ui->zMin->value(),ui->zMax->value(),applyToAll);
}

void IROOTAxisEditor::on_timeDispX_clicked(bool checked)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetXAxisAsTime(checked,ui->timeFormat->currentText(),ui->timeOffset->currentText(),applyToAll);
}
void IROOTAxisEditor::on_timeDispY_clicked(bool checked)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetYAxisAsTime(checked,ui->timeFormat->currentText(),ui->timeOffset->currentText(),applyToAll);
}

void IROOTAxisEditor::on_scinot_valueChanged(int arg1)
{
    IROOTPad::GetAxisHistogram();
    IROOTPad::SetDecimals(arg1,applyToAll);
}

void IROOTAxisEditor::setIROOTPadObject(IROOTPad* object)
{
    // IROOTPad:: = object;
}


void IROOTAxisEditor::on_close_clicked()
{
    hide();
}

void IROOTAxisEditor::on_applyToAll_clicked()
{
    applyToAll = ui->applyToAll->isChecked();
}
