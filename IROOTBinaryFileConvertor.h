#ifndef IROOTBINARYFILECONVERTOR_H
#define IROOTBINARYFILECONVERTOR_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QThread>

enum dataTypes{INTEGER, UINTEGER, LONG, ULONG, FLOAT, DOUBLE, STRING};

namespace Ui {
  class IROOTBinaryFileConvertor;
}
class IROOTBinarySampleViewer;
class ConvertBinaryFileThread;
class IROOTBinaryFileConvertor : public QWidget
{
  Q_OBJECT
  
public:
  explicit IROOTBinaryFileConvertor(QWidget *parent = 0);

  template< typename T> QVariantList ReadFile(QString fileName, int nItems=-1); //-1 reads the entrire file
  ~IROOTBinaryFileConvertor();
signals:
  void openFiles(QStringList s);
private slots:
  void on_openFile_clicked();

  void on_readType_currentIndexChanged(int index);

  void on_convert_clicked();

  void EnableControls();
  void DisableControls();

private:
  Ui::IROOTBinaryFileConvertor *ui;
  QString currentFile;
  IROOTBinarySampleViewer* sampleModel;
  ConvertBinaryFileThread* converter;
};

class IROOTBinarySampleViewer:public QAbstractTableModel
{
  Q_OBJECT
public:
  IROOTBinarySampleViewer(){}
  int rowCount(const QModelIndex& parent) const
  {return sampledata.count();}
  int columnCount(const QModelIndex& parent) const
  {return 1;}
  QVariant data(const QModelIndex& index, int role) const{
    if(role==Qt::DisplayRole){
        if(index.row()<sampledata.count())
          return sampledata.at(index.row());
      }
    return QVariant::Invalid;
  }
  QVariant headerData(int section, Qt::Orientation orientation, int role) const
  {return QVariant::Invalid;}
  //void SetTable(QTableView* view){table = view;}
  // Qt::ItemFlags flags(const QModelIndex &index) const;
  void updateData(QVariantList d){sampledata.clear();sampledata=d; emit layoutChanged();}
private:
  QVariantList sampledata;

};

class ConvertBinaryFileThread : public QThread
{
  Q_OBJECT
public:
  void initialize(QString inputFile, int skip, int nCols, int conversionDataType){
    filename      = inputFile;
    skipItems = skip;
    nColumns  = nCols;
    conversionType = conversionDataType;
  }
  void setOutputParams(QString outfile, QString treename){
    outFileName = outfile;
    treeName    = treename;
  }

  QString getOutFileName(){return outFileName;}
  template< typename T> inline void ReadFileIntoTree(QString infileName, QString outFileName, QString treeName, int skip, int nCols);
  void StopNow(){stopNow=true;}
signals:
  void Started(int fileNumber);
  void Stopped(QString output="");
  void Status(QString status);
  void Progress(int progress);
  void EnableControls(bool en);

private:
  void run();
  QVariantList data;
  int skipItems;
  int nColumns;
  int conversionType;
  int GetTypeCount(QString type);
  QString filename;
  QString delimiter;

  QString outFileName;
  QString treeName;
  bool stopNow;
};
#endif // IROOTBINARYFILECONVERTOR_H
