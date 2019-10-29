#include "IROOT.h"
#include "ui_IROOT.h"

IROOTAxisEditor     *axisEditor;
IROOTFittingObjects *fitObjects;
IROOTLoopModule     *loopModule;

static int tableN =1;



TFile* IROOT::currentFile = 0;
TTree* IROOT::currentTree = 0;



IROOT::IROOT(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IROOT)
{

    ui->setupUi(this);
    RemoveDlls();
    InitializeVariables();
    InitializeRoot();
    CreateWidgets();
    showMaximized();
    ui->mainStackedWidget->setCurrentWidget(ui->openingPage);
    ui->filesWidget->setHeaderHidden(false);
    loadSettings();


}

IROOT::~IROOT()
{

    delete ui;
    //    delete loopModule;
    //    message->close();
    //    fitObjects->close();
    //    axisEditor->close();
    //    loopModule->close();

}

// this function is required because windows sometime does not allow to delete the dlls when in use. We delete on startup.
void IROOT::RemoveDlls()
{
    QStringList wildCards;
    wildCards<<"irootcode*";
    QFileInfoList l= QDir::current().entryInfoList(wildCards,QDir::Files);
    for(int ii=0;ii<l.count();ii++){
        qDebug()<<l.at(ii).fileName();
        QFile f(l.at(ii).fileName());
        if(f.exists())
            f.remove();
    }
}

/*
 *Intializes ROOT Variables
 */
void IROOT::closeEvent(QCloseEvent *)
{
    saveSettings();
    delete colorDialog;
    delete message;
    delete axisEditor;
    delete fitOptions;
    delete fitObjects;
    delete loopModule;
    delete histogramBinning;
    delete worksheetImporter;
    delete multiDirSearch;
    delete asciiConvertor;
    delete plotOptionsEntry;
    delete binaryConvertor;
}


void IROOT::InitializeRoot()
{
    iRootApp = new TApplication("iRootApp",0,0);
    iRootApp->SetReturnFromRun(true);  // tell application to return from run
    // set up our own style
    gStyle->SetCanvasColor(kWhite);     // background is no longer mouse-dropping white
    gStyle->SetCanvasBorderMode(0);     // turn off canvas borders
    // gStyle->SetPadBorderMode(0);

    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameLineWidth(0.2);
    gStyle->SetTitleFillColor(0);
    gStyle->SetStatColor(0);

    //iRoot Default Styles
    gStyle->SetStatFont(2+(13)*10); // Times New Roman Regular
    gStyle->SetStatFontSize(0.03);
    gStyle->SetStatTextColor(1); //Red Color

    gStyle->SetTitleTextColor(1);
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleFont(2+(13)*10,"t");

    gStyle->SetTitleFont(2+(13)*10,"xyz");
    gStyle->SetTitleSize(0.03,"xyz");
    gStyle->SetTitleColor(1,"xyz");

    gStyle->SetLabelFont(2+(13)*10,"xyz");
    gStyle->SetLabelSize(0.03,"xyz");
    gStyle->SetLabelColor(1,"xyz");

    gStyle->SetMarkerStyle(2);
    gStyle->SetMarkerSize(1);
    gStyle->SetMarkerColor(4);



    gStyle->SetHistFillColor(38);
    gStyle->SetHistLineStyle(0);
    gStyle->SetHistLineColor(1);


    gROOT->SetStyle("Pub");
    ui->plotTemplate->setCurrentIndex(4);
    gROOT->ForceStyle();
    gStyle->UseCurrentStyle();

    //  on_addNewCanvas_clicked();
}
// intitialize variables
void IROOT::InitializeVariables()
{

    currentTree         =0;
    isChain             =false;

    QStringList generalOptions;
    QStringList histogramOptions12;
    QStringList histogramOptions1;
    QStringList histogramOptions2;
    QStringList histogramOptions3;
    QStringList graphOptions;


    generalOptions<<"Superimpose on previous plot |SAME"
                 <<"Draw X axis on the top side |X+"
                <<"Draw Y axis on the left side |Y+"
               <<"Use the graphical cut |[cutg]";


    histogramOptions12<<"Draw error bars |E"
                     <<"Lego plot with hidden line removal |LEGO1"
                    <<"Lego plot with hidden surfacee removal |LEGO2"
                   <<"Lego plot with hidden line removal without borders |LEGO2"
                  <<"Draw bin contents as text |TEXT"
                 <<"Draw bin contents as text tilted by 45 degrees |TEXT45";


    histogramOptions1<<"Draw without axis |AH"
                    <<"Bar chart  |B"
                   <<"3D bar chart |BAR"
                  <<"3D horizontal bars |HBAR"
                 <<"Draw smooth curve through bins |C"
                <<"Draw error bars for bins with 0 contents |E0"
               <<"Draw error bars with perpendicular lines at edges |E1"
              <<"Draw error bars with rectangles |E2"
             <<"Draw fill area through end points of vertical error bars |E3"
            <<"Draw smooth fill area through end points of error bars |E4"
           <<"Draw fill area through end points of vertical error bars (ignore empty bins) |E5"
          <<"Draw smooth fill area through end points of error bars (ignore empty bins) |E6"
         <<"Draw a line through the bins |L"
        <<"Draw marker at each non-empty bins |P"
       <<"Draw marker at all bins |P0"
      <<"Pie chart |PIE"
     <<"Use high resolution mode |9";

    histogramOptions2<<"Arrow mode, shows gradient between adjancent cells |ARR"
                    <<"Draw a box for each cell, size proportional to contents absoulte value |BOX"
                   <<"Draw a 3D box for each cell, size proportional to contents absoulte value |BOX"
                  <<"Draw a box for each cell with color scale varying with contents |COL"
                 <<"Draw a box for each cell with color scale varying with contents, show palette |COLZ"
                <<"Contour plot with surface colors |CONT0"
               <<"Contour plot with different line styles for contours|CONT1"
              <<"Contour plot with same line styles for contours |CONT2"
             <<"Contour plot with fill area colors |CONT3"
            <<"Contour plot with surface colors |CONT4"
           <<"Contour plot with delaunay triangles |CONT5"
          <<"Cylindrical coordinates |CYL"
         <<"Polar coordinates |POL"
        <<"Spherical coordinates |SPH"
       <<"Pseudorapidity/Phi coordinates |PSR"
      <<"Surface plot with hidden line removal |SURF"
     <<"Surface plot with hidden surface removal |SURF1"
    <<"Surface plot with colors for cell contents |SURF2"
    <<"Surface plot with contour view |SURF3"
    <<"Surface plot with Gouraud shading |SURF4"
    <<"Surface plot with only contours (use with CYL, SPH, PSR) |SURF5";


    histogramOptions3<<"Draw a Gouraud shaded 3d iso surface |ISO"
                    <<"Draw a box with volume proportional to absolute value of cell |BOX";

    graphOptions<<"Draw axis around the graph |A"
               <<"Polyline |L"
              <<"Fill area |F"
             <<"Smooth curve |C"
            <<"Draw a * at each point |*"
           <<"Use current marker |P"
          <<"Bar chart |B";


    QTreeWidgetItem *generalOptionsItem = new QTreeWidgetItem();
    generalOptionsItem->setText(0,"General options");
    QList <QTreeWidgetItem*> generalOptionsItemChildren;
    for(int ii=0; ii<generalOptions.count();ii++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,generalOptions.at(ii));
        generalOptionsItemChildren.append(item);
    }
    generalOptionsItem->addChildren(generalOptionsItemChildren);
    ui->templateOptions->addTopLevelItem(generalOptionsItem);


    QTreeWidgetItem *histogramOptions12Item = new QTreeWidgetItem();
    histogramOptions12Item->setText(0,"1D and 2D histogram options");
    QList <QTreeWidgetItem*> histogramOptions12ItemChildren;
    for(int ii=0; ii<histogramOptions12.count();ii++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,histogramOptions12.at(ii));
        histogramOptions12ItemChildren.append(item);
    }
    histogramOptions12Item->addChildren(histogramOptions12ItemChildren);
    ui->templateOptions->addTopLevelItem(histogramOptions12Item);


    QTreeWidgetItem *histogramOptions1Item = new QTreeWidgetItem();
    histogramOptions1Item->setText(0,"1D histogram options");
    QList <QTreeWidgetItem*> histogramOptions1ItemChildren;
    for(int ii=0; ii<histogramOptions1.count();ii++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,histogramOptions1.at(ii));
        histogramOptions1ItemChildren.append(item);
    }
    histogramOptions1Item->addChildren(histogramOptions1ItemChildren);
    ui->templateOptions->addTopLevelItem(histogramOptions1Item);


    QTreeWidgetItem *histogramOptions2Item = new QTreeWidgetItem();
    histogramOptions2Item->setText(0,"2D histogram options");
    QList <QTreeWidgetItem*> histogramOptions2ItemChildren;
    for(int ii=0; ii<histogramOptions2.count();ii++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,histogramOptions2.at(ii));
        histogramOptions2ItemChildren.append(item);
    }
    histogramOptions2Item->addChildren(histogramOptions2ItemChildren);
    ui->templateOptions->addTopLevelItem(histogramOptions2Item);


    QTreeWidgetItem *histogramOptions3Item = new QTreeWidgetItem();
    histogramOptions3Item->setText(0,"3D histogram options");
    QList <QTreeWidgetItem*> histogramOptions3ItemChildren;
    for(int ii=0; ii<histogramOptions3.count();ii++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,histogramOptions3.at(ii));
        histogramOptions3ItemChildren.append(item);
    }
    histogramOptions3Item->addChildren(histogramOptions3ItemChildren);
    ui->templateOptions->addTopLevelItem(histogramOptions3Item);

    QTreeWidgetItem *graphOptionsItem = new QTreeWidgetItem();
    graphOptionsItem->setText(0,"Graph options");
    QList <QTreeWidgetItem*> graphOptionsItemChildren;
    for(int ii=0; ii<graphOptions.count();ii++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,graphOptions.at(ii));
        graphOptionsItemChildren.append(item);
    }
    graphOptionsItem->addChildren(graphOptionsItemChildren);
    ui->templateOptions->addTopLevelItem(graphOptionsItem);

    fitResultsTable = new IROOTWorksheet(2);
    fitResultsTable->SetTable(ui->fitResultsTable);
    connect(fitResultsTable,SIGNAL(Message(QString,QString)),this,SLOT(AppendMessage(QString,QString)));
    connect(fitResultsTable,SIGNAL(CreatePad()),this,SLOT(on_addNewCanvas_clicked()));

    drawStringCompleter     =new QCompleter();
    cutStringCompleter      =new QCompleter();
    filterStringCompleter   =new QCompleter();
    ui->plotString->setCompleter(0);
    ui->cutString->setCompleter(0);

    drawStringCompleter->setCompletionMode(QCompleter::PopupCompletion);
    drawStringCompleter->setCaseSensitivity(Qt::CaseInsensitive);

    cutStringCompleter->setCompletionMode(QCompleter::PopupCompletion);
    cutStringCompleter->setCaseSensitivity(Qt::CaseInsensitive);


    filterStringCompleter->setCompletionMode(QCompleter::PopupCompletion);
    filterStringCompleter->setCaseSensitivity(Qt::CaseInsensitive);

    connect(drawStringCompleter,SIGNAL(activated(QString)),this,SLOT(setDrawStringFromCompleter(QString)));
    connect(cutStringCompleter,SIGNAL(activated(QString)),this,SLOT(setCutStringFromCompleter(QString)));
    connect(filterStringCompleter,SIGNAL(activated(QString)),this,SLOT(setFilterStringFromCompleter(QString)));

    uniqueLineColor     = new QAction("Line color",this);
    uniqueMarkerColor   = new QAction("Marker color",this);
    uniqueMarkerStyle   = new QAction("Marker style",this);

    ui->unique->addAction(uniqueLineColor);
    ui->unique->addAction(uniqueMarkerColor);
    ui->unique->addAction(uniqueMarkerStyle);

    connect(uniqueLineColor,SIGNAL(triggered()),this,SLOT(SetUniqueLineColor()));
    connect(uniqueMarkerColor,SIGNAL(triggered()),this,SLOT(SetUniqueMarkerColor()));
    connect(uniqueMarkerStyle,SIGNAL(triggered()),this,SLOT(SetUniqueMarkerStyle()));

    lineLegend          =new QAction("Lines",this);
    markerLegend        =new QAction("Markers",this);
    linemarkerLegend    =new QAction("Line/Markers",this);

    ui->createLegends->addAction(lineLegend);
    ui->createLegends->addAction(markerLegend);
    ui->createLegends->addAction(linemarkerLegend);

    connect(lineLegend,SIGNAL(triggered()),this,SLOT(CreateLineLegend()));
    connect(markerLegend,SIGNAL(triggered()),this,SLOT(CreateMarkerLegend()));
    connect(linemarkerLegend,SIGNAL(triggered()),this,SLOT(CreateLineMarkerLegend()));

    //    ui->unique->setStyleSheet("background-color: black; color: white; font: 7pt \"Segoe UI\"");
    //    ui->createLegends->setStyleSheet("background-color: black; color: white; font: 7pt \"Segoe UI\"");



}

