#include "IROOTWorksheetImporter.h"
#include "ui_IROOTWorksheetImporter.h"

IROOTWorksheetImporter::IROOTWorksheetImporter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTWorksheetImporter)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    importThread = new WorksheetImportThread();
    connect(importThread,SIGNAL(Status(QString)),ui->message,SLOT(append(QString)));
    connect(importThread,SIGNAL(Progress(int)),ui->progressBar,SLOT(setValue(int)));
    connect(importThread,SIGNAL(started()),this,SLOT(DisableControls()));
    connect(importThread,SIGNAL(Stopped(int)),this,SLOT(EnableControls()));

}

IROOTWorksheetImporter::~IROOTWorksheetImporter()
{
    delete ui;
}

void IROOTWorksheetImporter::EnableControls()
{
ui->openAscii->setEnabled(true);
ui->startImport->setEnabled(true);
}

void IROOTWorksheetImporter::DisableControls()
{
ui->openAscii->setDisabled(true);
ui->startImport->setDisabled(true);
}

void IROOTWorksheetImporter::on_openAscii_clicked()
{
    ui->tableWidget->clear();
    currentFile =  QFileDialog::getOpenFileName(this,"Open ASCII File", "","*.txt *.dat *.csv;;All files (*.*)");
    if(currentFile!=""){
        currentDelimiter = "\t";
        if(ui->commaSeparated->isChecked())
            currentDelimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            currentDelimiter =ui->delimiter->text();

        UpdateTable(currentFile,currentDelimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

QStringList IROOTWorksheetImporter::ReadLine(QString fileName, QString delimiter, int lineN)
{
    QFile file(fileName);
    QStringList lineData;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->message->append("Error reading file...");
        return lineData;
    }
    int readLines=-1;

    while(lineN!=readLines){
        lineData = QString(file.readLine()).split(QRegExp(delimiter));
        readLines++;

    }
    file.close();
    return lineData;

}

void IROOTWorksheetImporter::UpdateTable(QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader)
{

    header= GetHeader(fileName,delimiter,skipLines,firstLineIsHeader);

    ui->tableWidget->setColumnCount(header.count());
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(header);

    if(firstLineIsHeader && skipLines==0){
        skipLines++;
    }

    QStringList firstValidString = ReadLine(fileName,delimiter, skipLines);
    ui->tableWidget->setColumnCount(firstValidString.count());

    for(int ii=0;ii<firstValidString.count();ii++){
        QTableWidgetItem *dataItem = new QTableWidgetItem(firstValidString.at(ii));
        ui->tableWidget->setItem(0,ii,dataItem);
    }
    ui->tableWidget->resizeColumnsToContents();
}

QStringList IROOTWorksheetImporter::GetHeader(QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader)
{
    //ui->message->setText("");

    QStringList header;
    QStringList headerString;
    QStringList firstValidString;

    int firstValidLine =skipLines;
    if(firstLineIsHeader && skipLines==0){
        firstValidLine++;
        headerString = ReadLine(fileName,delimiter,0);
    }
    firstValidString = ReadLine(fileName,delimiter,firstValidLine);
    int nCols = firstValidString.count();
    if(firstValidString.count()!=headerString.count()){
        ui->message->append("Incorrect header count, using default headers");
        firstLineIsHeader = false;
    }

    if(!firstLineIsHeader){
        headerString.clear();
        for(int ii=0; ii<nCols; ii++){
            headerString.append(QString("c%1").arg(ii));
        }
    }

    //    long    longVal;
    //    double  doubleVal;
    //    QString type;

    //    for(int ii=0;ii<headerString.count();ii++){
    //        bool ok;
    //        longVal=firstValidString.at(ii).toLong(&ok); // check for long
    //        type = headerString.at(ii);
    //        type.simplified();
    //        type.replace(" ","");
    //        if(ok){
    //            header.append(QString("%1:Long").arg(type));
    //        }
    //        else{ // check for double
    //            doubleVal=firstValidString.at(ii).toDouble(&ok); // check for double
    //            if(ok){
    //                header.append(QString("%1:Double").arg(type));
    //            }
    //            else{
    //                header.append(QString("%1:String").arg(type));
    //            }
    //        }

    //    }
    return headerString;
}
void IROOTWorksheetImporter::on_close_clicked()
{
    hide();
}

void IROOTWorksheetImporter::on_tabSeparated_clicked()
{
    if(currentFile!=""){
        currentDelimiter = "\t";
        if(ui->commaSeparated->isChecked())
            currentDelimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            currentDelimiter =ui->delimiter->text();

        UpdateTable(currentFile,currentDelimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

void IROOTWorksheetImporter::on_commaSeparated_clicked()
{
    if(currentFile!=""){
        currentDelimiter = "\t";
        if(ui->commaSeparated->isChecked())
            currentDelimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            currentDelimiter =ui->delimiter->text();

        UpdateTable(currentFile,currentDelimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

void IROOTWorksheetImporter::on_otherDelimiter_clicked()
{
    if(currentFile!=""){
        currentDelimiter = "\t";
        if(ui->commaSeparated->isChecked())
            currentDelimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            currentDelimiter =ui->delimiter->text();

        UpdateTable(currentFile,currentDelimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

void IROOTWorksheetImporter::on_firstLineHeader_clicked(bool checked)
{
    if(currentFile!=""){
        currentDelimiter = "\t";
        if(ui->commaSeparated->isChecked())
            currentDelimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            currentDelimiter =ui->delimiter->text();

        UpdateTable(currentFile,currentDelimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

void IROOTWorksheetImporter::on_skipLines_valueChanged(int arg1)
{
    if(currentFile!=""){
        currentDelimiter = "\t";
        if(ui->commaSeparated->isChecked())
            currentDelimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            currentDelimiter =ui->delimiter->text();

        UpdateTable(currentFile,currentDelimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

void IROOTWorksheetImporter::on_startImport_clicked()
{
    if(currentFile!=NULL){
        int skip =ui->skipLines->value();
        if(ui->firstLineHeader->isChecked() && ui->skipLines->value()==0){
            skip++;
        }
        importThread->initialize(currentFile,currentDelimiter,header,skip);
        importThread->start();
    }
}

void WorksheetImportThread::run()
{
    QFile file(filename);
    QStringList lineData;
    Status(QString("Opening %1").arg(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        Status(QString("Error: Could not open %1").arg(filename));
       // Stopped(-1);
        return;
    }
    int readLines=-1;

    stopNow = false;

    Progress(0);
    double ratio;
    Started(0);
    // QTableWidget *table= new QTableWidget();
    int ii=0;
    data.clear();
    nRows=0;
    nCols=0;
    while(!file.atEnd() && !stopNow){
        lineData = QString(file.readLine()).split(QRegExp(delimiter));
        readLines++;
        ratio = (double)file.pos()/(double)file.size();
        emit Progress(100*ratio);

        if(readLines>=skip){
            data.append(lineData);
            //            for(ii=0;ii<lineData.count();ii++){
            //                //                QTableWidgetItem *item = new QTableWidgetItem(lineData.at(ii));
            //                //                table->setRowCount(nRows);
            //                //                if(lineData.count()>nCols){
            //                //                    table->setColumnCount(lineData.count());
            //                //                    nCols=lineData.count();
            //                //                }
            //                //                table->setItem(nRows-1,ii,item);
            //                //SetTableData(nRows-1,ii,lineData.at(ii));
            //                data.append(lie);

            //            }
            if(lineData.count()>nCols)
                nCols = lineData.count();
            nRows++;
            //            qDebug()<<"SENT"<<nRows;
        }
    }

    if(stopNow)
        Status("Conversion stopped.");
    file.close();
    Stopped(0);


}

void IROOTWorksheetImporter::on_stop_clicked()
{
    importThread->StopNow();
}
