#include "IROOTAsciiFileConvertor.h"
#include "ui_IROOTAsciiFileConvertor.h"

IROOTAsciiFileConvertor::IROOTAsciiFileConvertor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTAsciiFileConvertor)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    //    ui->widget->hide();
    //    adjustSize();

    tableMenu = new QMenu();
    isEditingOn=false;

    //              - I : a 32 bit signed integer (Int_t)
    //              - i : a 32 bit unsigned integer (UInt_t)
    //              - F : a 32 bit floating point (Float_t)
    //              - D : a 64 bit floating point (Double_t)
    //              - L : a 64 bit signed integer (Long64_t)
    //              - l : a 64 bit unsigned integer (ULong64_t)

    QAction *setName      = new QAction("set name",this);
    QAction *signedInt    = new QAction("signed integer",this);
    QAction *unsignedInt  = new QAction("unsigned integer",this);
    QAction *signedLong   = new QAction("signed Long",this);
    QAction *unsignedLong = new QAction("unsigned Long",this);
    QAction *ffloat       = new QAction("float",this);
    QAction *ddouble      = new QAction("double",this);
    QAction *sstring       = new QAction("string",this);

    tableMenu->addAction(setName);
    tableMenu->addAction(signedInt);
    tableMenu->addAction(unsignedInt);
    tableMenu->addAction(signedLong);
    tableMenu->addAction(unsignedLong);
    tableMenu->addAction(ffloat);
    tableMenu->addAction(ddouble);
    tableMenu->addAction(sstring);
    tableMenu->setStyleSheet("background-color: white; font: 8pt \"Segoe UI\"");
    connect(setName,SIGNAL(triggered()),this,SLOT(SetColumnName()));
    connect(signedInt,SIGNAL(triggered()),this,SLOT(SetAsSignedInteger()));
    connect(unsignedInt,SIGNAL(triggered()),this,SLOT(SetAsUnSignedInteger()));
    connect(signedLong,SIGNAL(triggered()),this,SLOT(SetAsSignedLong()));
    connect(unsignedLong,SIGNAL(triggered()),this,SLOT(SetAsUnSignedLong()));
    connect(ffloat,SIGNAL(triggered()),this,SLOT(SetAsFloat()));
    connect(ddouble,SIGNAL(triggered()),this,SLOT(SetAsDouble()));
    connect(sstring,SIGNAL(triggered()),this,SLOT(SetAsString()));

    currentFile="";
    ui->convert->setDisabled(true);

    convert = new ConvertThread();
    connect(convert, SIGNAL(Progress(int)), ui->progressBar,SLOT(setValue(int)));
    connect(convert,SIGNAL(Status(QString)),ui->message,SLOT(append(QString)));
    connect(convert,SIGNAL(Stopped(QString)),this,SLOT(NextFileInList(QString)));
    connect(convert,SIGNAL(started()),this,SLOT(DisableControls()));
    connect(convert,SIGNAL(Stopped()),this,SLOT(EnableControls()));
}

IROOTAsciiFileConvertor::~IROOTAsciiFileConvertor()
{
    delete ui;
}

//void IROOTAsciiFileConvertor::mousePressEvent(QMouseEvent *e)
//{
//    mousePressOrigin = e->globalPos();
//}
//void IROOTAsciiFileConvertor::mouseMoveEvent(QMouseEvent *e)
//{