void IROOT::setDrawStringFromCompleter(QString completer)
{
    QStringList l = ui->plotString->currentText().split(":");
    l.removeLast();
    QString merged;
    merged= l.join(":").append(":").append(completer);
    if(!ui->plotString->currentText().contains(":"))// first element
        merged =completer;

    ui->plotString->insertItem(0,merged);
    ui->plotString->setCurrentIndex(0);
}

void IROOT::setCutStringFromCompleter(QString completer)
{
    QString currentString  = ui->cutString->currentText();
    int size=  cutStringCompleter->completionPrefix().size();
    //    currentString.replace("&&","::");
    //    currentString.replace("||","::");
    //    currentString.replace("==","::");
    currentString.remove(currentString.size()-size,size);
    currentString.append(completer);

    //    QStringList l = currentString.split("::");
    //    l.removeLast();
    //    QString merged;
    //    merged= l.join(":").append(":").append(completer);
    //    //    if(!ui->plotString->currentText().contains(":"))// first element
    //    //        merged =completer;
    //    QString merged = ui->cutString->currentText().append(completer);
    ui->cutString->insertItem(0,currentString);
    ui->cutString->setCurrentIndex(0);
}

void IROOT::setFilterStringFromCompleter(QString completer)
{
    QString currentString  = ui->filterString->text();
    int size=  filterStringCompleter->completionPrefix().size();
    //    currentString.replace("&&","::");
    //    currentString.replace("||","::");
    //    currentString.replace("==","::");
    currentString.remove(currentString.size()-size,size);
    currentString.append(completer);

    //    QStringList l = currentString.split("::");
    //    l.removeLast();
    //    QString merged;
    //    merged= l.join(":").append(":").append(completer);
    //    //    if(!ui->plotString->currentText().contains(":"))// first element
    //    //        merged =completer;
    //    QString merged = ui->cutString->currentText().append(completer);
    ui->filterString->setText(currentString);

}

void IROOT::CreateWidgets()
{
    message     = new IROOTMessageBox();
    axisEditor  = new IROOTAxisEditor();
    fitOptions  = new IROOTFitOptions();
    fitObjects  = new IROOTFittingObjects();
    loopModule  = new IROOTLoopModule();
    histogramBinning    = new IROOTHistogramBinning();
    worksheetImporter   = new IROOTWorksheetImporter();
    multiDirSearch  = new IROOTMultiDirectorySearch();
    asciiConvertor  = new IROOTAsciiFileConvertor();
    plotOptionsEntry = new IROOTPlotOptionsEntry();
    binaryConvertor = new IROOTBinaryFileConvertor();
    connect(binaryConvertor,SIGNAL(openFiles(QStringList)),this,SLOT(OpenFiles(QStringList)));

    connect(multiDirSearch,SIGNAL(OpenFiles(QStringList)),this,SLOT(OpenFiles(QStringList)));

    connect(asciiConvertor,SIGNAL(rootFilesReady(QStringList)),this,SLOT(OpenFiles(QStringList)));


    connect(plotOptionsEntry,SIGNAL(plotOptionsChanged(QString)),ui->styleString,SLOT(setText(QString)));

    connect(ui->styleString,SIGNAL(textChanged(QString)),plotOptionsEntry,SLOT(setText(QString)));

    connect(worksheetImporter->GetImportThread(),SIGNAL(Stopped(int)),this,SLOT(CreateTable()));
    //    ui->frontPageWidgets->addWidget(asciiConvertor);
    //    ui->frontPageWidgets->setCurrentIndex(2);

    colorDialog = new QColorDialog;
    colorDialog->setOption(QColorDialog::NoButtons);

    connect(fitOptions,SIGNAL(selectionChanged(QList<QTreeWidgetItem*>)),this,SLOT(SetFitOption(QList<QTreeWidgetItem*>)));
    connect(colorDialog,SIGNAL(currentColorChanged(QColor)),this,SLOT(fontColorSelected(QColor)));
    // connect(ui->mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(windowActivated(QMdiSubWindow*)));
    QList <QTabBar*> tabList = ui->mdiArea->findChildren<QTabBar*>();
    //tabList.at(0)->setTabsClosable("true");
    connect(tabList.at(tabList.count()-1), SIGNAL(currentChanged(int)),this, SLOT(tabChanged(int)),Qt::AutoConnection);
    connect(tabList.at(tabList.count()-1), SIGNAL(tabCloseRequested(int)),this, SLOT(tabChanged(int)),Qt::AutoConnection);

    connect(fitObjects,SIGNAL(itemsSelected(QStringList)),this,SLOT(fitObjectsSelected(QStringList)));

    ui->fitResultsTable->setModel(fitResultsTable);
    ui->fitResultsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->fitResultsTable, SIGNAL(customContextMenuRequested(const QPoint &)),fitResultsTable, SLOT(showMenu(QPoint)));

    connect(histogramBinning,SIGNAL(binningChanged(HistBin)),this,SLOT(setCurrentBinning(HistBin)));
    currentBinning = histogramBinning->GetCurrentBinning();

    ui->fit->setDisabled(true);
    IROOTTreeModel = new IROOTWorksheet(1);
    connect(this,SIGNAL(currentTreeChanged(TTree*, QStringList)),IROOTTreeModel,SLOT(SetCurrentTree(TTree*, QStringList)));

    CreateTreeViewer();

    closeTimer  =new QTimer();
    closeTimer->setInterval(250);
    closeTimer->start();
    connect(closeTimer,SIGNAL(timeout()),this,SLOT(closeHiddenWidgets()));
}


void IROOT::closeHiddenWidgets()
{
    if(!asciiConvertor->isActiveWindow()){
        if(!asciiConvertor->isEditing())
            asciiConvertor->hide();
    }
    if(!axisEditor->isActiveWindow())
        axisEditor->hide();
    if(!fitOptions->isActiveWindow())
        fitOptions->hide();
    if(!fitObjects->isActiveWindow())
        fitObjects->hide();
    if(!histogramBinning->isActiveWindow())
        histogramBinning->hide();
    if(!loopModule->isActiveWindow())
        loopModule->hide();
    if(!multiDirSearch->isActiveWindow())
        multiDirSearch->hide();
    if(!plotOptionsEntry->isActiveWindow())
        plotOptionsEntry->hide();
    if(!worksheetImporter->isActiveWindow())
        worksheetImporter->hide();

    //    axisEditor  = new IROOTAxisEditor();
    //    fitModule   = new IROOTFittingModule();
    //    fitOptions  = new IROOTFitOptions();
    //    fitObjects  = new IROOTFittingObjects();
    //    loopModule  = new IROOTLoopModule();
    //    histogramBinning    = new IROOTHistogramBinning();
    //    worksheetImporter   = new IROOTWorksheetImporter();
    //    multiDirSearch  = new IROOTMultiDirectorySearch();
    //    asciiConvertor  = new IROOTAsciiFileConvertor();
    //    plotOptionsEntry = new IROOTPlotOptionsEntry();

}
void IROOT::tabChanged(int tabN)
{
    QMdiSubWindow *window = ui->mdiArea->currentSubWindow();
    if(window!=NULL){
        //qDebug()<<window->widget()->metaObject()->className();
        if(window->widget()->metaObject()->className()==QString("iCanvas")){ // canvas
            ((iCanvas*)(window->widget()))->GetCanvas()->cd(1);
            fitObjects->Update();
        }

    }
}


void IROOT::tabClose(int tabN)
{
    //    QMdiSubWindow *window = ui->mdiArea->currentSubWindow();
    //    if(window!=NULL){
    //        //qDebug()<<window->widget()->metaObject()->className();
    //        if(window->windowTitle()==QString("IROOT Tree Viewer (Read Only)")){ // canvas
    //            ShowMessage("This window cannot be closed","ERR");
    //        }

    //    }
}

void QMdiSubWindow::closeEvent(QCloseEvent *closeEvent)
{
    if(windowTitle()==QString("IROOT Tree Viewer (Read only)")){ // canvas
        closeEvent->ignore();
    }


    else closeEvent->accept();



}
void IROOT::windowActivated(QMdiSubWindow *window)
{
    if(window!=NULL){
        //qDebug()<<window->widget()->metaObject()->className();
        if(window->widget()->metaObject()->className()==QString("iCanvas")){
            ((iCanvas*)(window->widget()))->GetCanvas()->cd(1);
        }
    }
}

void IROOT::on_backtoOpeningPage_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->openingPage);

}

//On pressing the open button
void IROOT::on_openFiles_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,"Open ROOT Files", "","(*.root);;All files (*.*)");
    OpenFiles(filenames);
    ui->mainStackedWidget->setCurrentWidget(ui->workingPage);

}

/*TCanvas *iRoot::AddCanvas(QString title, int x,int y)
 *Adds a new canvas in the mdi area and dividing it x by y
 *if x and y are 0 then the values from the ui are taken
 */
