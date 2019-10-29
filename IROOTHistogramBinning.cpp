#include "IROOTHistogramBinning.h"
#include "ui_IROOTHistogramBinning.h"

IROOTHistogramBinning::IROOTHistogramBinning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTHistogramBinning)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    currentBinning.xmin=0;
    currentBinning.xmax=0;
    currentBinning.xbins=100;

    currentBinning.ymin=0;
    currentBinning.ymax=0;
    currentBinning.ybins=100;

    currentBinning.zmin=0;
    currentBinning.zmax=0;
    currentBinning.zbins=100;

    currentBinning.useDefault=ui->useDefault->isChecked();
    on_useDefault_clicked(ui->useDefault->isChecked());

}

IROOTHistogramBinning::~IROOTHistogramBinning()
{
    delete ui;
}

void IROOTHistogramBinning::on_close_clicked()
{
    this->hide();
}

void IROOTHistogramBinning::on_useDefault_clicked(bool checked)
{
    UseDefault(checked);
}

void IROOTHistogramBinning::UseDefault(bool d)
{
    ui->xmin->setDisabled(d);
    ui->xmax->setDisabled(d);
    ui->xbins->setDisabled(d);

    ui->ymin->setDisabled(d);
    ui->ymax->setDisabled(d);
    ui->ybins->setDisabled(d);

    ui->zmin->setDisabled(d);
    ui->zmax->setDisabled(d);
    ui->zbins->setDisabled(d);

    ui->xBinWidth->setDisabled(d);
    ui->yBinWidth->setDisabled(d);
    ui->zBinWidth->setDisabled(d);

    ui->xBinArrow->setDisabled(d);
    ui->yBinArrow->setDisabled(d);
    ui->zBinArrow->setDisabled(d);
    currentBinning.useDefault=d;
    binningChanged(currentBinning);

}

void IROOTHistogramBinning::on_xmin_valueChanged(double arg1)
{
    if(ui->xBinArrow->text()=="->"){
        double diff = ui->xmax->value()-ui->xmin->value();
        ui->xBinWidth->setValue(fabs(diff)/ui->xbins->value());
    }
    else{
        double diff = ui->xmax->value()-ui->xmin->value();
        ui->xbins->setValue(fabs(diff)/ui->xBinWidth->value());
    }
    currentBinning.xmin=arg1;
    binningChanged(currentBinning);
}

void IROOTHistogramBinning::on_xmax_valueChanged(double arg1)
{
    if(ui->xBinArrow->text()=="->"){
        double diff = ui->xmax->value()-ui->xmin->value();
        ui->xBinWidth->setValue(fabs(diff)/ui->xbins->value());
    }
    else{
        double diff = ui->xmax->value()-ui->xmin->value();
        ui->xbins->setValue(fabs(diff)/ui->xBinWidth->value());
    }
    currentBinning.xmax=arg1;
    binningChanged(currentBinning);
}
void IROOTHistogramBinning::on_xbins_valueChanged(int arg1)
{
    if(ui->xBinArrow->text()=="->"){
        double diff = ui->xmax->value()-ui->xmin->value();
        ui->xBinWidth->setValue(fabs(diff)/arg1);
    }
    currentBinning.xbins=arg1;
    binningChanged(currentBinning);
    IROOTPad::xbins = arg1;
}
void IROOTHistogramBinning::on_ymin_valueChanged(double arg1)
{
    if(ui->yBinArrow->text()=="->"){
        double diff = ui->ymax->value()-ui->ymin->value();
        ui->yBinWidth->setValue(fabs(diff)/ui->ybins->value());
    }
    else{
        double diff = ui->ymax->value()-ui->ymin->value();
        ui->ybins->setValue(fabs(diff)/ui->yBinWidth->value());
    }
    currentBinning.ymin=arg1;
    binningChanged(currentBinning);
    IROOTPad::ybins = arg1;
}



void IROOTHistogramBinning::on_ymax_valueChanged(double arg1)
{
    if(ui->yBinArrow->text()=="->"){
        double diff = ui->ymax->value()-ui->ymin->value();
        ui->yBinWidth->setValue(fabs(diff)/ui->ybins->value());
    }
    else{
        double diff = ui->ymax->value()-ui->ymin->value();
        ui->ybins->setValue(fabs(diff)/ui->yBinWidth->value());
    }
    currentBinning.ymax=arg1;
    binningChanged(currentBinning);
}

