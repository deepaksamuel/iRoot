#include "IROOTCompiler.h"
#include "ui_IROOTCompiler.h"

IROOTCompiler::IROOTCompiler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTCompiler)
{
    ui->setupUi(this);
    IROOTSyntaxHighlighter *sourceHighlighter = new IROOTSyntaxHighlighter(ui->sourceText->document());
    IROOTSyntaxHighlighter *headerHighlighter = new IROOTSyntaxHighlighter(ui->headerText->document());
}

IROOTCompiler::~IROOTCompiler()
{
    delete ui;
}

void IROOTCompiler::on_createSkeletonCode_clicked()
{
    if(IROOT::currentTree==NULL){
        ShowMessage("No trees present!","ERR");
        return;
    }
    if(!ui->sourceText->toPlainText().simplified().isEmpty() || !ui->headerText->toPlainText().simplified().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("The current code will be deleted.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Save:
            on_save_clicked();
            break;
        case QMessageBox::Discard:
            CreateCode(IROOT::currentTree);
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }

    }
    else
        CreateCode(IROOT::currentTree);




}

void IROOTCompiler::CreateCode(TTree *tree)
{
    if(tree !=NULL){
        tree->MakeSelector("SkeletonCode");
        ReadSourceFile("SkeletonCode.C");
        ReadHeaderFile("SkeletonCode.h");
    }
    else
        ShowMessage("Invalid tree","ERR");

}

void IROOTCompiler::ReadSourceFile(QString fileName)
{
    if(fileName!=""){

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            ShowMessage(QString("Error: Could not open %1").arg(fileName),"ERR");
            return;
        }
        ui->sourceText->clear();
        //while(!file.atEnd()){
        //lineData = QString(file.readLine()).split(QRegExp(delimiter));
        //ui->sourceText->appendPlainText(file.readAll());
        ui->sourceText->append(file.readAll());
        //}
    }
}

void IROOTCompiler::ReadHeaderFile(QString fileName)
{
    if(fileName!=""){

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            ShowMessage(QString("Error: Could not open %1").arg(fileName),"ERR");
            return;
        }
        ui->headerText->clear();
        //while(!file.atEnd()){
        //  lineData = QString(file.readLine()).split(QRegExp(delimiter));
        //ui->headerText->appendPlainText(file.readAll());
        ui->headerText->append(file.readAll());
        //}
    }
}

void IROOTCompiler::SaveFiles(QString fileName, QString replacethis, QString replaceby)
{

    if(fileName!=""){
        if(fileName.contains("."))
            fileName = fileName.split(".").at(0);
        QString sourceFileName = QString("%1.C").arg(fileName);
        QString headerFileName = QString("%1.h").arg(fileName);

        QFile sfile(sourceFileName);
        if (!sfile.open(QIODevice::WriteOnly | QIODevice::Text)){
            ShowMessage(QString("Error: Could not open %1 for writing").arg(sourceFileName),"ERR");
            return;
        }
        QTextStream out1(&sfile);
        QString data = ui->sourceText->toPlainText().replace(replacethis,replaceby);
        out1 <<data;
        sfile.close();

        QFile hfile(headerFileName);
        if (!hfile.open(QIODevice::WriteOnly | QIODevice::Text)){
            ShowMessage(QString("Error: Could not open %1 for writing").arg(headerFileName),"ERR");
            return;
        }
        QTextStream out2(&hfile);
        data = ui->headerText->toPlainText().replace(replacethis,replaceby);
        out2 <<data;
        hfile.close();
        ShowMessage(QString("Saved in %1").arg(sourceFileName),"SUCCESS");
    }

}

void IROOTCompiler::on_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save source and header files");
    if(fileName!="")
        SaveFiles(fileName,"","");
}


void IROOTCompiler::ProcessTree(TTree *tree, QString codeFileName)
{
    if(tree!=NULL){
        if(gPad==NULL)
            CreateNewPad();
        if(codeFileName!=NULL){
            //            TSelector *selector = TSelector::GetSelector(codeFileName.toLatin1().data());
            //            if(selector==NULL) ShowMessage("Error compiling code, check for errors","ERR");
            //            else if(tree->Process(selector)==-1)
            //                ShowMessage("Error compiling code, check for errors","ERR");
            //            if(selector!=NULL)delete selector;
            //gROOT->ProcessLine(codeFileName.prepend(".L ").toLatin1().data());

            if(tree->Process(codeFileName.toLatin1().data())==-1)
                ShowMessage("Error compiling code, check for errors","ERR");
        }

    }
}



void IROOTCompiler::on_execute_clicked()
{
    if(IROOT::currentTree==NULL){
        ShowMessage("No trees present!","ERR");
        return;
    }
    if(!ui->sourceText->toPlainText().simplified().isEmpty() || !ui->headerText->toPlainText().simplified().isEmpty())
    {
        static int codeN=0;
        //        QFile f1("irootcode.C");
        //        if(f1.exists())f1.remove();
        //        QFile f2("irootcode.h");
        //        if(f2.exists())f2.remove();
        // Everytime the code is compiled it seems we need a new file name. We should then delete them at the next call
        QString fileName    = QString("%1%2").arg("irootcode").arg(codeN++);
        QString cFileName   = QString("%1.C").arg(fileName);
        QString hFileName   = QString("%1.h").arg(fileName);
        QString dllFileName     = QString("%1_C.dll").arg(fileName);
        QString callFileName    = QString("%1.C++").arg(fileName);

        SaveFiles(fileName,"SkeletonCode",fileName);
        QFile sf(cFileName);
        if(!sf.exists()){
            ShowMessage("Could not save file for compiling","ERR");
            return;
        }
        QFile hf(hFileName);
        if(!hf.exists()){
            ShowMessage("Could not save file for compiling","ERR");
            return;
        }
        QFile f3(dllFileName); // should change for linux
        if(f3.exists())f3.remove();

        ProcessTree(IROOT::currentTree,callFileName);// if a + is appended (irootcode.C+) then code is compiled else interpreted. But host computer requires compiler to be preinstalled for compiling!!!!
    }
}

void IROOTCompiler::on_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open ROOT skeleton files","","*.C;;*.h");
    if(fileName!=""){
        if(fileName.contains("."))
            fileName = fileName.split(".").at(0);
        OpenFile(fileName);
    }
}

void IROOTCompiler::OpenFile(QString fileName)
{
    if(fileName!=""){
        QFile f1(QString("%1.C").arg(fileName));
        if(!f1.exists()){
            ShowMessage(QString("Could not find %1 for opening").arg(f1.fileName()),"ERR");
            return;
        }
        QFile f2(QString("%1.h").arg(fileName));
        if(!f1.exists()){
            ShowMessage(QString("Could not find %1 for opening").arg(f2.fileName()),"ERR");
            return;
        }
        ReadSourceFile(f1.fileName());
        ReadHeaderFile(f2.fileName());
    }

}
