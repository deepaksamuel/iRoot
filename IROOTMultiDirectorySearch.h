#ifndef IROOTMULTIDIRECTORYSEARCH_H
#define IROOTMULTIDIRECTORYSEARCH_H

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QListView>
#include <QTreeView>
#include <QMouseEvent>

namespace Ui {
class IROOTMultiDirectorySearch;
}

class IROOTMultiDirectorySearch : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTMultiDirectorySearch(QWidget *parent = 0);
    QStringList GetFiles(QStringList partfileNames, QStringList directoryList);
    QStringList GetDirectories(QStringList partDirNames, QStringList dirNames);
    ~IROOTMultiDirectorySearch();



private slots:
    void on_close_clicked();

    void on_openMultipleDirectories_clicked();

signals:
    void OpenFiles(QStringList fileNames);
private:
    Ui::IROOTMultiDirectorySearch *ui;
    QPoint          mousePressOrigin;
    void            mousePressEvent(QMouseEvent *e){ mousePressOrigin = e->globalPos();}
    void            mouseMoveEvent(QMouseEvent *e){
        if(e->buttons() & Qt::LeftButton ){
            move(pos()-(mousePressOrigin-e->globalPos()));
            mousePressOrigin = e->globalPos();
        }
    }
};

#endif // IROOTMULTIDIRECTORYSEARCH_H
