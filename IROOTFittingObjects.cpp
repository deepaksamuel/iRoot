#include "IROOTFittingObjects.h"
#include "ui_IROOTFittingObjects.h"

IROOTFittingObjects::IROOTFittingObjects(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTFittingObjects)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

IROOTFittingObjects::~IROOTFittingObjects()
{
    delete ui;
}

void IROOTFittingObjects::on_close_clicked()
{
    hide();
}

void IROOTFittingObjects::Update()
{
    if(gPad!=NULL){
        ui->fittableObjects->clear();
        int n = IROOTPad::GetNPads();
        if(n<1) return;
        else{
            for(int ii=0;ii<n;ii++){
                QTreeWidgetItem *top = new QTreeWidgetItem();
                top->setText(0,QString("Pad %1").arg(ii+1));
                ui->fittableObjects->addTopLevelItem(top);
                TVirtualPad* pad = gPad->GetCanvas()->GetPad(ii+1);
                if(pad!=0){
                    QStringList l = IROOTPad::GetListofFittables(pad);
                    for(int jj=0;jj<l.count();jj++){
                        QTreeWidgetItem *t = new QTreeWidgetItem(top);
                        t->setText(0,l.at(jj));
                    }

                }


            }
            ui->fittableObjects->expandAll();
            emit itemsSelected(GetListofSelectedObjects());

        }


    }

}

void IROOTFittingObjects::on_fittableObjects_itemClicked(QTreeWidgetItem *item, int column)
{
    //format: [padnumber.primNumber] objectName
    if(item->text(0).contains(".")){

        if(ui->highLight->isChecked()){
            QString id = GetObjectId(item->text(0));
            emit highLight(id);
        }
    }
    emit itemsSelected(GetListofSelectedObjects());
}

QString IROOTFittingObjects::GetObjectId(QString s)
{
    QStringList list = s.split("]");
    QString id = list.at(0);
    return id.remove("[");
}

QStringList IROOTFittingObjects::GetListofSelectedObjects()
{
    QStringList l;
    QList <QTreeWidgetItem*> items = ui->fittableObjects->selectedItems();
    for(int ii=0;ii<items.count();ii++){
        if(items.at(ii)->text(0).contains(".")&&items.at(ii)->text(0).contains("[")&&items.at(ii)->text(0).contains("]")){
            l.append(items.at(ii)->text(0));
        }

    }
    return l;

}

