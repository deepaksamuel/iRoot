#include "IROOTBinaryFileConvertor.h"
#include "ui_IROOTBinaryFileConvertor.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <TFile.h>
#include <TTree.h>

IROOTBinaryFileConvertor::IROOTBinaryFileConvertor(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::IROOTBinaryFileConvertor)
{
  ui->setupUi(this);
  sampleModel = new IROOTBinarySampleViewer();
  ui->sampleViewer->setModel(sampleModel);
  currentFile ="";
  converter = new ConvertBinaryFileThread();
  connect(converter, SIGNAL(Progress(int)), ui->progressBar,SLOT(setValue(int)));
  connect(converter,SIGNAL(Status(QString)),ui->status,SLOT(setText(QString)));
  connect(converter,SIGNAL(started()),this,SLOT(DisableControls()));
  connect(converter,SIGNAL(Stopped()),this,SLOT(EnableControls()));
  ui->progressBar->setValue(0);
}

IROOTBinaryFileConvertor::~IROOTBinaryFileConvertor()
{
  delete ui;
}

void IROOTBinaryFileConvertor::EnableControls()
{
  ui->openFile->setEnabled(true);
  ui->convert->setEnabled(true);
  QStringList convertedFiles;
  convertedFiles.append(converter->getOutFileName());
  openFiles(convertedFiles);
}


void IROOTBinaryFileConvertor::DisableControls()
{
  ui->progressBar->setValue(0);
  ui->openFile->setEnabled(false);
  ui->convert->setEnabled(false);

}

template< typename T> QVariantList IROOTBinaryFileConvertor::ReadFile(QString fileName, int nItems)
{
  QFile binFile(fileName);
  QVariantList fullData;
  if (!binFile.open(QIODevice::ReadOnly)){
      QMessageBox::critical(0,"Error",QString("Could not open file %1 for reading...").arg(fileName));
      return fullData;
    }
  ui->fileName->setText(fileName);
  int lineN=0;

  T data;
  QDataStream in(&binFile);

  while (!binFile.atEnd()) {
      in >> data;
      fullData.append(data);
      lineN++;
      if(nItems!=-1 && nItems==lineN)
        break;
    }
  binFile.close();
  // qDebug()<<"Full data count"<<fullData.count();
  return fullData;
}

void IROOTBinaryFileConvertor::on_openFile_clicked()
{
  currentFile =  QFileDialog::getOpenFileName(this,"Open binary file", "","*.bin *.dat;;All files (*.*)");
  if(currentFile!=""){
      if(ui->readType->currentIndex()==INTEGER)
        sampleModel->updateData(ReadFile<int>(currentFile,25));
      if(ui->readType->currentIndex()==UINTEGER)
        sampleModel->updateData(ReadFile<unsigned int>(currentFile,25));
      if(ui->readType->currentIndex()==LONG)
        sampleModel->updateData(ReadFile<qint64>(currentFile,25));
      if(ui->readType->currentIndex()==ULONG)
        sampleModel->updateData(ReadFile<quint64>(currentFile,25));
      if(ui->readType->currentIndex()==FLOAT)
        sampleModel->updateData(ReadFile<float>(currentFile,25));
      if(ui->readType->currentIndex()==DOUBLE)
        sampleModel->updateData(ReadFile<double>(currentFile,25));
      if(ui->readType->currentIndex()==STRING)
        sampleModel->updateData(ReadFile<QString>(currentFile,25));
    }
}

