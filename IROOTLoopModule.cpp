#include "IROOTLoopModule.h"
#include "ui_IROOTLoopModule.h"
#include <QDebug>
IROOTLoopModule::IROOTLoopModule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTLoopModule)
{

    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

IROOTLoopModule::~IROOTLoopModule()
{
    delete ui;
}


void IROOTLoopModule::on_close_clicked()
{
    hide();
}

int IROOTLoopModule::GetLoopType()
{
    if(ui->loopoverIndex->isChecked())
        return 1;
    else if(ui->loopoverFiles->isChecked())
        return 2;
    else if(ui->loopoverFilesandIndex->isChecked())
        return 3;
    else if(ui->loopoverIndexandFiles->isChecked())
        return 4;

}

int IROOTLoopModule::GetLoopBegin()
{
    return ui->loopBegin->value();
}

int IROOTLoopModule::GetLoopEnd()
{
    return ui->loopEnd->value();
}

int IROOTLoopModule::GetLoopIncrement()
{
    return ui->loopIncrement->value();
}

int IROOTLoopModule::GetPadIncrement()
{
    return ui->padChangeOver->value();
}
