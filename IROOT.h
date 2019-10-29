#ifndef IROOT_H
#define IROOT_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QTableWidget>
#include <QCompleter>
#include <QColorDialog>
#include <QToolTip>
#include <QMenu>
#include <QAction>
#include <QStringListModel>
#include <QScrollBar>
#include <QClipboard>
#include <QApplication>
#include <QSettings>
#include <QTimer>
#include <QUndoStack>
#include <QSplashScreen>



#include <iCanvas.h>
#include <IROOTMessageBox.h>
#include <IROOTPlotOptionsEntry.h>
#include <IROOTAxisEditor.h>

#include <IROOTFitOptions.h>
#include <IROOTFittingObjects.h>
#include <IROOTLoopModule.h>
#include <IROOTMultiDirectorySearch.h>
#include <IROOTAsciiFileConvertor.h>
#include <IROOTHistogramBinning.h>
#include <IROOTWorksheetImporter.h>
#include <IROOTTree.h>
#include <IROOTWorksheet.h>
#include <IROOTCompiler.h>
#include <IROOTTreeFormulaChecker.h>
#include <IROOTAboutBox.h>
#include <IROOTBinaryFileConvertor.h>

#include <TApplication.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TKey.h>
#include <TFriendElement.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TPave.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <TMarker.h>
#include <THStack.h>
#include <TText.h>


#define MAXRECENTFILESLIST 15

namespace Ui {
class IROOT;
}
//class IROOTFitResultsTable;
class IROOTWorksheet;

class IROOT : public QMainWindow
{
    Q_OBJECT

public:
    explicit IROOT(QWidget *parent = 0);
    TCanvas*            AddCanvas(QString title="", int x=0,int y=0);

    int                 GetNumberOfFiles(bool selectedOnly=false);
    QStringList         GetFileList();
    void                SetCurrentFile(int index);
    QList <TTree*>      GetTree(TFile *f);
    //    QStringList         ListBranches(TTree *tree);
    //    QStringList         GetListofLeavesinTree(TTree *tree);
    void                UpdateBranches(QStringList branches);
    int                 CountPads();
    void                NextPad();
    QTableWidget*       CreateNewWorksheet(int rows, int cols, QString title);
    QTableView*         CreateNewTableView(IROOTWorksheet *model, QString title);
    int                 plot(QString draw, QString cut, QString style, int loop, int type, bool refreshObjects, bool same, bool autoFit=false);

    void                ShowMessage(QString messageText, QString type);
    void                ShowMessages(int milliseconds);
    QString             GetPlotString();
    QString             GetCutString();
    void                LoopPlot(int type, int begin, int end, int increment, int padincrement);
    void                LoopPlotOverIndex(int begin, int end, int increment, int padincrement);
    void                LoopPlotOverFiles(bool selectedOnly, int loop, int padIncrement);
    void                Fit(QString opt="", int loop=0, bool statsOnPad=false);
    void                UpdateRecentFiles(QStringList recentFiles);
    QTableWidget*       GetCurrentWorksheet();
    IROOTMessageBox     *message;
    QTableView*         CreateTreeViewer();
    void                SaveCanvas();
    void                SaveWorksheet();
    void                CreateCompilerWindow();
    void                RemoveDlls(); // function to remove ACLIC dlls which are remaining.
    static TFile               *currentFile;
    static TTree               *currentTree;

    ~IROOT();

private slots:

    void tabChanged(int tabN);

    void tabClose(int tabN);
    void windowActivated(QMdiSubWindow* window);

    void on_backtoOpeningPage_clicked();

    void on_openFiles_clicked();

    void on_filesWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_branchWidget_itemClicked(QTreeWidgetItem *item, int column);


    void on_addNewCanvas_clicked();

    void on_canvasRows_valueChanged(int arg1);


    void on_canvasColumns_valueChanged(int arg1);

    void on_nextPad_clicked();

    void on_prevPad_clicked();

    void on_addWorksheet_clicked();


    void on_plot_clicked();


    void on_gridX_clicked();

    void on_gridY_clicked();

    void on_logX_clicked();

    void on_logY_clicked();

    void on_plotTemplate_currentIndexChanged(const QString &arg1);

    void on_openRootFile_clicked();