TCanvas *IROOT::AddCanvas(QString title, int x,int y)// adds a new canvas window divided by x rows and y columns
{
    setCursor(Qt::BusyCursor);
    QMdiSubWindow *subWindow = new QMdiSubWindow();
    iCanvas *widget = new iCanvas(axisEditor,fitObjects, subWindow);
    subWindow->setWidget(widget);
    ui->mdiArea->addSubWindow(subWindow);
    subWindow->showMaximized();
    if(title==""){
        subWindow->setWindowTitle(QString("Canvas %1").arg(iCanvas::canvasN));
    }
    else
        subWindow->setWindowTitle(title);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->activateWindow();
    subWindow->setOption (QMdiSubWindow::RubberBandResize);
    ui->mdiArea->setActiveSubWindow(subWindow);
    setCursor(Qt::ArrowCursor);
    if(x<1)
        x=ui->canvasRows->value();
    if(y<1)
        y=ui->canvasColumns->value();

    widget->GetCanvas()->Divide(x,y);
    widget->GetCanvas()->cd(1);
    gPad->SetLogx(ui->logX->isChecked());
    gPad->SetLogy(ui->logY->isChecked());
    gPad->SetGridx(ui->gridX->isChecked());
    gPad->SetGridy(ui->gridY->isChecked());
    widget->GetCanvas()->Update();
    gPad->Clear();

    return(widget->GetCanvas());

}

void IROOT::OpenFiles(QStringList fileNames)
{

    int nFiles=0;
    isClearingFiles=false;// reset the flag, if set by clear files slot
    for(int ii=0;ii<fileNames.count();ii++){
        QString file= fileNames.at(ii);
        TFile *f = new TFile(file.toLatin1().data(),"READ");
        if(f!=NULL){
            QList <TTree*> l= GetTree(f);
            if(l.count()>1)
                AppendMessage("This file has more than 1 tree, opening as different files","INFO");
            for(int jj=0;jj<l.count();jj++){
                TTree *tree = l.at(jj);
                if(tree!=NULL){
                    QTreeWidgetItem *newfile = new QTreeWidgetItem(ui->filesWidget);
                    newfile->setText(0, f->GetFile()->GetName());
                    newfile->setText(1, QString("%1").arg(tree->GetEntriesFast()));
                    newfile->setText(2,tree->GetName());
                    newfile->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
                    ui->filesWidget->resizeColumnToContents(0);
                    ui->filesWidget->resizeColumnToContents(1);
                    newfile->setCheckState(0,Qt::Unchecked);
                    //GetListofRootPlots(f);
                    nFiles++;
                    //delete f; !!! wrong if more than 1 tree is present
                }
            }
            //            if(GetListofRootPlots(f)>0 && l.count()==0){ // check for plots
            //                mainWindow->ShowMessage(QString("No trees found in: %1").arg(file),"WARN");
            //                QTreeWidgetItem *newfile = new QTreeWidgetItem(ui->filesWidget);
            //                newfile->setText(0, f->GetFile()->GetName());
            //                newfile->setText(1, "0");
            //                newfile->setText(2,"");
            //                newfile->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            //                ui->filesWidget->resizeColumnToContents(0);
            //                ui->filesWidget->resizeColumnToContents(1);
            //                newfile->setCheckState(0,Qt::Unchecked);
            //                nFiles++;
            //                //delete f;
            //            }


            delete f;
        }
        else{
            AppendMessage(QString("Error opening file %1").arg(file),"ERR");
        }
    }

    //    if(nFiles>0)
    //        spreadSheetEntryN=0; // reset
    if(ui->filesWidget->currentItem()==NULL)
        SetCurrentFile(0);
    ui->mainStackedWidget->setCurrentWidget(ui->workingPage);
    ui->leftStackWidget->setCurrentWidget(ui->filesPage);
    if(nFiles>0)
        AppendMessage(QString("%1 file(s) opened...").arg(nFiles),"INFO");
    isChain=false;

}

void IROOT::OpenFilesAsChain(QStringList filenames)
{
    if(filenames.count()>0){
        TChain *chain=0;
        isChain=false;
        bool chainCreated=false;
        //QString treeName=set->GetChainTreeName();
        QString treeName="";
        for(int ii=0;ii<filenames.count();ii++){
            QString file= filenames.at(ii);
            TFile *f = new TFile(file.toLatin1().data(),"READ");
            if(f!=NULL){
                TTree *tree = GetTree(f).at(0);
                if(tree==0){
                    ShowMessage(QString("Error: No trees found in the file: %1").arg(file),"ERR");
                }
                else{
                    if(treeName=="")
                        treeName=tree->GetName();
                    if(treeName==tree->GetName()){
                        if(chainCreated==false){
                            ShowMessage(QString("Creating TChain with tree name: %1").arg(treeName),"INFO");
                            chain = new TChain(treeName.toLatin1().data());
                            chainCreated=true;
                        }
                        QTreeWidgetItem *newfile = new QTreeWidgetItem(ui->filesWidget);
                        newfile->setText(0, f->GetFile()->GetName());
                        newfile->setText(1, QString("%1").arg(tree->GetEntriesFast()));
                        newfile->setText(2,tree->GetName());
                        newfile->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
                        ui->filesWidget->resizeColumnToContents(0);
                        ui->filesWidget->resizeColumnToContents(1);
                        newfile->setCheckState(0,Qt::Checked);
                        delete f;
                        if(chainCreated==true)    chain->Add(file.toLatin1().data());
                    }
                    else{
                        ShowMessage(QString("Error: Could not find tree %1 in file %2").arg(treeName).arg(file),"ERR");
                    }
                }
            }
            else{
                ShowMessage(QString("Error opening file %1").arg(file),"ERR");
            }
        }
        if(chainCreated==true){ // enable or disable
            //ui->actionOpen_Root_Files->setDisabled(true);
            //ui->actionOpen_Root_Files_as_Chain->setDisabled(true);
            isChain=true;
            ui->filesWidget->setDisabled(true);
            //            ui->actionSplit_Selected_Trees->setDisabled(true);
            //            loop->DisableFiles();
            currentTree=chain;
            QStringList branches = IROOTTree::GetListofBranches(currentTree);
            UpdateBranches(branches);
            currentTreeChanged(currentTree,branches);
        }
        else{ // chain not created
            isChain=false;
        }
    }

}
int IROOT::GetNumberOfFiles(bool selectedOnly)
{
    int nF=ui->filesWidget->topLevelItemCount();
    int selectedN=0;
    if(selectedOnly){
        for(int ii=0;ii<nF;ii++)
            if(ui->filesWidget->topLevelItem(ii)->checkState(0)==Qt::Checked) selectedN++;
        return selectedN;
    }
    return nF;
}

QStringList IROOT::GetFileList()
{
    QStringList fileList;
    for(int ii=0;ii<ui->filesWidget->topLevelItemCount();ii++){
        fileList.append(ui->filesWidget->topLevelItem(ii)->text(0));
    }
    return fileList;

}
/*Sets the file at index as the current file
*/
void IROOT::SetCurrentFile(int index)
{
    if(index<ui->filesWidget->topLevelItemCount()&& index >-1){
        ui->filesWidget->setCurrentItem(ui->filesWidget->topLevelItem(index));
    }
}

/*TTree* iRootFileContentsWidget::GetTree(TFile *f)
 * returns the trees in the file
 *
 */
QList <TTree*> IROOT::GetTree(TFile *f)
{
    QList <TTree*> treeList;
    if(f!=NULL){
        TList *list = f->GetListOfKeys();//StreamerInfo
        TKey *keys;
        if(list==NULL){
            //mainWindow->ShowMessage("Error: No keys found for finding tree in file...","ERR");
            return treeList;
        }
        TIter next(list);
        TString className="TTree";
        TTree *tree=0;
        while ((keys = (TKey *)next())){
            if(keys->GetClassName()==className){
                tree = (TTree*)f->Get(keys->GetName());
                if(tree!=NULL){
                    if(tree->GetEntry(tree->GetEntryNumber(0))==-1){
                        //mainWindow->ShowMessage("Error: No entries found in tree...","ERR");
                    }
                    else{
                        treeList.append(tree);
                    }
                }
            }
        }

    }
    else{
        //mainWindow->ShowMessage("Error: No tree found in file...","ERR");
        return treeList;
    }
    return treeList;
}

//on clicking new file in file widget
void IROOT::on_filesWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    //bool closePreviousFile = !ui->keepFilesOpen->isChecked();
    if(!isClearingFiles){// do not call this slot if clearing files is going on
        if(previous!=NULL&&currentFile!=0){
            //            if(closePreviousFile){ // latest addition
            //                delete currentFile; // close previous file
            //            }
            if(!ui->multiFileMode->isChecked())
                delete currentFile;
        }
        currentFile = new TFile(current->text(0).toLatin1().data(),"READ");
        if(current->text(2)==""){
            currentTree=0;
            ui->branchWidget->clear();
        }
        else{
            currentTree = (TTree*)currentFile->Get(current->text(2).toLatin1().data());
            QStringList branches = IROOTTree::GetListofBranches(currentTree);
            UpdateBranches(branches);
            currentTreeChanged(currentTree,branches);
            if(ui->applyFilterToAll->isChecked() && ui->filterString->text()!=""){
                on_filterEntries_clicked();
            }
            //            if(ui->filterAutoApply->isChecked()){
            //                FilterTree(currentTree,ui->filterString->text());
            //            }
            //else ResetAllFilters(currentTree);
        }

        // GetListofRootPlots(currentFile);
        //  mainWindow->ShowMessage(QString("File: %1 opened...").arg(currentFile->GetName()),"SUCCESS");

    }
}

void IROOT::UpdateBranches(QStringList branches){

    if(branches.count()>0){
        ui->branchWidget->clear();
        for(int ii=0;ii<branches.count();ii++){
            QTreeWidgetItem *t2 = new QTreeWidgetItem(ui->branchWidget);
            t2->setText(0,branches.at(ii));
            //            if(ii>=(leafList.count()-nFriends)){
            //                t2->setTextColor(0,Qt::blue);
            //            }

        }
        QStringList branches_and_options = branches;
        branches_and_options.append("Entry$");
        drawStringCompleter->setModel(new QStringListModel(branches_and_options,drawStringCompleter));
        cutStringCompleter->setModel(new QStringListModel(branches_and_options,cutStringCompleter));
        filterStringCompleter->setModel(new QStringListModel(branches_and_options,filterStringCompleter));
    }
}
///*QStringList iRootFileContentsWidget::ListBranches(TTree *tree)
// *List the names of branches in the tree
// */
//QStringList IROOT::ListBranches(TTree *tree)
//{
//    QStringList leafList;
//    ui->branchWidget->clear();
//    if(tree!=NULL){
//        QCompleter *completer;
//        leafList = GetListofLeavesinTree(tree);
//        // if tree has friends
//        int nFriends=0;
//        TList *l = tree->GetListOfFriends();
//        if(l!=NULL){
//            int n = l->GetEntries();
//            if(n>0){
//                for(int ii=0;ii<n;ii++){
//                    //mainWindow->ShowMessage(QString("Found friend tree: %1...").arg(l->At(ii)->GetName()),"INFO");
//                    TFriendElement * fe = (TFriendElement*)l->At(ii);
//                    if(fe!=NULL){
//                        TTree *t = fe->GetTree();
//                        if(t!=NULL){
//                            QStringList friendList = GetListofLeavesinTree(t);
//                            leafList.append(friendList);
//                            nFriends = nFriends+friendList.count();
//                        }
//                    }
//                }
//            }
//        }
//        if(leafList.count()>0){
//            for(int ii=0;ii<leafList.count();ii++){
//                QTreeWidgetItem *t2 = new QTreeWidgetItem(ui->branchWidget);
//                t2->setText(0,leafList.at(ii));
//                if(ii>=(leafList.count()-nFriends)){
//                    t2->setTextColor(0,Qt::blue);
//                }

