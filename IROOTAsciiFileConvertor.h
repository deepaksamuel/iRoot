#ifndef IROOTASCIIFILECONVERTOR_H
#define IROOTASCIIFILECONVERTOR_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QMenu>
#include <QComboBox>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>
#include <QMouseEvent>


#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"


class ConvertThread;
namespace Ui {
class IROOTAsciiFileConvertor;
}

typedef struct {
    QStringList columnName;
    QStringList type;
} worksheetHeader;

class IROOTAsciiFileConvertor : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTAsciiFileConvertor(QWidget *parent = 0);
    bool isEditing(){return isEditingOn;}
    ~IROOTAsciiFileConvertor();

    
private slots:
    void on_openAscii_clicked();
    void ConvertFile(QString fileName, worksheetHeader header, QString delimiter="\t", int skipLines=0, QString outFileName="test.root", QString treeName="iRootFile");
    void UpdateTable(QString fileName, QString delimiter="\t", int skipLines=0, bool firstLineIsHeader=false);
    QStringList ReadLine(QString fileName, QString delimiter="\t", int lineN=0);
    QStringList GetHeader(QString fileName, QString delimiter="\t", int skipLines=0, bool firstLineIsHeader=false);
    worksheetHeader GetWorkSheetHeader(QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader);

    QStringList GetTableHeaders(bool removeType =false);
    bool headerTextExists(QString text);

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void on_firstLineHeader_clicked(bool checked);

    void on_commaSeparated_clicked();

    void on_tabSeparated_clicked(bool checked);

    void on_otherDelimiter_clicked(bool checked);

    void on_skipLines_valueChanged(int arg1);
    void SetColumnName();
    void SetAsSignedInteger();
    void SetAsUnSignedInteger();
    void SetAsSignedLong();
    void SetAsUnSignedLong();
    void SetAsFloat();
    void SetAsDouble();
    void SetAsString();
    void NextFileInList(QString outputFileName);
    void EnableControls();
    void DisableControls();
    //    void SetStatus(QString);


    void on_convert_clicked();

    void on_chooseOutputFileName_clicked();

    void on_stopConversion_clicked();

    void on_close_clicked();

    void on_saveWithDifferentName_clicked();


signals:
    void SetMessage(QString message);
    void rootFilesReady(QStringList fileNames);
private:
    Ui::IROOTAsciiFileConvertor *ui;
    QMenu           *tableMenu;
    void            SetColumnType(QString);
    worksheetHeader currentHeaderData;
    void            UpdateHeaderData();
    bool            canConvert(QString value, QString type);
    bool            IsCompatibleHeader(worksheetHeader header,QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader );
    ConvertThread   *convert;
    QString         currentFile;
    QStringList     currentFileList;
    int             currentIndex;
    QString         currentDelimiter;
    bool            useSameHeader;
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
    bool isEditingOn; // flag to show that some editing is going



};

class ConvertThread : public QThread
{
    Q_OBJECT
public:
    void initialize(QString fileName, QString Delimiter, worksheetHeader header, int skipLines){
        filename =  fileName;
        h        =  header;
        skip     =  skipLines;
        delimiter=  Delimiter;
    }
    void setOutputParams(QString outfile, QString treename){
        outFileName = outfile;
        treeName    = treename;
    }
    void StopNow(){stopNow=true;}
signals:
    void Started(int fileNumber);
    void Stopped(QString output="");
    void Status(QString status);
    void Progress(int progress);
    void EnableControls(bool en);

private:
    void run();
    int GetTypeCount(QString type);
    QString filename;
    worksheetHeader h;
    QString delimiter;
    int skip;
    QString outFileName;
    QString treeName;
    bool stopNow;
};



#endif // IROOTASCIIFILECONVERTOR_H