    void on_templateOptions_itemClicked(QTreeWidgetItem *item, int column);

    void on_viewFiles_clicked();

    void on_viewTemplates_clicked();

    void on_viewSetings_clicked();

    void on_fit_clicked();

    void SetFitOption(QList <QTreeWidgetItem*>);

    void on_chooseFitOptions_clicked();

    void on_moreObjects_clicked();

    void on_plotFunction_clicked();

    void on_loop_clicked();

    void on_fontcomboBox_currentIndexChanged(const QString &arg1);

    void on_fontSize_valueChanged(double arg1);

    void on_fontColor_clicked();

    void fontColorSelected(QColor);

    void on_bold_clicked();

    void on_italic_clicked();

    void on_openMultipleRootFile_clicked();

    void OpenFiles(QStringList fileNames);
    void OpenFilesAsChain(QStringList fileNames);
    void on_convertAsciitoTree_clicked();
    void fitObjectsSelected(QStringList list);


    void on_histogramBinning_clicked();
    void setCurrentBinning(HistBin b){currentBinning =b;}

    void on_ImportAscii_clicked();
    void CreateTable();
    //void SetTableData(int row, int column, QString data);

    void on_plotOptions_clicked();
    void closeHiddenWidgets();

    void on_plotString_textChanged(const QString &arg1);
    void setDrawStringFromCompleter(QString completer);
    void setCutStringFromCompleter(QString completer);
    void setFilterStringFromCompleter(QString completer);

    void on_cutString_textChanged(const QString &arg1);

    void on_chainFiles_clicked();

    void on_clear_files_clicked();

    void on_filterEntries_clicked();

    void on_filterString_textChanged(const QString &arg1);

    void on_clearFitData_clicked();

    void saveSettings();

    void loadSettings();

    void on_recentFiles_itemClicked(QTreeWidgetItem *item, int column);

    void on_save_clicked();
    void AppendMessage(QString messageText, QString type);
    void SetUniqueLineColor();
    void SetUniqueMarkerColor();
    void SetUniqueMarkerStyle();

    //void on_createLegends_clicked();
    void CreateLineLegend();
    void CreateMarkerLegend();
    void CreateLineMarkerLegend();

    void on_gotoWorkspace_clicked();

    void on_viewCompiler_clicked();

    void on_about_clicked();

    void on_insertGraphicalCut_clicked();

    void on_insertArrow_clicked();

    void on_insertArc_clicked();

    void on_insertEllipse_clicked();

    void on_insertLine_clicked();

    void on_insertMarker_clicked();

    void on_insertText_clicked();

    void on_resetInsertMode_clicked();

    void on_convertbinarytoTree_clicked();

signals:
    void currentTreeChanged(TTree* currTree, QStringList branches);

private:
    Ui::IROOT *ui;
    QSize mainSize;// store the size of the maximized window
    TApplication        *iRootApp;
    void                InitializeRoot();
    void                InitializeVariables();
    void                CreateWidgets();
    int                 CheckTreeString(QString s, QString c, int type=0); // type =1 filter string
    void                closeEvent(QCloseEvent *);
    bool                isClearingFiles;
    bool                closePreviousFile;
    bool                isChain; // flag to denote if files are chained

    IROOTFitOptions     *fitOptions;
    IROOTMultiDirectorySearch   *multiDirSearch;
    IROOTAsciiFileConvertor     *asciiConvertor;
    IROOTBinaryFileConvertor    *binaryConvertor;
    IROOTWorksheetImporter      *worksheetImporter;
    IROOTHistogramBinning       *histogramBinning;
    IROOTPlotOptionsEntry       *plotOptionsEntry;
    QColorDialog                *colorDialog;
    HistBin                     currentBinning;
    IROOTWorksheet              *IROOTTreeModel;
    IROOTWorksheet              *fitResultsTable;
    QTimer                      *closeTimer;
    QCompleter                  *drawStringCompleter;
    QCompleter                  *cutStringCompleter;
    QCompleter                  *filterStringCompleter;

    QAction                     *uniqueLineColor;
    QAction                     *uniqueMarkerColor;
    QAction                     *uniqueMarkerStyle;

    QAction                     *lineLegend;
    QAction                     *markerLegend;
    QAction                     *linemarkerLegend;
};



#endif // IROOT_H