//            }
//            completer = new QCompleter(leafList, this);
//            ui->plotString->setCompleter(completer);
//            ui->cutString->setCompleter(completer);

//        }
//        return leafList;
//    }
//    return leafList;

//}

///*QStringList iRootFileContentsWidget::GetListofLeavesinTree(TTree *tree)
// *Gets the list of names of leaves in the tree
// */
//QStringList IROOT::GetListofLeavesinTree(TTree *tree)
//{
//    QStringList leavesList;
//    if(tree==NULL)return leavesList;
//    int nDim=0; //dimension of the arrays if present
//    QChar  fillChar = QLatin1Char('0');
//    //tree->ResetBranchAddresses();
//    TObjArray * leafarray = tree->GetListOfLeaves();
//    TObject *leaf;
//    QString s,slstr;
//    QString s2;
//    QStringList sl;
//    int leafcnt=0;
//    int dim1,dim2;
//    if(leafarray!=NULL){
//        for(int ii=0;ii<leafarray->GetEntries();ii++){
//            leaf = leafarray->At(ii);
//            if(leaf!=NULL){
//                leavesList.append(QString("%1").arg(leaf->GetName()));
//                s = ((TLeaf*) leaf)->GetBranch()->GetName();
//                nDim=s.count("[");
//                //**IMPROVE**//
//                // allow only for 1d and 2d arrays at the moment
//                ((TLeaf*)leaf)->GetLeafCounter(leafcnt);
//                if(leafcnt>1){ // its an array
//                    //cross check for dimensions from leaf title
//                    s2=leaf->GetTitle();
//                    if(s2.count("[")==2){  nDim=2;
//                        s=s2;
//                    }
//                    if(nDim==1||nDim==0){ // one dimensional array
//                        for(int jj =0; jj<leafcnt; jj++){
//                            leavesList.append(QString("%1[%2]").arg(leaf->GetName()).arg(jj,3,10,fillChar));
//                        }
//                    }
//                    else if(nDim==2){
//                        // Get the dimensions from the string
//                        dim1=0; dim2=0;
//                        sl=s.split("[");
//                        slstr=sl.at(1);
//                        slstr=slstr.remove("]");
//                        dim1 = slstr.toInt();
//                        slstr=sl.at(2);
//                        slstr=slstr.remove("]");
//                        dim2 = slstr.toInt();

//                        for(int ii =0; ii<dim1; ii++){
//                            for(int jj=0;jj<dim2;jj++){
//                                leavesList.append(QString("%1[%2][%3]").arg(leaf->GetName()).arg(ii,3,10,fillChar).arg(jj,3,10,fillChar));
//                            }
//                        }
//                    }
//                }
//            }
//        }

//        return leavesList;
//    }
//    return leavesList;
//}

void IROOT::on_branchWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(currentTree!=NULL){
        QList <QTreeWidgetItem*> selectedItems= ui->branchWidget->selectedItems();
        if(selectedItems.count()>2){
            while(selectedItems.count()!=2){
                ui->branchWidget->setItemSelected(selectedItems.at(1),false);
                selectedItems.removeAt(1);
            }
        }
        if(selectedItems.count()==1){
            ui->plotString->insertItem(0,selectedItems.at(0)->text(0));
            ui->plotString->setCurrentIndex(0);
            on_plot_clicked();
        }
        if(selectedItems.count()==2){
            ui->plotString->insertItem(0,selectedItems.at(1)->text(0).append(":").append(selectedItems.at(0)->text(0)));
            ui->plotString->setCurrentIndex(0);
            on_plot_clicked();
        }
    }

}


void IROOT::on_addNewCanvas_clicked()
{
    AddCanvas("",ui->canvasRows->value(),ui->canvasColumns->value());
    ui->mainStackedWidget->setCurrentWidget(ui->workingPage);
}

void IROOT::on_canvasRows_valueChanged(int arg1)
{
    if(gPad!=NULL){
        gPad->GetCanvas()->Clear();
        gPad->GetCanvas()->Divide(arg1,ui->canvasColumns->value());
        gPad->GetCanvas()->Update();
        gPad->cd(1);
        gPad->SetLogx(ui->logX->isChecked());
        gPad->SetLogy(ui->logY->isChecked());
        gPad->SetGridx(ui->gridX->isChecked());
        gPad->SetGridy(ui->gridY->isChecked());

    }
}



void IROOT::on_canvasColumns_valueChanged(int arg1)
{
    if(gPad!=NULL){
        gPad->GetCanvas()->Clear();
        gPad->GetCanvas()->Divide(ui->canvasRows->value(),arg1);
        gPad->GetCanvas()->Update();
        gPad->cd(1);
        gPad->SetLogx(ui->logX->isChecked());
        gPad->SetLogy(ui->logY->isChecked());
        gPad->SetGridx(ui->gridX->isChecked());
        gPad->SetGridy(ui->gridY->isChecked());

    }
}


/*int iRoot::CountPads()
 *counts the number of pads in the current canvas.
 *returns -1 if canvas or pad is not found
 */
int IROOT::CountPads()
{
    //count the number of pads in pad
    if (!gPad) return -1;
    TCanvas* canvas = gPad->GetCanvas();
    if(canvas!=NULL){
        Int_t npads = ((TList*) canvas->GetListOfPrimitives())->GetSize();;
        return npads;
    }
    return -1;

}

/* void iRoot::on_nextPad_clicked()
 * Button action when next pad is clicked
 * Log scales are set and active pad numbers are set.
 * After last pad is reached, the first pad is activated
 */
void IROOT::on_nextPad_clicked()
{
    int npads=CountPads();
    if(npads>1){
        int padNumber = gPad->GetNumber();
        if ((padNumber+1)>npads){
            gPad->GetCanvas()->cd(1);
            iCanvas* c = (iCanvas*)ui->mdiArea->currentSubWindow()->widget();
            c->cd(1);
            //ui->padN->setText(QString("%1/%2").arg("1").arg(npads));
        }
        else{
            gPad->GetCanvas()->cd(padNumber+1);
            iCanvas* c = (iCanvas*)ui->mdiArea->currentSubWindow()->widget();
            c->cd(padNumber+1);
            //ui->padN->setText(QString("%1/%2").arg(padNumber+1).arg(npads));
        }
        gPad->SetLogx(ui->logX->isChecked());
        gPad->SetLogy(ui->logY->isChecked());
        gPad->SetGridx(ui->gridX->isChecked());
        gPad->SetGridy(ui->gridY->isChecked());
        gPad->Update();
    }
}
/*void iRoot::NextPad()
 *Go to next pad, if last pad, create a new pad,
 *used in loops
 *
 *
 */
void IROOT::NextPad()
{
    int npads=CountPads();
    if(npads==1)    AddCanvas("",ui->canvasRows->value(),ui->canvasColumns->value());
    if(npads>1){
        int padNumber = gPad->GetNumber();
        if ((padNumber+1)>npads){
            AddCanvas("",ui->canvasRows->value(),ui->canvasColumns->value());
        }
        else{
            gPad->GetCanvas()->cd(padNumber+1);
            iCanvas* c = (iCanvas*)ui->mdiArea->currentSubWindow()->widget();
            c->cd(padNumber+1);
        }

        gPad->SetLogx(ui->logX->isChecked());
        gPad->SetLogy(ui->logY->isChecked());
        gPad->SetGridx(ui->gridX->isChecked());
        gPad->SetGridy(ui->gridY->isChecked());

        //RefreshListofObjects();
        gPad->Update();

    }

}

/* void iRoot::on_prevPad_clicked()
 * Button action when next pad is clicked
 * Log scales are set and active pad numbers are set.
 * After first pad is reached, the last pad is activated
 */
void IROOT::on_prevPad_clicked()
{
    int npads=CountPads();
    if(npads>1){
        int padNumber = gPad->GetNumber();
        if ((padNumber-1)<1){
            gPad->GetCanvas()->cd(npads);
            iCanvas* c = (iCanvas*)ui->mdiArea->currentSubWindow()->widget();
            c->cd(npads);
            // ui->padN->setText(QString("%1/%2").arg(npads).arg(npads));
        }
        else{
            gPad->GetCanvas()->cd(padNumber-1);
            iCanvas* c = (iCanvas*)ui->mdiArea->currentSubWindow()->widget();
            c->cd(padNumber-1);
            // ui->padN->setText(QString("%1/%2").arg(padNumber-1).arg(npads));
        }
        gPad->SetLogx(ui->logX->isChecked());
        gPad->SetLogy(ui->logY->isChecked());
        gPad->SetGridx(ui->gridX->isChecked());
        gPad->SetGridy(ui->gridY->isChecked());
        gPad->Update();
        //   RefreshListofObjects();
    }
}



void IROOT::on_addWorksheet_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->workingPage);
    int nRows=25;
    int nCols=25;
    QStringList rowData;
    QStringList header;
    QList <QStringList> emptyData;
    for(int cc=0;cc<nCols;cc++){
        rowData.append("");
        header.append(QString("Column %1").arg(cc+1));
    }
    for(int rr=0;rr<nRows;rr++)
        emptyData.append(rowData);

    IROOTWorksheet *ws  = new IROOTWorksheet(emptyData,
                                             nRows,
                                             nCols,
                                             header
                                             );

    CreateNewTableView(ws,QString("Sheet %1").arg(tableN++));


}

/* void iRoot::QTableWidget *iRoot::CreateNewWorksheet(int rows, int cols, QString title)
 * Creates a new worksheet in the mdi area and sets its title
 * Connects the menu to the table
 * After first pad is reached, the last pad is activated
 */
QTableWidget *IROOT::CreateNewWorksheet(int rows, int cols, QString title)
{
    setCursor(Qt::BusyCursor);
    static int  sheetN=0;
    QTableWidget *workSheet = new QTableWidget(rows,cols);
    QMdiSubWindow *subWindow = new QMdiSubWindow();
    subWindow->setWidget(workSheet);
    ui->mdiArea->addSubWindow(subWindow);
    subWindow->showMaximized();
    if(title==""){
        subWindow->setWindowTitle(QString("Worksheet %1").arg(sheetN++));
    }
    subWindow->setWindowTitle(title);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->activateWindow();
    subWindow->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(subWindow,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowWorksheetMenu(QPoint)));
    ui->mdiArea->setActiveSubWindow(subWindow);
    setCursor(Qt::ArrowCursor);
    return workSheet;
}

