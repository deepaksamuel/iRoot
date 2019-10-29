#include "IROOTFittingModule.h"
#include "ui_IROOTFittingModule.h"

IROOTFittingModule::IROOTFittingModule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTFittingModule)
{
    ui->setupUi(this);
    fitOptionsWidget = new IROOTFitOptions();
    connect(fitOptionsWidget,SIGNAL(selectionChanged(QList<QTreeWidgetItem*>)),this,SLOT(SetFitOption(QList<QTreeWidgetItem*>)));
}

IROOTFittingModule::~IROOTFittingModule()
{
    delete ui;
}

void IROOTFittingModule::SetROOTObjectName(const QString name)
{
    ui->fitObjectName->setText(name);
}


void IROOTFittingModule::SetIROOTPadObject(IROOTPad* object)
{
    // iPad = object;
}

void IROOTFittingModule::on_fit_clicked()
{
    QStringList             parNames;
    QList <double>          parValues;
    for(int ii=0;ii<10;ii++){
        if(ui->parTable->item(ii,0)!=NULL){
            parNames.append(ui->parTable->item(ii,0)->text());
        }
        else{
            parNames.append("");
        }
        if(ui->parTable->item(ii,1)!=NULL){
            parValues.append(ui->parTable->item(ii,1)->text().toDouble());
        }
        else{
            parValues.append(0);
        }
    }
   // QStringList results = IROOTPad::FitData(ui->fitObjectName->text(),ui->fitFunction->currentText(),parNames,parValues,ui->fitOptions->text(),ui->rangeMin->value(),ui->rangeMax->value());
    //for(int ii=0;ii<results.count();ii++)
   //     ui->results->appendPlainText(results.at(ii));
   // ui->results->appendPlainText("______________________________________________________________________________________");
}

void IROOTFittingModule::on_showFitOptions_clicked()
{
    fitOptionsWidget->show();
    fitOptionsWidget->raise();
    fitOptionsWidget->move(QCursor::pos().x(),QCursor::pos().y());
}

void IROOTFittingModule::SetFitOption(QList<QTreeWidgetItem *> list)
{
    QString options;
    for(int ii=0;ii<list.count();ii++){
        if(list.at(ii)->text(0).contains("|"))
            options.append(list.at(ii)->text(0).split("|").at(1));
    }
    ui->fitOptions->setText(options);


}
void IROOTFittingModule::SetListofObjects(QStringList list)
{
    ui->fitObjects->clear();

    for(int ii=0;ii<list.count();ii++){
        QTreeWidgetItem *t = new QTreeWidgetItem();
        t->setText(0,list.at(ii));
        ui->fitObjects->addTopLevelItem(t);
    }
}



