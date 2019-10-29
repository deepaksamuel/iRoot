#include "IROOTMultiDirectorySearch.h"
#include "ui_IROOTMultiDirectorySearch.h"

IROOTMultiDirectorySearch::IROOTMultiDirectorySearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IROOTMultiDirectorySearch)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

IROOTMultiDirectorySearch::~IROOTMultiDirectorySearch()
{
    delete ui;
}

void IROOTMultiDirectorySearch::on_close_clicked()
{
    hide();
}

QStringList IROOTMultiDirectorySearch::GetFiles(QStringList partfileNames, QStringList directoryList)
{
    QStringList fileList;
    for(int ii=0;ii<directoryList.count();ii++){
        QFileInfoList fl = QDir(directoryList.at(ii)).entryInfoList();
        for(int jj=0;jj<fl.count();jj++){
            for(int kk=0;kk<partfileNames.count();kk++){
                if(fl.at(jj).fileName().contains(partfileNames.at(kk))){
                    fileList.append(fl.at(jj).filePath());
                    break;
                }
            }
        }
    }
    return fileList;
}

QStringList IROOTMultiDirectorySearch::GetDirectories(QStringList partDirNames, QStringList dirNames)
{
    if(partDirNames.count()<1)
        return dirNames;
    QStringList dirList;
    for(int ii=0;ii<dirNames.count();ii++){
        for(int jj=0;jj<partDirNames.count();jj++){
            if(dirNames.at(ii).contains(partDirNames.at(jj))){
                dirList.append(dirNames.at(ii));
                break;
            }
        }
    }
    return dirList;
}

void IROOTMultiDirectorySearch::on_openMultipleDirectories_clicked()
{

    QFileDialog *fD = new QFileDialog();
    fD->setFileMode(QFileDialog::DirectoryOnly);
    fD->setOption(QFileDialog::DontUseNativeDialog,true);
    QListView *l = fD->findChild<QListView*>("listView");
    if (l) {
        l->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    QTreeView *t = fD->findChild<QTreeView*>();
    if (t) {
        t->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    if ( fD->exec()){
        QStringList  dirNames = fD->selectedFiles();
        close();
        if(dirNames.count()<1) return;
        if(ui->onlySelectedDirectories->isChecked()){
            OpenFiles(GetFiles(ui->partFileNames->text().split(QRegExp("\\s+"), QString::SkipEmptyParts),
                               GetDirectories(ui->partDirNames->text().split(QRegExp("\\W+"), QString::SkipEmptyParts),dirNames))
                      );
        }
        else
            OpenFiles(GetFiles(ui->partFileNames->text().split(QRegExp("\\s+"), QString::SkipEmptyParts),dirNames));
    }

}