/* void iRoot::QTableWidget *iRoot::CreateNewWorksheet(int rows, int cols, QString title)
 * Creates a new worksheet in the mdi area and sets its title
 * Connects the menu to the table
 * After first pad is reached, the last pad is activated
 */
QTableView *IROOT::CreateNewTableView(IROOTWorksheet* model, QString title)
{
    setCursor(Qt::BusyCursor);
    static int  sheetN=1;
    QTableView *tableView = new QTableView();
    QMdiSubWindow *subWindow = new QMdiSubWindow();
    subWindow->setWidget(tableView);
    ui->mdiArea->addSubWindow(subWindow);
    subWindow->showMaximized();
    if(title==""){
        subWindow->setWindowTitle(QString("Worksheet %1").arg(sheetN++));
    }
    tableView->setModel(model);
    model->SetTable(tableView);
    subWindow->setWindowTitle(title);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->activateWindow();
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    //connect(tableView,SIGNAL())
    //connect(subWindow,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowWorksheetMenu(QPoint)));
    connect(tableView, SIGNAL(customContextMenuRequested(const QPoint &)),model, SLOT(showMenu(QPoint)));
    connect(model,SIGNAL(CreatePad()),this,SLOT(on_addNewCanvas_clicked()));
    connect(model,SIGNAL(Message(QString,QString)),this,SLOT(AppendMessage(QString,QString)));
    ui->mdiArea->setActiveSubWindow(subWindow);
    setCursor(Qt::ArrowCursor);
    return tableView;
}

void IROOT::CreateCompilerWindow()
{
    static int nCompiler=1;
    setCursor(Qt::BusyCursor);
    IROOTCompiler *compiler = new IROOTCompiler();
    QMdiSubWindow *subWindow = new QMdiSubWindow();
    subWindow->setWidget(compiler);
    ui->mdiArea->addSubWindow(subWindow);
    subWindow->showMaximized();
    subWindow->setWindowTitle(QString("IROOT Compiler %1").arg(nCompiler++));
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->activateWindow();
    ui->mdiArea->setActiveSubWindow(subWindow);
    setCursor(Qt::ArrowCursor);
    connect(compiler,SIGNAL(ShowMessage(QString,QString)),this,SLOT(AppendMessage(QString,QString)));
    connect(compiler,SIGNAL(CreateNewPad()),this,SLOT(on_addNewCanvas_clicked()));
    //   return tableView;


}

QTableView *IROOT::CreateTreeViewer()
{
    //    setCursor(Qt::BusyCursor);
    //    static int  sheetN=1;
    QTableView *tableView = new QTableView();
    QMdiSubWindow *subWindow = new QMdiSubWindow(0,Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    subWindow->setWidget(tableView);
    ui->mdiArea->addSubWindow(subWindow);
    subWindow->showMaximized();

    subWindow->setWindowTitle("IROOT Tree Viewer (Read only)");

    tableView->setModel(IROOTTreeModel);
    IROOTTreeModel->SetTable(tableView);
    //  subWindow->setWindowTitle(title);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->activateWindow();
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    // tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    //connect(tableView,SIGNAL())
    //connect(subWindow,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowWorksheetMenu(QPoint)));
    connect(tableView, SIGNAL(customContextMenuRequested(const QPoint &)),IROOTTreeModel, SLOT(showMenu(QPoint)));
    connect(IROOTTreeModel,SIGNAL(CreatePad()),this,SLOT(on_addNewCanvas_clicked()));
    connect(IROOTTreeModel,SIGNAL(Message(QString,QString)),this,SLOT(AppendMessage(QString,QString)));
    //connect(subWindow,SIGNAL())
    ui->mdiArea->setActiveSubWindow(subWindow);
    setCursor(Qt::ArrowCursor);
    return tableView;
}

//void IROOT::SetTableData(int row, int column, QString data)
//{

//    if(row>20 && row<50)
//        qDebug()<<row;
//    QTableWidget* currentws = GetCurrentWorksheet();
//    if(currentws!=NULL){
//        if(row>currentws->rowCount())
//            currentws->setRowCount(row+1);
//        if(column>currentws->columnCount())
//            currentws->setColumnCount(column+1);
//        QTableWidgetItem *i = new QTableWidgetItem(data);
//        currentws->setItem(row, column, i);
//    }


//}

void IROOT::CreateTable()
{



    ui->mainStackedWidget->setCurrentWidget(ui->workingPage);
    ui->leftStackWidget->setCurrentWidget(ui->filesPage);

    IROOTWorksheet *ws  = new IROOTWorksheet(worksheetImporter->GetImportThread()->GetData(),
                                             worksheetImporter->GetImportThread()->GetRowCount(),
                                             worksheetImporter->GetImportThread()->GetColumnCount(),
                                             worksheetImporter->GetImportThread()->GetHeader()
                                             );

    CreateNewTableView(ws,QString("Sheet %1").arg(tableN++));


}

QTableWidget* IROOT::GetCurrentWorksheet()
{
    if(ui->mdiArea->currentSubWindow()==NULL)
        return 0;
    if(ui->mdiArea->currentSubWindow()->widget()==NULL)
        return 0;
    if(ui->mdiArea->currentSubWindow()->widget()->inherits("QTableWidget"))
        return (QTableWidget*)ui->mdiArea->currentSubWindow()->widget();
    else return 0;

}
/*int iRoot::plot(QString draw, QString cut, QString style, int loop,int type, bool refreshObjects, bool applyTitle, bool fit)
 *main plotting functions, manages updating the objects and looping
 *if same option is true, then "SAME" is appended to the style string if there is a plot already existing
 */
int IROOT::plot(QString draw, QString cut, QString style, int loop,int type, bool refreshObjects, bool same, bool autoFit) // type 1: stack 2: multigraph
{
    if(currentTree!=NULL){
        if(gPad==NULL) AddCanvas("",0,0);
        int ret=0;

        // existence of a TFrame is a sign that there is some thing already plotted
        if(gPad->FindObject("TFrame") && same)
            style = style+"SAME";
        if(!gPad->FindObject("TFrame")){ // invalid same
            if(style.contains("SAME")||style.contains("same")){
                style = style.remove("SAME");
                style = style.remove("same");
            }
        }
        static int nhist=0;
        if(currentBinning.useDefault!=true){
            if(!draw.contains(":")){
                draw.append(QString(">>h%1(%2,%3,%4)")
                            .arg(nhist++)
                            .arg(currentBinning.xbins)
                            .arg(currentBinning.xmin)
                            .arg(currentBinning.xmax));
            }
            else if(draw.count(":")==1){
                draw.append(QString(">>h%1(%2,%3,%4,%5,%6,%7,%8,%9,%10)")
                            .arg(nhist++)
                            .arg(currentBinning.xbins)
                            .arg(currentBinning.xmin)
                            .arg(currentBinning.xmax)
                            .arg(currentBinning.ybins)
                            .arg(currentBinning.ymin)
                            .arg(currentBinning.ymax)
                            );
            }
            else{
                draw.append(QString(">>h%1(%2,%3,%4,%5,%6,%7,%8,%9,%10)")
                            .arg(nhist++)
                            .arg(currentBinning.xbins)
                            .arg(currentBinning.xmin)
                            .arg(currentBinning.xmax)
                            .arg(currentBinning.ybins)
                            .arg(currentBinning.ymin)
                            .arg(currentBinning.ymax)
                            .arg(currentBinning.zbins)
                            .arg(currentBinning.zmin)
                            .arg(currentBinning.zmax));
            }
        }
        else{
            //draw.append(">>htemp(100)");
        }



        ret = currentTree->Draw(draw.replace("%1",QString("%1").arg(loop)).toLatin1().data(),
                                cut.replace("%1",QString("%1").arg(loop)).toLatin1().data(),
                                style.toLatin1().data());
        if(ret==0) ShowMessage("Information: Plot returns no entries...","INFO");
        //ret < 0 is handled by Check tree string
        //else if(ret<0) ShowMessage("Error: Plot returns negative values, check draw and cut strings...","ERR");
        else if(ret>0)
        { // do all the other services
            // save strings
            //            if(currentBinning.useDefault!=true){
            //              TObject* lastObject =  gPad->GetPrimitive(QString("h%1").arg(nhist-1).toLatin1().data());
            //              if(lastObject!=NULL)
            //                  gROOT->Add(lastObject);
            //            }
            //currentTree->GetHistogram()->SetDirectory(gDirectory);
            if(ui->plotString->findText(ui->plotString->currentText())==-1)
                ui->plotString->addItem(ui->plotString->currentText());
            if(ui->cutString->findText(ui->cutString->currentText())==-1)
                ui->cutString->addItem(ui->cutString->currentText());
            gPad->Update();
            if(autoFit){
                if(ui->plotTemplate->currentIndex()==4){
                    Fit("LASTPRIMITIVE",loop,false);
                }
                else{
                    Fit("LASTPRIMITIVE",loop,true);
                }

            }



            //            if(ui->useCurrentStyle->isChecked()){
            //                gPad->UseCurrentStyle();
            //            }
            //            if(ui->canvasGridX->isChecked()){
            //                gPad->SetGridx(1);
            //            }
            //            if(ui->canvasGridY->isChecked()){
            //                gPad->SetGridy(1);
            //            }
            //            if(ui->BuildLegends->isChecked())
            //                BuildLegend();

            //currentLoop=loop;

            if(refreshObjects){
                //                axisD->SetCurrentLoop(loop);
                //                RefreshListofObjects();
                //                if(ui->padStyles->currentText()=="Worksheet")
                //                    SaveStatisticsInWorksheet(loop);

            }

        }

        return ret;
    }
    return -5;
}

void IROOT::on_plot_clicked()
{

    if(currentTree!=NULL){
        if(ui->loop->isChecked()){
            LoopPlot(loopModule->GetLoopType(),
                     loopModule->GetLoopBegin(),
                     loopModule->GetLoopEnd(),
                     loopModule->GetLoopIncrement(),
                     loopModule->GetPadIncrement());
            fitObjects->Update();
            return;
        }

        QString draw=ui->plotString->currentText();
        QString cut="";
        QString style="";
        cut=(ui->cutString->currentText()=="Enter cut string here") ? cut : ui->cutString->currentText();
        style=(ui->styleString->text()=="Enter style string here") ? style : ui->styleString->text();

        if(ui->superImpose->isChecked() && !(style.contains("SAME")||style.contains("same")))
            style.append("SAME");
        // CHECK PLOT STRING
        CheckTreeString(draw,cut);
        // Proceed to plotting
        //        if(ui->loopingPlot->isChecked()){
        //            return;
        //        }
        //else{
        if(draw.contains("%1")||cut.contains("%1")){
            ShowMessage("Warning: Loop variable %1 is allowed only for looping plots, using minimum value...","WARN");
        }

        plot(draw,cut,style,0,0,true,false,ui->Autofit->isChecked());
        fitObjects->Update();

    }
}

/*
 *Loops over the index or files
 *
 */
void IROOT::LoopPlot(int type, int begin, int end, int increment, int padincrement)
{
    if(CheckTreeString(ui->plotString->currentText(),ui->cutString->currentText())<0){
        QMessageBox msgBox;
        msgBox.setText("Errors in draw/cut string.");
        msgBox.setInformativeText("Do you want to proceed with the loop?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Yes:
            // Save was clicked
            break;
        case QMessageBox::No:
            return;
            break;
        case QMessageBox::Cancel:
            return;
            break;
        default:
            // should never be reached
            break;
        }
    }

    AddCanvas("",0,0);
    if(type==1){ // index loop
        LoopPlotOverIndex(loopModule->GetLoopBegin(),
                          loopModule->GetLoopEnd(),
                          loopModule->GetLoopIncrement(),
                          loopModule->GetPadIncrement()
                          );
    }
    else if(type==2){ // file loop
        LoopPlotOverFiles(true,0,loopModule->GetPadIncrement());
    }
    else if(type==3){// files and then index
        for(int ii=begin;ii<=end;ii=ii+increment){
            LoopPlotOverFiles(true,ii,padincrement);

            if((ii+increment)!=end)
                NextPad();
        }

    }
    else if(type==4){
        int nF  = GetNumberOfFiles();
        for(int ii=0;ii<nF;ii++){
            if(ui->filesWidget->topLevelItem(ii)->checkState(0)==Qt::Checked)
            {
                SetCurrentFile(ii);
                LoopPlotOverIndex(begin,end,increment,padincrement);
                if((ii+1)!=nF)
                    NextPad();
            }

        }
    }
}

/*
 *Loops over the index or files
 *
 */
void IROOT::LoopPlotOverIndex(int begin, int end, int increment, int padincrement)
{
    QString draw=ui->plotString->currentText();
    QString cut="";
    QString style="";

    cut=(ui->cutString->currentText()=="Enter cut string here") ? cut : ui->cutString->currentText();
    style=(ui->styleString->text()=="Enter style string here") ? style : ui->styleString->text();

    // CHECK PLOT STRING
    //   if(CheckTreeString(draw,cut)!=0) return;
    int nPlots=0;
    for(int ii=begin;ii<=end;ii=ii+increment){
        plot(draw,
             cut,
             ui->styleString->text(),
             ii,
             0,
             false,
             true,
             ui->Autofit->isChecked());
        nPlots++;
        if(nPlots==padincrement && (ii+increment)<=end){
            NextPad();
            nPlots=0;
        }
    }
}


/*
 *Loops over all the files, if selectedOnly is set to true, then loops over only selected files
 *
 */
void IROOT::LoopPlotOverFiles(bool selectedOnly, int loop, int padincrement)
{
    QString draw=ui->plotString->currentText();
    QString cut="";
    QString style="";

    cut=(ui->cutString->currentText()=="Enter cut string here") ? cut : ui->cutString->currentText();
    style=(ui->styleString->text()=="Enter style string here") ? style : ui->styleString->text();


    int nF  = GetNumberOfFiles();
    int nPlots=0;
    for(int ii=0;ii<nF;ii++){
        if((selectedOnly==true && ui->filesWidget->topLevelItem(ii)->checkState(0)==Qt::Checked)||(selectedOnly==false))
        {
            SetCurrentFile(ii);

            plot(draw,
                 cut,
                 ui->styleString->text(),
                 loop,
                 0,
                 false,
                 true,
                 ui->Autofit->isChecked());

            nPlots++;

            if(nPlots==padincrement && (ii+1)!=nF){
                NextPad();
                nPlots=0;
            }
        }


    }


}



/*int IROOT::CheckTreeString(QString s, QString c)
 *Checks the draw strings and the cut strings and reports errors
 *
 */


int IROOT::CheckTreeString(QString s, QString c, int type)
{
    if(currentTree!=NULL){
        int err=0;
        int ret =0;
        if(currentTree->Draw(s.replace("%1","").toLatin1().data(),"","goff",1,0)==-1){ // plot without condition to check error in plot string
            IROOTTreeFormulaChecker *formula = new IROOTTreeFormulaChecker("checkDrawString",s.replace("%1","").toLatin1().data(),currentTree);
            formula->Analyze(s.replace("%1","").toLatin1().data(),err,0);
            if(type==1)
                ShowMessage(QString("Filter string error: %1").arg(formula->GetErrorString()),"ERR");
            else
                ShowMessage(QString("Draw string error: %1").arg(formula->GetErrorString()),"ERR");
            ui->plotString->insertItem(0,QIcon(":/icons/icons/windows8_icons/System/error.png"),ui->plotString->currentText());
            ui->plotString->setCurrentIndex(0);
            ret=-1;
            delete formula;
            if(c=="")return ret;
        }
        else{
            ui->plotString->insertItem(0,QIcon(":/icons/icons/metro.icons/black_nocircle/check.png"),ui->plotString->currentText());
            ui->plotString->setCurrentIndex(0);
        }

        if(currentTree->Draw(s.replace("%1","").toLatin1().data(),c.replace("%1","").toLatin1().data(),"goff",1,0)==-1){ // plot with condition to check error in cut
            IROOTTreeFormulaChecker *formula = new IROOTTreeFormulaChecker("checkDrawString",c.replace("%1","").toLatin1().data(),currentTree);
            formula->Analyze(c.replace("%1","").toLatin1().data(),err,0);
            ShowMessage(QString("Cut string error: %1").arg(formula->GetErrorString()),"ERR");
            ui->cutString->insertItem(0,QIcon(":/icons/icons/windows8_icons/System/error.png"),ui->cutString->currentText());
            ui->cutString->setCurrentIndex(0);
            ret=ret-2;
            delete formula;
            //            if(c.contains("CUTG")&&ui->padStyles->currentText()=="Pub"){
            //                ShowMessage("Graphical cuts are not available in Pub mode","ERR");
            //            }
        }
        else{
            ui->cutString->insertItem(0,QIcon(":/icons/icons/metro.icons/black_nocircle/check.png"),ui->cutString->currentText());
            ui->cutString->setCurrentIndex(0);
        }

        return ret;
    }
    else
        return -5;
}
void IROOT::ShowMessage(QString messageText, QString type)
{
    message->ShowMessage(messageText,type,ui->mdiArea);
    //QToolTip::showText( ui->plotTemplate->mapToGlobal( QPoint( 0, 0 ) ), messageText );
}

void IROOT::AppendMessage(QString messageText, QString type)
{
    message->ShowMessage(messageText,type,ui->mdiArea);
    //message->Append(messageText,type,ui->canvasColumns->pos().x(),ui->canvasColumns->pos().y());
    // QToolTip::showText( ui->plotTemplate->mapToGlobal( QPoint( 0, 0 ) ), messageText );
}
void IROOT::ShowMessages(int milliseconds)
{
    message->ShowMessages(0);
}



void IROOT::on_gridX_clicked()
{
    if(gPad!=NULL){
        if(ui->gridX->isChecked())
            gPad->SetGridx(1);
        else
            gPad->SetGridx(0);
    }
}

void IROOT::on_gridY_clicked()
{
    if(gPad!=NULL){
        if(ui->gridY->isChecked())
            gPad->SetGridy(1);
        else
            gPad->SetGridy(0);
    }
}

void IROOT::on_logX_clicked()
{
    if(gPad!=NULL){
        if(ui->logX->isChecked())
            gPad->SetLogx(1);
        else
            gPad->SetLogx(0);
    }
}

void IROOT::on_logY_clicked()
{
    if(gPad!=NULL){
        if(ui->logY->isChecked())
            gPad->SetLogy(1);
        else
            gPad->SetLogy(0);
    }
}

void IROOT::on_plotTemplate_currentIndexChanged(const QString &arg1)
{

    gROOT->SetStyle(arg1.toLatin1().data());
    gROOT->ForceStyle();
    gStyle->UseCurrentStyle();

}

void IROOT::on_openRootFile_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,"Open ROOT Files", "","(*.root);;All files (*.*)");
    OpenFiles(filenames);
    if(filenames.count()>0)
        ui->mainStackedWidget->setCurrentWidget(ui->workingPage);

}

