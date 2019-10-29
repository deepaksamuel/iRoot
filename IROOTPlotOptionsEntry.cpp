#include "IROOTPlotOptionsEntry.h"
#include "ui_IROOTPlotOptionsEntry.h"

IROOTPlotOptionsEntry::IROOTPlotOptionsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTPlotOptionsEntry)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

IROOTPlotOptionsEntry::~IROOTPlotOptionsEntry()
{
    delete ui;
}

void IROOTPlotOptionsEntry::on_close_clicked()
{
    hide();
}

void IROOTPlotOptionsEntry::on_plotOptions_textChanged(const QString &arg1)
{
   plotOptionsChanged(arg1);
}
void IROOTPlotOptionsEntry::setText(QString arg)
{
ui->plotOptions->setText(arg);
}