//    if(e->buttons() & Qt::LeftButton ){
//        move(pos()-(mousePressOrigin-e->globalPos()));
//        mousePressOrigin = e->globalPos();
//    }
//}
void IROOTAsciiFileConvertor::EnableControls()
{
    ui->openAscii->setEnabled(true);
    ui->convert->setEnabled(true);
}
void IROOTAsciiFileConvertor::DisableControls()
{
    ui->openAscii->setDisabled(true);
    ui->convert->setDisabled(true);
}
void IROOTAsciiFileConvertor::on_openAscii_clicked()
{
    // ui->message->setText("");
    ui->widget->show();
    adjustSize();
    ui->tableWidget->clear();
    ui->fileStatus->clear();
    currentFileList =  QFileDialog::getOpenFileNames(this,"Open ASCII File", "","*.txt *.dat *.csv;;All files (*.*)");
    if(currentFileList.count()>0){

        currentIndex    =0;
        if(currentFileList.count()>1){
            QMessageBox msgBox;
            msgBox.setText("iRoot");
            msgBox.setInformativeText("The header set for the first file will be used for all the files if compatible else automatic headers will be used");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            //            switch (ret) {
            //            case QMessageBox::Yes:
            //                useSameHeader=true;
            //                break;
            //            case QMessageBox::Discard:
            //                useSameHeader=false;
            //                break;
            //            case QMessageBox::Cancel:
            //                useSameHeader=false;
            //                break;
            //            default:
            //                // should never be reached
            //                break;
            //            }

        }
        if(currentFileList.at(0)!=""){
            currentFile=currentFileList.at(0);
            QString delimiter = "\t";
            if(ui->commaSeparated->isChecked())
                delimiter =",";
            if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
                delimiter =ui->delimiter->text();

            currentDelimiter=delimiter;
            UpdateTable(currentFile,delimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
            ui->convert->setDisabled(false);
        }

    }

}

void IROOTAsciiFileConvertor::NextFileInList(QString outputFileName)
{
    if(outputFileName!=""){
        QStringList fileNames;
        fileNames.append(outputFileName);
        emit rootFilesReady(fileNames);
    }

    if(currentFileList.count()>1 && currentIndex<currentFileList.count()-1){
        currentIndex++;
        QString fileName    = currentFileList.at(currentIndex);
        worksheetHeader header;
        if(!IsCompatibleHeader(currentHeaderData,fileName,currentDelimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked())){
            header = GetWorkSheetHeader(fileName,currentDelimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
        }
        else
            header = currentHeaderData;

        int fileN=0;
        if(fileName!=""){
            QString outFileName, treeName;
            treeName = ui->treeName->text();
            if(treeName=="")treeName = "iRootFile";
            if(ui->changeExtension->isChecked()){
                outFileName = fileName.split(".").at(0);
                outFileName.append(".root");
            }
            else{
                QString newfileName = ui->outFileName->text().split(".").at(0);

                QString ext="root";
                if(ui->outFileName->text().split(".").count()>1){
                    ext = ui->outFileName->text().split(".").at(1);
                }
                outFileName =QString("%1_%2.%3").arg(newfileName).arg(fileN++).arg(ext);
                QFile f(outFileName);
                while(f.exists()){
                    outFileName =QString("%1_%2.%3").arg(newfileName).arg(fileN++).arg(ext);
                    f.setFileName(outFileName);
                }
            }
            ui->message->append(QString("Using output file name %1").arg(outFileName));
            int skipLines = ui->skipLines->value();
            if(ui->firstLineHeader->isChecked() && skipLines==0){
                skipLines++;
            }
            ConvertFile(fileName,header,currentDelimiter,skipLines,outFileName,treeName);

        }


    }
}
void IROOTAsciiFileConvertor::UpdateTable(QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader)
{

    // ui->message->setText("");

    QStringList header= GetHeader(fileName,delimiter,skipLines,firstLineIsHeader);

    ui->tableWidget->setColumnCount(header.count());
    ui->tableWidget->setRowCount(1);

    ui->tableWidget->setHorizontalHeaderLabels(header);

    if(firstLineIsHeader && skipLines==0){
        skipLines++;
    }

    QStringList firstValidString = ReadLine(fileName,delimiter, skipLines);

    for(int ii=0;ii<firstValidString.count();ii++){
        QTableWidgetItem *dataItem = new QTableWidgetItem(firstValidString.at(ii));
        ui->tableWidget->setItem(0,ii,dataItem);
    }
    UpdateHeaderData();
}

bool IROOTAsciiFileConvertor::IsCompatibleHeader(worksheetHeader header,QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader )
{
    if(firstLineIsHeader && skipLines==0){
        skipLines++;
    }
    QStringList firstValidString = ReadLine(fileName,delimiter, skipLines);
    if(firstValidString.count()!=header.type.count())
        return false;
    for(int ii=0;ii<firstValidString.count();ii++){
        if(!canConvert(firstValidString.at(ii),header.type.at(ii)))
            return false;
    }
    return true;
}


void IROOTAsciiFileConvertor::on_convert_clicked()
{
    if(currentFile!=""){
        currentIndex=0;
        QString outFileName, treeName;
        treeName = ui->treeName->text();
        if(treeName=="")treeName = "iRootFile";
        if(ui->changeExtension->isChecked()){
            outFileName = currentFile.split(".").at(0);
            outFileName.append(".root");
        }
        else{
            outFileName = ui->outFileName->text();
        }
        if(outFileName=="")
            outFileName="test.root";

        int skipLines = ui->skipLines->value();
        if(ui->firstLineHeader->isChecked() && skipLines==0){
            skipLines++;
        }
        ConvertFile(currentFile,currentHeaderData, currentDelimiter,skipLines,outFileName,treeName);
    }
}

void IROOTAsciiFileConvertor::ConvertFile(QString fileName, worksheetHeader header, QString delimiter, int skipLines,QString outFileName, QString treeName)
{
    convert->initialize(fileName,delimiter,header,skipLines);
    convert->setOutputParams(outFileName,treeName);
    convert->start();
}


QStringList IROOTAsciiFileConvertor::ReadLine(QString fileName, QString delimiter, int lineN)
{
    QFile file(fileName);
    QStringList lineData;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->fileStatus->setText("Error reading file...");
        return lineData;
    }
    int readLines=-1;

    while(lineN!=readLines){
        //return QString(file.readLine()).split(QRegExp(delimiter));
        lineData = QString(file.readLine()).split(QRegExp(delimiter));
        readLines++;

    }
    file.close();
    return lineData;

}

/*!
  \brief Gets the header line as a string using the first valid line in the text files

    If the line is automatically identified to be a string, long or a double

*/
QStringList IROOTAsciiFileConvertor::GetHeader(QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader)
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

    long    longVal;
    double  doubleVal;
    QString type;

    for(int ii=0;ii<headerString.count();ii++){
        bool ok;
        longVal=firstValidString.at(ii).toLong(&ok); // check for long
        type = headerString.at(ii);
        type.simplified();
        type.replace(" ","");
        if(ok){
            header.append(QString("%1:Long").arg(type));
        }
        else{ // check for double
            doubleVal=firstValidString.at(ii).toDouble(&ok); // check for double
            if(ok){
                header.append(QString("%1:Double").arg(type));
            }
            else{
                header.append(QString("%1:String").arg(type));
            }
        }

    }
    return header;
}