QString IROOT::GetPlotString()
{
    if(ui->plotString->currentText().contains("Enter plot"))
        return "";
    return ui->plotString->currentText();
}

QString IROOT::GetCutString()
{
    if(ui->cutString->currentText().contains("Enter cut"))
        return "";

    return ui->cutString->currentText();
}

void IROOT::on_templateOptions_itemClicked(QTreeWidgetItem *item, int column)
{
    QString styleString;
    QList <QTreeWidgetItem*> items = ui->templateOptions->selectedItems();
    for(int ii=0;ii<items.count();ii++){
        if(items.at(ii)->text(0).contains("|")){
            styleString.append(items.at(ii)->text(0).split("|").at(1));
        }
    }
    ui->styleString->setText(styleString);
    if(ui->previewTemplateStyle->isChecked())
        on_plot_clicked();
}

void IROOT::on_viewFiles_clicked()
{
    ui->leftStackWidget->setCurrentIndex(0);
}

void IROOT::on_viewTemplates_clicked()
{
    ui->leftStackWidget->setCurrentIndex(2);
}

void IROOT::on_viewSetings_clicked()
{
    ui->leftStackWidget->setCurrentIndex(3);
}

void IROOT::on_fit_clicked()
{
    if(ui->plotTemplate->currentIndex()==4)
        Fit("",0,false);
    else
        Fit("",0,true);

}

void IROOT::Fit(QString opt, int loop, bool statsOnPad)
{
    QStringList             parNames;
    QList <double>          parValues;
    for(int ii=0;ii<10;ii++){
        if(ui->parTable->item(ii,0)!=NULL){
            parNames.append(ui->parTable->item(ii,0)->text());
        }
        else{
            parNames.append("");
        }
        if(ui->parTable->item(ii,1)!=NULL){
            parValues.append(ui->parTable->item(ii,1)->text().toDouble());
        }
        else{
            parValues.append(0);
        }
    }

    int N=fitObjects->GetListofSelectedObjects().count();
    if(opt=="LASTPRIMITIVE")
        N=1;
    for(int ii=0;ii<N;ii++){
        QList <double> par;
        QList <double> parErr;
        double chiSq;
        int    DOF;
        QStringList results;
        fitResult fR;
        if(opt=="LASTPRIMITIVE")
            results = IROOTPad::FitData(opt,ui->fitFunction->currentText(),
                                        parNames,parValues,
                                        ui->fitOptions->text(),
                                        ui->rangeMin->value(),
                                        ui->rangeMax->value(),
                                        fR,
                                        statsOnPad);
        else
            results = IROOTPad::FitData(fitObjects->GetListofSelectedObjects().at(ii),ui->fitFunction->currentText(),
                                        parNames,parValues,
                                        ui->fitOptions->text(),
                                        ui->rangeMin->value(),
                                        ui->rangeMax->value(),
                                        fR,
                                        statsOnPad);

        if(results.count()>0)
            ui->fitResults->appendPlainText(QString("---FIT RESULTS FOR %1---").arg(results.at(0)));
        for(int jj=1;jj<results.count();jj++)
            ui->fitResults->appendPlainText(results.at(jj));
        ui->fitResults->appendPlainText("---END OF RESULT---");

        if(currentFile!=NULL)
            fR.fileName = currentFile->GetName();

        fR.loopN        =loop;
        fR.drawString   =GetPlotString();
        fR.cutString    =GetCutString();
        fR.fitFunction  =ui->fitFunction->currentText();
        fR.fitOptions   =ui->fitOptions->text();

        fitResultsTable->appendFitResults(fR);
    }

}


