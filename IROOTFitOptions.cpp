#include "IROOTFitOptions.h"
#include "ui_IROOTFitOptions.h"

IROOTFitOptions::IROOTFitOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTFitOptions)
{
    ui->setupUi(this);
     setWindowFlags(Qt::FramelessWindowHint);
}

IROOTFitOptions::~IROOTFitOptions()
{
    delete ui;
}

void IROOTFitOptions::on_fitOptions_itemClicked(QTreeWidgetItem *item, int column)
{
    emit selectionChanged(ui->fitOptions->selectedItems());
}

void IROOTFitOptions::on_close_clicked()
{
    hide();
}
