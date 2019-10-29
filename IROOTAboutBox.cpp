#include "IROOTAboutBox.h"
#include "ui_IROOTAboutBox.h"

IROOTAboutBox::IROOTAboutBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IROOTAboutBox)
{
    ui->setupUi(this);
}

IROOTAboutBox::~IROOTAboutBox()
{
    delete ui;
}