void IROOT::on_chooseFitOptions_clicked()
{
    fitOptions->move(QCursor::pos().x(),QCursor::pos().y());
    fitOptions->show();
    fitOptions->raise();

}

void IROOT::SetFitOption(QList<QTreeWidgetItem *> list)
{
    QString options;
    for(int ii=0;ii<list.count();ii++){
        if(list.at(ii)->text(0).contains("|"))
            options.append(list.at(ii)->text(0).split("|").at(1));
    }
    ui->fitOptions->setText(options);


}

void IROOT::on_moreObjects_clicked()
{
    fitObjects->Update();
    fitObjects->show();
    fitObjects->move(QCursor::pos().x(),QCursor::pos().y());
    fitObjects->raise();
}

void IROOT::on_plotFunction_clicked()
{
    if(gPad==NULL)AddCanvas("",0,0);
    double rangeMin, rangeMax;
    rangeMin = ui->rangeMin->value();
    rangeMax = ui->rangeMax->value();
    if(ui->rangeMin->value()==0 && ui->rangeMax->value()==0){
        ShowMessage("Incorrect range for plotting function, using default (0,100)","ERR");
        rangeMin=0;
        rangeMax=100;
    }
    QStringList             parNames;
    QList <double>          parValues;
    for(int ii=0;ii<10;ii++){
        if(ui->parTable->item(ii,0)!=NULL){
            parNames.append(ui->parTable->item(ii,0)->text());
        }
        else{
            parNames.append("");
        }
        if(ui->parTable->item(ii,1)!=NULL){
            parValues.append(ui->parTable->item(ii,1)->text().toDouble());
        }
        else{
            parValues.append(0);
        }
    }
    bool allZeros=true;
    for(int jj=0;jj<parValues.count();jj++){
        if(parValues.at(jj)!=0){
            allZeros=false;
            break;
        }
    }
    if(allZeros){
        ShowMessage("None of the function parameters are initialized!","ERR");
        ui->fitIO->setCurrentIndex(0);
    }
    QStringList results =IROOTPad::PlotFunction(ui->fitFunction->currentText(),parNames,parValues,rangeMin,rangeMax);
    for(int ii=0;ii<results.count();ii++)
        ui->fitResults->appendPlainText(results.at(ii));
    fitObjects->Update();

}

void IROOT::on_loop_clicked()
{
    if(ui->loop->isChecked()){
        loopModule->move(QCursor::pos().x()-loopModule->width(),QCursor::pos().y()-loopModule->height());
        loopModule->show();
        loopModule->raise();

    }
}

void IROOT::on_fontcomboBox_currentIndexChanged(const QString &arg1)
{
    //    QFont f(arg1);
    //    ui->fontcomboBox->setFont(f);
    if(ui->axisFont->isChecked())
        IROOTPad::SetFont(IROOTPad::GetFontIndex(ui->fontcomboBox->currentIndex(),ui->italic->isChecked(),ui->bold->isChecked()),
                          "axis");
    if(ui->titleFont->isChecked())
        IROOTPad::SetFont(IROOTPad::GetFontIndex(ui->fontcomboBox->currentIndex(),ui->italic->isChecked(),ui->bold->isChecked()),
                          "title");
    if(ui->statsFont->isChecked())
        IROOTPad::SetFont(IROOTPad::GetFontIndex(ui->fontcomboBox->currentIndex(),ui->italic->isChecked(),ui->bold->isChecked()),
                          "stats");
    if(ui->legendFont->isChecked())
        IROOTPad::SetFont(IROOTPad::GetFontIndex(ui->fontcomboBox->currentIndex(),ui->italic->isChecked(),ui->bold->isChecked()),
                          "legend");

}

void IROOT::on_fontSize_valueChanged(double arg1)
{
    if(ui->axisFont->isChecked())
        IROOTPad::SetFontSize(arg1,"axis");
    if(ui->titleFont->isChecked())
        IROOTPad::SetFontSize(arg1,"title");
    if(ui->statsFont->isChecked())
        IROOTPad::SetFontSize(arg1,"stats");
    if(ui->legendFont->isChecked())
        IROOTPad::SetFontSize(arg1,"legend");
}

void IROOT::on_fontColor_clicked()
{
    colorDialog->move(QCursor::pos().x(),QCursor::pos().y());
    colorDialog->raise();
    colorDialog->show();
}

void IROOT::fontColorSelected(QColor c)
{
    if(ui->axisFont->isChecked())
        IROOTPad::SetFontColor(c.red(),c.green(),c.blue(),"axis");
    if(ui->titleFont->isChecked())
        IROOTPad::SetFontColor(c.red(),c.green(),c.blue(),"title");
    if(ui->statsFont->isChecked())
        IROOTPad::SetFontColor(c.red(),c.green(),c.blue(),"stats");
    if(ui->legendFont->isChecked())
        IROOTPad::SetFontColor(c.red(),c.green(),c.blue(),"legend");
}

void IROOT::on_bold_clicked()
{
    on_fontcomboBox_currentIndexChanged(ui->fontcomboBox->currentText())   ;
}

void IROOT::on_italic_clicked()
{
    on_fontcomboBox_currentIndexChanged(ui->fontcomboBox->currentText())   ;
}

void IROOT::on_openMultipleRootFile_clicked()
{
    multiDirSearch->raise();
    multiDirSearch->show();
}

void IROOT::on_convertAsciitoTree_clicked()
{
    asciiConvertor->raise();
    asciiConvertor->show();
}

void IROOT::fitObjectsSelected(QStringList list)
{
    ui->moreObjects->setText(QString("(%1)").arg(list.count()));
    if(list.count()==0)
        ui->fit->setDisabled(true);
    else
        ui->fit->setDisabled(false);
}


//Fit Results table
//TABLE VIEW

//int IROOTFitResultsTable::rowCount(const QModelIndex &parent) const
//{
//    return results.count();
//}



//int IROOTFitResultsTable::columnCount(const QModelIndex &parent) const
//{
//    return maxColumns;
//}

//QVariant IROOTFitResultsTable::data(const QModelIndex &index, int role) const
//{

//    if(role == Qt::DisplayRole)
//    {
//        if(results.count()>0){
//            if(index.column()==0)
//                return results.at(index.row()).fileName;
//            if(index.column()==1)
//                return results.at(index.row()).loopN;
//            if(index.column()==2)
//                return results.at(index.row()).drawString;
//            if(index.column()==3)
//                return results.at(index.row()).cutString;
//            if(index.column()==4)
//                return results.at(index.row()).entries;
//            if(index.column()==5)
//                return results.at(index.row()).mean;
//            if(index.column()==6)
//                return results.at(index.row()).rms;
//            if(index.column()==7)
//                return results.at(index.row()).fitFunction;
//            if(index.column()==8)
//                return results.at(index.row()).fitOptions;
//            if(index.column()==9)
//                return results.at(index.row()).objectName;
//            if(index.column()==10)
//                return results.at(index.row()).chiSq;
//            if(index.column()==11)
//                return results.at(index.row()).DOF;
//            if(index.column()>11){
//                if(index.column()%2==0){
//                    if((index.column()-12)/2 < results.at(index.row()).par.count())
//                        return results.at(index.row()).par.at((index.column()-12)/2);
//                }
//                else{
//                    if((index.column()-13)/2 < results.at(index.row()).parErr.count())
//                        return results.at(index.row()).parErr.at((index.column()-13)/2);
//                }
//            }

//        }


//    }


//    return QVariant::Invalid;
//}

//QVariant IROOTFitResultsTable::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
//    {
//        if(results.count()>0){
//            if(section==0)
//                return "File";
//            if(section==1)
//                return "Loop";
//            if(section==2)
//                return "Plot String";
//            if(section==3)
//                return "Cut String";
//            if(section==4)
//                return "Entries";
//            if(section==5)
//                return "Mean";
//            if(section==6)
//                return "RMS";
//            if(section==7)
//                return "Fit Function";
//            if(section==8)
//                return "Fit options";
//            if(section==9)
//                return "Object";
//            if(section==10)
//                return "Chi.sq";
//            if(section==11)
//                return "DOF";
//            if(section>11){
//                if(section%2==0){
//                    return QString("P%1").arg((section-12)/2);
//                }
//                else{
//                    return QString("Error P%1").arg((section-13)/2);
//                }
//            }
//        }

//    }

//    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
//    {
//        return section;
//    }


//    return QVariant::Invalid;
//}


// Table View





void IROOT::on_histogramBinning_clicked()
{
    histogramBinning->show();
    histogramBinning->move(QCursor::pos().x(),QCursor::pos().y()-histogramBinning->height());
    histogramBinning->raise();
}

void IROOT::on_ImportAscii_clicked()
{
    worksheetImporter->show();
}

void IROOT::on_plotOptions_clicked()
{

    plotOptionsEntry->move(QCursor::pos().x(),QCursor::pos().y()-plotOptionsEntry->height());
    plotOptionsEntry->show();
    plotOptionsEntry->raise();
}

void IROOT::on_plotString_textChanged(const QString &arg1)
{
    QStringList l = arg1.split(":");
    QString lastString = l.at(l.count()-1);
    if(lastString=="")return;
    {
        drawStringCompleter->setCompletionPrefix(lastString);
        drawStringCompleter->popup()->setCurrentIndex( drawStringCompleter->completionModel()->index(0, 0));
        drawStringCompleter->setWidget(ui->plotString);
        //        drawStringCompleter->popup()->move(ui->plotString->mapToGlobal(ui->plotString->pos()).x()+ui->plotString->lineEdit()->cursorPosition()+80,
        //                                           ui->plotString->mapToGlobal(ui->plotString->pos()).y()
        //                                           );//QCursor::pos()
        drawStringCompleter->complete();
    }

}

void IROOT::on_cutString_textChanged(const QString &arg1)
{
    QString replaced = arg1;
    replaced.replace("&&","::");
    replaced.replace("||","::");
    replaced.replace("==","::");
    QStringList l = replaced.split("::");
    QString lastString = l.at(l.count()-1);
    if(lastString=="")return;
    {
        cutStringCompleter->setCompletionPrefix(lastString);
        cutStringCompleter->popup()->setCurrentIndex( cutStringCompleter->completionModel()->index(0, 0));
        cutStringCompleter->setWidget(ui->cutString);
        //        drawStringCompleter->popup()->move(ui->plotString->mapToGlobal(ui->plotString->pos()).x()+ui->plotString->lineEdit()->cursorPosition()+80,
        //                                           ui->plotString->mapToGlobal(ui->plotString->pos()).y()
        //                                           );//QCursor::pos()
        cutStringCompleter->complete();
    }
}