void IROOTHistogramBinning::on_ybins_valueChanged(int arg1)
{
    if(ui->yBinArrow->text()=="->"){
        double diff = ui->ymax->value()-ui->ymin->value();
        ui->yBinWidth->setValue(fabs(diff)/arg1);
    }
    currentBinning.ybins=arg1;
    binningChanged(currentBinning);
}

void IROOTHistogramBinning::on_zmin_valueChanged(double arg1)
{
    if(ui->zBinArrow->text()=="->"){
        double diff = ui->zmax->value()-ui->zmin->value();
        ui->zBinWidth->setValue(fabs(diff)/ui->zbins->value());
    }
    else{
        double diff = ui->zmax->value()-ui->zmin->value();
        ui->zbins->setValue(fabs(diff)/ui->zBinWidth->value());
    }
    currentBinning.zmin=arg1;
    binningChanged(currentBinning);
}

void IROOTHistogramBinning::on_zmax_valueChanged(double arg1)
{
    if(ui->zBinArrow->text()=="->"){
        double diff = ui->zmax->value()-ui->zmin->value();
        ui->zBinWidth->setValue(fabs(diff)/ui->zbins->value());
    }
    else{
        double diff = ui->zmax->value()-ui->zmin->value();
        ui->zbins->setValue(fabs(diff)/ui->zBinWidth->value());
    }
    currentBinning.zmax=arg1;
    binningChanged(currentBinning);
}

void IROOTHistogramBinning::on_zbins_valueChanged(int arg1)
{
    if(ui->zBinArrow->text()=="->"){
        double diff = ui->zmax->value()-ui->zmin->value();
        ui->zBinWidth->setValue(fabs(diff)/arg1);
    }
    currentBinning.zbins=arg1;
    binningChanged(currentBinning);
    IROOTPad::xbins = arg1;
}

void IROOTHistogramBinning::on_xBinWidth_valueChanged(double arg1)
{
    if(ui->xBinArrow->text()=="<-"){
        double diff = ui->xmax->value()-ui->xmin->value();
        ui->xbins->setValue(fabs(diff)/arg1);
    }
}

void IROOTHistogramBinning::on_yBinWidth_valueChanged(double arg1)
{
    if(ui->yBinArrow->text()=="<-"){
        double diff = ui->ymax->value()-ui->ymin->value();
        ui->ybins->setValue(fabs(diff)/arg1);
    }
}

void IROOTHistogramBinning::on_zBinWidth_valueChanged(double arg1)
{
    if( ui->zBinArrow->text()=="<-"){
        double diff = ui->zmax->value()-ui->zmin->value();
        ui->zbins->setValue(fabs(diff)/arg1);
    }
}

void IROOTHistogramBinning::on_xBinArrow_clicked(bool checked)
{
    if(ui->xBinArrow->text()=="->"){
        ui->xBinArrow->setText("<-");
        ui->xbins->setDisabled(true);
        ui->xBinWidth->setEnabled(true);
    }
    else{
        ui->xBinArrow->setText("->");
        ui->xbins->setDisabled(false);
        ui->xBinWidth->setEnabled(false);
    }
}

void IROOTHistogramBinning::on_yBinArrow_clicked()
{
    if(ui->yBinArrow->text()=="->"){
        ui->yBinArrow->setText("<-");
        ui->ybins->setDisabled(true);
        ui->yBinWidth->setEnabled(true);
    }
    else{
        ui->yBinArrow->setText("->");
        ui->ybins->setDisabled(false);
        ui->yBinWidth->setEnabled(false);
    }
}

void IROOTHistogramBinning::on_zBinArrow_clicked()
{
    if(ui->zBinArrow->text()=="->"){
        ui->zBinArrow->setText("<-");
        ui->zbins->setDisabled(true);
        ui->zBinWidth->setEnabled(true);
    }
    else{
        ui->zBinArrow->setText("->");
        ui->zbins->setDisabled(false);
        ui->zBinWidth->setEnabled(false);
    }
}
