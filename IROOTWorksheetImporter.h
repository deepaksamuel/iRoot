#ifndef IROOTWORKSHEETIMPORTER_H
#define IROOTWORKSHEETIMPORTER_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QThread>
#include <QDebug>
#include <QMouseEvent>

namespace Ui {
class IROOTWorksheetImporter;
}
class WorksheetImportThread;
class IROOTWorksheetImporter : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTWorksheetImporter(QWidget *parent = 0);
    ~IROOTWorksheetImporter();
    QStringList ReadLine(QString fileName, QString delimiter, int lineN);
    void UpdateTable(QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader);
    QStringList GetHeader(QString fileName, QString delimiter, int skipLines, bool firstLineIsHeader);
    WorksheetImportThread* GetImportThread(){return importThread;}
    
private slots:
    void on_openAscii_clicked();

    void on_close_clicked();

    void on_tabSeparated_clicked();

    void on_commaSeparated_clicked();

    void on_otherDelimiter_clicked();

    void on_firstLineHeader_clicked(bool checked);

    void on_skipLines_valueChanged(int arg1);

    void on_startImport_clicked();

    void on_stop_clicked();
    void EnableControls();
    void DisableControls();

private:
    Ui::IROOTWorksheetImporter *ui;
    QStringList header;
    QString currentFile;
    QString currentDelimiter;
    WorksheetImportThread *importThread;
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
};

class WorksheetImportThread : public QThread
{
    Q_OBJECT
public:
    void initialize(QString fileName, QString Delimiter, QStringList header, int skipLines){
        filename =  fileName;
        h        =  header;
        skip     =  skipLines;
        delimiter=  Delimiter;
    }
    //    void setOutputParams(QString outfile, QString treename){
    //    outFileName = outfile;
    //    treeName    = treename;
    //    }
    QList <QStringList> GetData(){return data;}
    int GetRowCount(){return nRows;}
    int GetColumnCount(){return nCols;}
    QStringList GetHeader(){return h;}

    void StopNow(){stopNow=true;}
signals:
    void Started(int fileNumber);
    void Stopped(int err);
    void Status(QString status);
    void Progress(int progress);
    void SetTableData(int row, int col, QString data);


private:
    void run();
    //    int GetTypeCount(QString type);
    QString filename;
    QStringList h;
    QString delimiter;
    int skip;
    //    QString outFileName;
    //    QString treeName;
    bool stopNow;
    QList <QStringList> data;
    int nCols;
    int nRows;


};

#endif // IROOTWORKSHEETIMPORTER_H