void IROOT::on_filterString_textChanged(const QString &arg1)
{
    QString replaced = arg1;
    replaced.replace("&&","::");
    replaced.replace("||","::");
    replaced.replace("==","::");
    QStringList l = replaced.split("::");
    QString lastString = l.at(l.count()-1);
    if(lastString=="")return;
    {
        filterStringCompleter->setCompletionPrefix(lastString);
        filterStringCompleter->popup()->setCurrentIndex(filterStringCompleter->completionModel()->index(0, 0));
        filterStringCompleter->setWidget(ui->filterString);
        //        drawStringCompleter->popup()->move(ui->plotString->mapToGlobal(ui->plotString->pos()).x()+ui->plotString->lineEdit()->cursorPosition()+80,
        //                                           ui->plotString->mapToGlobal(ui->plotString->pos()).y()
        //                                           );//QCursor::pos()
        filterStringCompleter->complete();
    }

}

void IROOT::on_chainFiles_clicked()
{
    if(!isChain){ // not chained
        QStringList fileList = GetFileList();
        if(fileList.count()>0){
            on_clear_files_clicked();
            OpenFilesAsChain(fileList);
            if(isChain)
                ui->chainFiles->setText("RELEASE CHAIN");
        }
    }
    else{ // chained
        QStringList fileList = GetFileList();
        if(fileList.count()>0){
            on_clear_files_clicked();
            OpenFiles(fileList);
        }
        if(!isChain)
            ui->chainFiles->setText("CHAIN");
    }

}

void IROOT::on_clear_files_clicked()
{
    QStringList l;
    IROOTTreeModel->resetTree();
    isClearingFiles=true;

    //   // closePreviousFile = true;
    //    if(closePreviousFile){
    //        if(ui->filesWidget->currentItem()!=NULL){
    //            if(currentFile!=NULL)
    //                delete currentFile; // close file
    //            currentTree=0;
    //            currentFile=0;
    //        }
    //    }
    //else if(ui->filesWidget->isEnabled()){// what if option "do not close files" is chosen??
    if(!isChain){ // only if this is not a chain
        //this should not be called for TChains as it willl delete all inexistent trees
        //this is overcome by checking if filesWidget is enabled (i.e it is not a chain)
        for(int ii=0;ii<ui->filesWidget->topLevelItemCount();ii++){
            currentFile = new TFile(ui->filesWidget->topLevelItem(ii)->text(0).toLatin1().data(),"READ");
            if(currentFile!=NULL)
                delete currentFile;
            currentTree=0;
            currentFile=0;
        }
    }

    ui->filesWidget->setEnabled(true);
    ui->filesWidget->clear();
    ui->branchWidget->clear();
    isClearingFiles=false;
}

void IROOT::on_filterEntries_clicked()
{
    if(currentTree!=NULL ){
        QString err;
        int ret = CheckTreeString(ui->filterString->text(),"",1);
        int entries = IROOTTree::FilterTree(currentTree,ui->filterString->text(),err);
        if(ret>-1){
            if(err!="")
                ShowMessage(err,"WARN");
            if(!isChain){
                if(ui->filesWidget->currentItem())
                    ui->filesWidget->currentItem()->setText(1,QString("%1/%2").arg(entries).arg(currentTree->GetEntries()));
            }

            IROOTTreeModel->refreshTree();
        }
    }
}


void IROOT::on_clearFitData_clicked()
{
    fitResultsTable->clearData();

}

void IROOT::saveSettings()
{
    QSettings settings("DSAMUELS","IROOT");
    if(GetFileList().count()!=0){
        QStringList empty;
        QStringList lastfileList =settings.value("RecentFiles",empty).toStringList();
        lastfileList.append(GetFileList());
        lastfileList.removeDuplicates();
        while(lastfileList.count()>MAXRECENTFILESLIST)
            lastfileList.removeFirst();
        settings.setValue("RecentFiles",lastfileList);
    }

}

void IROOT::loadSettings()
{
    QSettings settings("DSAMUELS","IROOT");
    QStringList empty;
    QStringList lastFileList =settings.value("RecentFiles",empty).toStringList();
    if(lastFileList.count()>0)
        UpdateRecentFiles(lastFileList);

}

void IROOT::UpdateRecentFiles(QStringList recentFiles)
{
    //ui->recentFiles->clear();
    for(int ii=0;ii<recentFiles.count();ii++){
        QFile f(recentFiles.at(ii));
        if(f.exists()){
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->recentFiles);
            item->setText(0,recentFiles.at(ii));
        }
    }
}

void IROOT::on_recentFiles_itemClicked(QTreeWidgetItem *item, int column)
{
    QStringList files;
    files.append(item->text(0));
    OpenFiles(files);
}

void IROOT::on_save_clicked()
{
    // this section is tricky as some times the focus is not received
    QApplication::setActiveWindow(ui->mdiArea); // This was needed to change the focus to the MdiArea
    QMdiSubWindow *subWin = ui->mdiArea->currentSubWindow();

    if(subWin==NULL){
        if(ui->mdiArea->subWindowList().count()>0){
            ui->mdiArea->setActiveSubWindow(ui->mdiArea->subWindowList().at(0));
            ui->mdiArea->activateWindow();
        }
        subWin=ui->mdiArea->subWindowList().at(0);
    }

    if(subWin!=NULL){
        if(subWin->widget()!=NULL){
            // qDebug()<<subWin->widget()->metaObject()->className();
            if(subWin->widget()->metaObject()->className()==iCanvas::staticMetaObject.className())
                SaveCanvas();
            else if(subWin->widget()->metaObject()->className()==QTableView::staticMetaObject.className())
                SaveWorksheet();
        }
    }
}

void IROOT::SaveCanvas()
{
    QString filter;
    static QString default_name="";
    // static int     nSave=1;

    QString fileName= QFileDialog::getSaveFileName(this,"Choose a file name to save canvas",default_name, "*.png;;*.jpg;;*.pdf;;*.eps;;*.svg;;*.gif;;*.xpm;;*.tiff;;*.cxx;;*.root;;*.xml",&filter);

    if(fileName!=""){
        if(fileName.contains(".")){ // remove the extension if already there
            fileName = fileName.split(".").at(0);
        }
        default_name = fileName;
        filter.remove("*.");

        QString ret =IROOTPad::SavePlot(fileName,filter);
        ShowMessage(ret,"INFO");
    }
}

void IROOT::SaveWorksheet()
{
    QApplication::setActiveWindow(ui->mdiArea); // This was needed to change the focus to the MdiArea
    QMdiSubWindow *subWin = ui->mdiArea->currentSubWindow();
    if(subWin==NULL){
        if(ui->mdiArea->subWindowList().count()>0){
            ui->mdiArea->setActiveSubWindow(ui->mdiArea->subWindowList().at(0));
            ui->mdiArea->activateWindow();
        }
        subWin=ui->mdiArea->subWindowList().at(0);
    }

    if(subWin!=NULL){
        if(subWin->widget()!=NULL){
            if(subWin->widget()->metaObject()->className()==QTableView::staticMetaObject.className()){
                QString filter;
                static QString default_name="";
                // static int     nSave=1;

                QString fileName= QFileDialog::getSaveFileName(this,"Choose a file name to save worksheet",default_name, "*.txt",&filter);

                if(fileName!=""){
                    if(fileName.contains(".")){ // remove the extension if already there
                        fileName = fileName.split(".").at(0);
                    }
                    default_name = fileName;
                    filter.remove("*.");


                    QTableView *table  = (QTableView*)subWin->widget();
                    IROOTWorksheet *ws = (IROOTWorksheet *)table->model();

                    QString ret =ws->saveData(fileName.append(".txt"),"TXT");
                    ShowMessage(ret,"INFO");
                }
            }


        }
    }
}

void IROOT::SetUniqueLineColor()
{
    if(gPad!=NULL){
        IROOTPad::SetUniqueLineColors();
        gPad->Update();
        gPad->GetCanvas()->Update();
    }

}

void IROOT::SetUniqueMarkerColor()
{
    if(gPad!=NULL){
        IROOTPad::SetUniqueMarkerColors();
        gPad->Update();
        gPad->GetCanvas()->Update();
    }
}

void IROOT::SetUniqueMarkerStyle()
{
    if(gPad!=NULL){
        IROOTPad::SetUniqueMarkerStyles();
        gPad->Update();
        gPad->GetCanvas()->Update();
    }
}

//void IROOT::on_createLegends_clicked()
//{
// IROOTPad::CreateLegends();
//}

void IROOT::CreateLineLegend()
{
    IROOTPad::CreateLegends("l");
}

void IROOT::CreateMarkerLegend()
{
    IROOTPad::CreateLegends("p");
}

void IROOT::CreateLineMarkerLegend()
{
    IROOTPad::CreateLegends("lp");
}

void IROOT::on_gotoWorkspace_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->workingPage);
    ui->leftStackWidget->setCurrentWidget(ui->filesPage);
}

void IROOT::on_viewCompiler_clicked()
{
    CreateCompilerWindow();
}

void IROOT::on_about_clicked()
{
    IROOTAboutBox *about = new IROOTAboutBox(this);
    about->show();
}

void IROOT::on_insertGraphicalCut_clicked()
{
    if (gROOT!=NULL){
        gROOT->SetEditorMode("CutG");
    }
}

void IROOT::on_insertArrow_clicked()
{
    if (gROOT!=NULL){
        gROOT->SetEditorMode("Arrow");
    }
}

void IROOT::on_insertArc_clicked()
{
    if (gROOT!=NULL){
        gROOT->SetEditorMode("Arc");
    }
}

void IROOT::on_insertEllipse_clicked()
{
    if (gROOT!=NULL){
        gROOT->SetEditorMode("Ellipse");
    }
}

void IROOT::on_insertLine_clicked()
{
    if (gROOT!=NULL){
        gROOT->SetEditorMode("Line");
    }
}

void IROOT::on_insertMarker_clicked()
{

    if (gROOT!=NULL){
        gROOT->SetEditorMode("Marker");
    }
}

void IROOT::on_insertText_clicked()
{
    if (gROOT!=NULL && gPad!=NULL){
        bool ok;

        QString text = QInputDialog::getText(this, tr("Enter Text"),tr("Pad text"), QLineEdit::Normal,"", &ok);
        TPaveText * padText = new TPaveText(0.2,0.2,0.4,0.4,"NDC");

        // padText->SetLineColor(0);
        padText->SetFillColor(0);
        padText->SetFillStyle(0);
        padText->SetShadowColor(0);
        padText->SetTextSize(0.1);
        padText->AddText(text.toLatin1().data());
        /*TList *l =*/
        //gPad->GetCanvas()->Update();
        padText->Draw();
        gPad->Update();
    }
}

void IROOT::on_resetInsertMode_clicked()
{
    if (gROOT!=NULL){
        gROOT->SetEditorMode("Modify");
    }
}

void IROOT::on_convertbinarytoTree_clicked()
{
 binaryConvertor->show();
}