worksheetHeader IROOTAsciiFileConvertor::GetWorkSheetHeader(QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader)
{
    worksheetHeader h;
    QStringList l = GetHeader(fileName,delimiter,skipLines,firstLineIsHeader);
    for(int ii=0;ii<l.count();ii++){
        h.columnName.append(l.at(ii).split(":").at(0));
        h.type.append(l.at(ii).split(":").at(1));
    }
    return h;
}


QStringList IROOTAsciiFileConvertor::GetTableHeaders(bool removeType)
{
    int nCols = ui->tableWidget->columnCount();
    QStringList list;
    for(int ii=0;ii<nCols;ii++){
        if(!removeType)
        list.append(ui->tableWidget->horizontalHeaderItem(ii)->text());
        else // remove type string
        list.append(ui->tableWidget->horizontalHeaderItem(ii)->text().split(":").at(0));
    }
     return list;

}

bool IROOTAsciiFileConvertor::headerTextExists(QString text)
{
    QStringList listofHeaders = GetTableHeaders(true);
    if(listofHeaders.contains(text))
        return true;
    else
        return false;
}

void IROOTAsciiFileConvertor::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    tableMenu->exec(QCursor::pos());
}

void IROOTAsciiFileConvertor::on_firstLineHeader_clicked(bool checked)
{
    if(currentFile!=""){
        QString delimiter = "\t";
        if(ui->commaSeparated->isChecked())
            delimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            delimiter =ui->delimiter->text();
        UpdateTable(currentFile,delimiter,ui->skipLines->value(),checked);
    }
}

