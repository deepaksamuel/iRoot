#ifndef IROOTCOMPILER_H
#define IROOTCOMPILER_H

#include <QWidget>
#include <IROOT.h>
#include <IROOTSyntaxHighlighter.h>
#include <TSelector.h>

namespace Ui {
class IROOTCompiler;
}

class IROOTCompiler : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTCompiler(QWidget *parent = 0);
    void ReadSourceFile(QString fileName);
    void ReadHeaderFile(QString fileName);
    void SaveFiles(QString fileName, QString replacethis="",QString replaceby="");
    void OpenFile(QString fileName);
    void CreateCode(TTree* tree);
    void ProcessTree(TTree *tree, QString codeFileName);

    ~IROOTCompiler();
    
private slots:
    void on_createSkeletonCode_clicked();
    void on_execute_clicked();

    void on_save_clicked();

    void on_open_clicked();

signals:
    void ShowMessage(QString message, QString type);
    void CreateNewPad();

private:
    Ui::IROOTCompiler *ui;
};

#endif // IROOTCOMPILER_H