void IROOTBinaryFileConvertor::on_readType_currentIndexChanged(int index)
{
  if(currentFile!=""){
      if(ui->readType->currentIndex()==INTEGER)
        sampleModel->updateData(ReadFile<int>(currentFile,25));
      if(ui->readType->currentIndex()==UINTEGER)
        sampleModel->updateData(ReadFile<unsigned int>(currentFile,25));
      if(ui->readType->currentIndex()==LONG)
        sampleModel->updateData(ReadFile<qint64>(currentFile,25));
      if(ui->readType->currentIndex()==ULONG)
        sampleModel->updateData(ReadFile<quint64>(currentFile,25));
      if(ui->readType->currentIndex()==FLOAT)
        sampleModel->updateData(ReadFile<float>(currentFile,25));
      if(ui->readType->currentIndex()==DOUBLE)
        sampleModel->updateData(ReadFile<double>(currentFile,25));
      if(ui->readType->currentIndex()==STRING)
        sampleModel->updateData(ReadFile<QString>(currentFile,25));
    }
}

void ConvertBinaryFileThread::run()
{

  //              - I : a 32 bit signed integer (Int_t)
  //              - i : a 32 bit unsigned integer (UInt_t)
  //              - F : a 32 bit floating point (Float_t)
  //              - D : a 64 bit floating point (Double_t)
  //              - L : a 64 bit signed integer (Long64_t)
  //              - l : a 64 bit unsigned integer (ULong64_t)
  if(conversionType==INTEGER)
    ReadFileIntoTree<int>(filename,outFileName,treeName,skipItems,nColumns);
  if(conversionType==UINTEGER)
    ReadFileIntoTree<unsigned int>(filename,outFileName,treeName,skipItems,nColumns);
  if(conversionType==LONG)
    ReadFileIntoTree<Long64_t>(filename,outFileName,treeName,skipItems,nColumns);
  if(conversionType==ULONG)
    ReadFileIntoTree<ULong64_t>(filename,outFileName,treeName,skipItems,nColumns);
  if(conversionType==FLOAT)
    ReadFileIntoTree<float>(filename,outFileName,treeName,skipItems,nColumns);
  if(conversionType==DOUBLE)
    ReadFileIntoTree<double>(filename,outFileName,treeName,skipItems,nColumns);
  if(conversionType==STRING)
    ReadFileIntoTree<QString>(filename,outFileName,treeName,skipItems,nColumns);
}

template< typename T> inline void ConvertBinaryFileThread ::ReadFileIntoTree(QString infileName, QString outFileName, QString treeName, int skip, int nCols)
{
  QFile binFile(infileName);
  if (!binFile.open(QIODevice::ReadOnly)){
      Status(QString("Could not open file %1 for reading...").arg(infileName));
    }


  int lineN=0;
  T data;
  T *columnData = new T[nCols];

  TFile *rootfile = new TFile(outFileName.toLatin1().data(),"RECREATE");
  TTree *tree = new TTree(treeName.toLatin1().data(), "Converted Using iRoot");

  for(int ii=0; ii<nCols;ii++){
      tree->Branch(QString("C%1").arg(ii).toLatin1().data(),&columnData[ii]);
    }

  QDataStream in(&binFile);

  Progress(0);
  double ratio;
  Started(0);
  int cc=0;
  Status("Converting, please wait!");
  while (!binFile.atEnd()) {
      in >> data;
      if(lineN>=skip){
          columnData[cc++]=data;
          if(cc==nCols){
              cc=0;
              tree->Fill();
              ratio = (double)binFile.pos()/(double)binFile.size();
              emit Progress(100*ratio);
            }
        }
      lineN++;

    }
  emit Progress(100);


  rootfile->Write();
  rootfile->Close();
  binFile.close();

  Status("Done !");
  Stopped();
}

void IROOTBinaryFileConvertor::on_convert_clicked()
{
  if(currentFile!=""){
      converter->initialize(currentFile,ui->skipItems->value(),ui->nColumns->value(),ui->readType->currentIndex());
      QStringList filenameSplit = currentFile.split(".");
      QString filePrefix  = filenameSplit.at(0);
      QString outFileName = QFileDialog::getSaveFileName(0,"Save root file",filePrefix.append(".root"));
      converter->setOutputParams(outFileName,"binTOtree");
      converter->start();
    }
}