void IROOTAsciiFileConvertor::on_tabSeparated_clicked(bool checked)
{
    if(currentFile!=""){
        QString delimiter = "\t";
        if(ui->commaSeparated->isChecked())
            delimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            delimiter =ui->delimiter->text();
        UpdateTable(currentFile,delimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}
void IROOTAsciiFileConvertor::on_commaSeparated_clicked()
{
    if(currentFile!=""){
        QString delimiter = "\t";
        if(ui->commaSeparated->isChecked())
            delimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            delimiter =ui->delimiter->text();
        UpdateTable(currentFile,delimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

void IROOTAsciiFileConvertor::on_otherDelimiter_clicked(bool checked)
{
    if(currentFile!=""){
        QString delimiter = "\t";
        if(ui->commaSeparated->isChecked())
            delimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            delimiter =ui->delimiter->text();
        UpdateTable(currentFile,delimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

void IROOTAsciiFileConvertor::on_skipLines_valueChanged(int arg1)
{
    if(currentFile!=""){
        QString delimiter = "\t";
        if(ui->commaSeparated->isChecked())
            delimiter =",";
        if(ui->otherDelimiter->isChecked() && ui->delimiter->text()!="")
            delimiter =ui->delimiter->text();
        UpdateTable(currentFile,delimiter,ui->skipLines->value(),ui->firstLineHeader->isChecked());
    }
}

void IROOTAsciiFileConvertor::SetColumnName()
{
    //ui->message->setText("");
    isEditingOn=true;
    QString name = QInputDialog::getText(this,"Edit Column Name","Column Name:");
    if(name!=""){
        int begin;
        int end;
        QTableWidgetItem* headerItem;
        QString currentType;
        int count = ui->tableWidget->selectedRanges().count();
        int n=0;
        QString newName;
        bool hasErr=false;
        for(int ii=0;ii<count;ii++){
            begin    = ui->tableWidget->selectedRanges().at(ii).leftColumn();
            end      = ui->tableWidget->selectedRanges().at(ii).rightColumn();

            for(int cc=begin;cc<=end;cc++){
                headerItem = ui->tableWidget->horizontalHeaderItem(cc);
                if(headerItem!=NULL){
                    currentType = headerItem->text().split(":").at(1);
                    if(count==1 && ((end-begin)==0)){// single selection
                        newName =QString("%1:").arg(name).append(currentType);
                        if(!headerTextExists(name))
                            headerItem->setText(newName);
                        else{
                            hasErr=true;
                            //                            QMessageBox::warning(this, tr("iRoot"),
                            //                                                 tr("WARNING: Some columns could not be renamed as they were used already. Please try another name."),
                            //                                                 QMessageBox::Ok );
                            //ui->message->setText("WARNING: Some columns could not be renamed as they were used already. Please try another name.");
                        }
                    }
                    else{ // multiple selection
                        newName = QString("%1_%2:").arg(name).arg(n++).append(currentType);
                        if(!headerTextExists(name))
                            headerItem->setText(newName);
                        else{
                            hasErr=true;
                            //                            QMessageBox::warning(this, tr("iRoot"),
                            //                                                 tr("WARNING: Some columns could not be renamed as they were used already. Please try another name."),
                            //                                                 QMessageBox::Ok );
                            // ui->message->setText("WARNING: Some columns could not be renamed as they were used already. Please try another name.");

                        }
                    }
                }
            }
        }
        if(hasErr){
            if(count==1 && begin==end)
                QMessageBox::warning(this, tr("iRoot"),
                                     tr("WARNING: This column could not be renamed as the name has been used already. Please try another name."),
                                     QMessageBox::Ok );
            else
                QMessageBox::warning(this, tr("iRoot"),
                                     tr("WARNING: Some columns could not be renamed as the name has been used already. Please try another name."),
                                     QMessageBox::Ok );
        }

        UpdateHeaderData();
    }
    isEditingOn=false;
    raise();show();

}
void IROOTAsciiFileConvertor::SetAsSignedInteger()
{
    SetColumnType("Int");

}
void IROOTAsciiFileConvertor::SetAsUnSignedInteger()
{
    SetColumnType("UInt");
}
void IROOTAsciiFileConvertor::SetAsSignedLong()
{
    SetColumnType("Long");
}
void IROOTAsciiFileConvertor::SetAsUnSignedLong()
{
    SetColumnType("ULong");
}
void IROOTAsciiFileConvertor::SetAsFloat()
{
    SetColumnType("Float");
}
void IROOTAsciiFileConvertor::SetAsDouble()
{
    SetColumnType("Double");
}
void IROOTAsciiFileConvertor::SetAsString()
{
    SetColumnType("String");
}
void IROOTAsciiFileConvertor::SetColumnType(QString type)
{
    //ui->message->setText("");
    isEditingOn = true;
    int begin;
    int end;
    QTableWidgetItem* headerItem;
    QString currentHeader;
    int count = ui->tableWidget->selectedRanges().count();
    bool hasErr=false;
    for(int ii=0;ii<count;ii++){
        begin    = ui->tableWidget->selectedRanges().at(ii).leftColumn();
        end      = ui->tableWidget->selectedRanges().at(ii).rightColumn();

        for(int cc=begin;cc<=end;cc++){
            headerItem = ui->tableWidget->horizontalHeaderItem(cc);
            if(!canConvert(ui->tableWidget->item(0,cc)->text(),type)){
                hasErr=true;
            }
            else if(headerItem!=NULL){
                currentHeader = headerItem->text().split(":").at(0);
                headerItem->setText(currentHeader.append(":").append(type));
            }
        }
    }
    if(hasErr){
        if(count==1 && begin==end)
            QMessageBox::warning(this, tr("iRoot"),
                                 tr("Error: This column cannot be converted to the specified type (invalid conversion)!"),
                                 QMessageBox::Ok );
        else
            QMessageBox::warning(this, tr("iRoot"),
                                 tr("Error: Some columns cannot be converted to the specified type (invalid conversion)!"),
                                 QMessageBox::Ok );

    }
    UpdateHeaderData();
    isEditingOn = false;
     raise();show();
}
bool::IROOTAsciiFileConvertor::canConvert(QString value, QString type)
{
    bool ok;
    if(type=="Int"){
        value.toInt(&ok);
    }
    else if(type=="UInt"){
        value.toUInt(&ok);
    }
    else if(type=="Long"){
        value.toLong(&ok);
    }
    else if(type=="ULong"){
        value.toULong(&ok);
    }
    else if(type=="Float"){
        value.toFloat(&ok);
    }
    else if(type=="Double"){
        value.toDouble(&ok);
    }
    return ok;

}



void IROOTAsciiFileConvertor::UpdateHeaderData()
{

    currentHeaderData.columnName.clear();
    currentHeaderData.type.clear();
    int nCols = ui->tableWidget->columnCount();
    QString header;
    for(int ii=0;ii<nCols;ii++){
        header = ui->tableWidget->horizontalHeaderItem(ii)->text();
        currentHeaderData.columnName.append(header.split(":").at(0).simplified().replace(" ","").remove("\n").remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]"))));
        currentHeaderData.type.append(header.split(":").at(1));
    }
}


void IROOTAsciiFileConvertor::on_chooseOutputFileName_clicked()
{
    QString outFileName = QFileDialog::getSaveFileName(this,"Output file name");
    if(outFileName!=""){
        if(!outFileName.contains(".")){
            outFileName.append(".root");
        }
        ui->outFileName->setText(outFileName);
    }
}


void IROOTAsciiFileConvertor::on_stopConversion_clicked()
{
    convert->StopNow();
}



int ConvertThread::GetTypeCount(QString Type)
{
    int c =0;
    for(int ii=0;ii<h.type.count();ii++){
        if(h.type.at(ii)==Type)
            c++;
    }
    return c;
}
void ConvertThread::run()
{
    if(filename!=""){
        QFile file(filename);
        QStringList lineData;
        //Started(0); // filenumber not used currently
        EnableControls(false);
        Status(QString("Opening %1").arg(filename));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            Status(QString("Error: Could not open %1").arg(filename));
            Stopped("");
            EnableControls(true);
            return;
        }
        int readLines=-1;
        stopNow = false;


        //              - I : a 32 bit signed integer (Int_t)
        //              - i : a 32 bit unsigned integer (UInt_t)
        //              - F : a 32 bit floating point (Float_t)
        //              - D : a 64 bit floating point (Double_t)
        //              - L : a 64 bit signed integer (Long64_t)
        //              - l : a 64 bit unsigned integer (ULong64_t)


        Int_t       *intData;
        UInt_t      *UintData;
        Float_t     *floatData;
        Double_t    *doubleData;
        Long64_t    *longData;
        ULong64_t   *UlongData;
        TString     *stringData;


        if(GetTypeCount("Int")>0)
            intData = new Int_t[GetTypeCount("Int")];
        if(GetTypeCount("UInt")>0)
            UintData = new UInt_t[GetTypeCount("UInt")];
        if(GetTypeCount("Float")>0)
            floatData = new Float_t[GetTypeCount("Float")];
        if(GetTypeCount("Double")>0)
            doubleData = new Double_t[GetTypeCount("Double")];
        if(GetTypeCount("Long")>0)
            longData = new Long64_t[GetTypeCount("Long")];
        if(GetTypeCount("ULong")>0)
            UlongData = new ULong64_t[GetTypeCount("ULong")];
        if(GetTypeCount("String")>0)
            stringData = new TString[GetTypeCount("String")];

        int II,UII,FF,DD,LL,ULL,SS;
        II=0;UII=0;FF=0;DD=0;LL=0;ULL=0;SS=0;
        TFile *rootfile = new TFile(outFileName.toLatin1().data(),"RECREATE");
        TTree *tree = new TTree(treeName.toLatin1().data(), "Converted Using iRoot");

        for(int ii=0; ii<h.type.count();ii++){

            if(h.type.at(ii)=="Int"){
                tree->Branch(h.columnName.at(ii).toLatin1().data()
                             ,&intData[II++],
                             QString("%1/I").arg(h.columnName.at(ii)).toLatin1().data());
            }
            else if(h.type.at(ii)=="UInt"){
                tree->Branch(h.columnName.at(ii).toLatin1().data()
                             ,&UintData[UII++],
                             QString("%1/i").arg(h.columnName.at(ii)).toLatin1().data());
            }
            else if(h.type.at(ii)=="Long"){
                tree->Branch(h.columnName.at(ii).toLatin1().data()
                             ,&longData[LL++],
                             QString("%1/L").arg(h.columnName.at(ii)).toLatin1().data());
            }
            else if(h.type.at(ii)=="ULong"){
                tree->Branch(h.columnName.at(ii).toLatin1().data()
                             ,&UlongData[ULL++],
                             QString("%1/l").arg(h.columnName.at(ii)).toLatin1().data());
            }
            else if(h.type.at(ii)=="Float"){
                tree->Branch(h.columnName.at(ii).toLatin1().data()
                             ,&floatData[FF++],
                             QString("%1/F").arg(h.columnName.at(ii)).toLatin1().data());
            }
            else if(h.type.at(ii)=="Double"){
                tree->Branch(h.columnName.at(ii).toLatin1().data()
                             ,&doubleData[DD++],
                             QString("%1/D").arg(h.columnName.at(ii)).toLatin1().data());
            }
            else if(h.type.at(ii)=="String"){
                tree->Branch(h.columnName.at(ii).toLatin1().data(),"TString"
                             ,&stringData[SS++],
                             1000,0);
            }

        }
        bool ok;
        bool inValidLines =false;
        bool hasConversionErrors=false;
        Progress(0);
        double ratio;
        Started(0);
        while(!file.atEnd() && !stopNow){
            lineData = QString(file.readLine()).split(QRegExp(delimiter));
            readLines++;

            if(lineData.count()!=h.type.count()&&readLines>=skip){
                inValidLines=true;
                Status(QString("Invalid column count at line %1, skipping").arg(readLines));
            }

            ratio = (double)file.pos()/(double)file.size();
            //qDebug()<<ratio<<100*(int)ratio;
            emit Progress(100*ratio);

            II=0;UII=0;FF=0;DD=0;LL=0;ULL=0;SS=0;

            if(readLines>=skip && inValidLines==false){
                for(int ii=0; ii<h.type.count();ii++){
                    if(h.type.at(ii)=="Int"){
                        intData[II++] = lineData.at(ii).toInt(&ok);
                        if(!ok)
                            hasConversionErrors = true;
                    }
                    else if(h.type.at(ii)=="UInt"){
                        UintData[UII++] = lineData.at(ii).toUInt(&ok);
                        if(!ok)
                            hasConversionErrors = true;
                    }
                    else if(h.type.at(ii)=="Long"){
                        longData[LL++] = lineData.at(ii).toLong(&ok);
                        if(!ok)
                            hasConversionErrors = true;
                    }
                    else if(h.type.at(ii)=="ULong"){
                        UlongData[ULL++] = lineData.at(ii).toULong(&ok);
                        if(!ok)
                            hasConversionErrors = true;
                    }
                    else if(h.type.at(ii)=="Float"){
                        floatData[FF++] = lineData.at(ii).toFloat(&ok);
                        if(!ok)
                            hasConversionErrors = true;
                    }
                    else if(h.type.at(ii)=="Double"){
                        doubleData[DD++] = lineData.at(ii).toDouble(&ok);
                        if(!ok)
                            hasConversionErrors = true;
                    }
                    else if(h.type.at(ii)=="String"){
                        stringData[SS++] = lineData.at(ii).toLatin1().data();
                    }

                }
                tree->Fill();

            }
        }
        if(stopNow)
            Status("Conversion stopped.");
        if(hasConversionErrors)
            Status("Some columns could not be converted to the set type.");

        rootfile->Write();
        rootfile->Close();
        file.close();
        Status(QString("Output saved in %1").arg(outFileName));
        Stopped(outFileName);

        //        if(GetTypeCount("Int")>0)
        //            delete intData;
        //        if(GetTypeCount("UInt")>0)
        //            delete UintData;
        //        if(GetTypeCount("Float")>0)
        //            delete floatData;
        //        if(GetTypeCount("Double")>0)
        //            delete doubleData;
        //        if(GetTypeCount("Long")>0)
        //            delete longData;
        //        if(GetTypeCount("ULong")>0)
        //            delete UlongData;
        //        if(GetTypeCount("String")>0)
        //            delete stringData;
    }
}


void IROOTAsciiFileConvertor::on_close_clicked()
{
    hide();
}

void IROOTAsciiFileConvertor::on_saveWithDifferentName_clicked()
{
    if(ui->saveWithDifferentName->isChecked()&& ui->outFileName->text()=="")
        on_chooseOutputFileName_clicked();
}
