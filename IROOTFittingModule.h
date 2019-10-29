#ifndef IROOTFITTINGMODULE_H
#define IROOTFITTINGMODULE_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <IROOTPad.h>
#include <IROOTFitOptions.h>


namespace Ui {
class IROOTFittingModule;
}

class IROOTFittingModule : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTFittingModule(QWidget *parent = 0);
    void SetROOTObjectName(QString name);
    void SetIROOTPadObject(IROOTPad *object);
    void SetListofObjects(QStringList list);

    ~IROOTFittingModule();
    
private slots:
    void on_fit_clicked();

    void on_showFitOptions_clicked();

    void SetFitOption(QList<QTreeWidgetItem *> list);
private:
    Ui::IROOTFittingModule *ui;
    IROOTPad*               iPad;
    IROOTFitOptions*        fitOptionsWidget;
};

#endif // IROOTFITTINGMODULE_H
