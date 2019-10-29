#-------------------------------------------------
#
# Project created by QtCreator 2012-11-02T17:13:07
#
#-------------------------------------------------

QT       += core gui
CONFIG +=console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IROOTNEW
TEMPLATE = app
INCLUDEPATH += $$(ROOTSYS)/include

SOURCES += main.cpp\
        IROOT.cpp \
    iCanvas.cpp \
    IROOTMessageBox.cpp \
    IROOTAxisEditor.cpp \
    IROOTPad.cpp \
    IROOTFitOptions.cpp \
    IROOTFittingObjects.cpp \
    IROOTLoopModule.cpp \
    IROOTMultiDirectorySearch.cpp \
    IROOTAsciiFileConvertor.cpp \
    IROOTHistogramBinning.cpp \
    IROOTWorksheetImporter.cpp \
    IROOTPlotOptionsEntry.cpp \
    IROOTTree.cpp \
    IROOTWorksheet.cpp \
    IROOTCompiler.cpp \
    IROOTSyntaxHighlighter.cpp \
    IROOTTreeFormulaChecker.cpp \
    IROOTAboutBox.cpp \
    IROOTBinaryFileConvertor.cpp

HEADERS  += IROOT.h \
    iCanvas.h \
    IROOTMessageBox.h \
    IROOTAxisEditor.h \
    IROOTPad.h \
    IROOTFitOptions.h \
    IROOTFittingObjects.h \
    IROOTLoopModule.h \
    IROOTMultiDirectorySearch.h \
    IROOTAsciiFileConvertor.h \
    IROOTHistogramBinning.h \
    IROOTWorksheetImporter.h \
    IROOTPlotOptionsEntry.h \
    IROOTTree.h \
    IROOTWorksheet.h \
    IROOTCompiler.h \
    IROOTSyntaxHighlighter.h \
    IROOTTreeFormulaChecker.h \
    IROOTAboutBox.h \
    IROOTBinaryFileConvertor.h

FORMS    += IROOT.ui \
    IROOTMessageBox.ui \
    IROOTAxisEditor.ui \
    IROOTFitOptions.ui \
    IROOTFittingObjects.ui \
    IROOTLoopModule.ui \
    IROOTMultiDirectorySearch.ui \
    IROOTAsciiFileConvertor.ui \
    IROOTHistogramBinning.ui \
    IROOTWorksheetImporter.ui \
    IROOTPlotOptionsEntry.ui \
    IROOTCompiler.ui \
    IROOTAboutBox.ui \
    IROOTBinaryFileConvertor.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    README.txt \
    myapp.rc \
    appicon.ico \
    License.txt \
    Version.txt \
    installer/installer.iss
LIBS+=C:/root/lib/libCore.lib\
C:/root/lib/libCint.lib\
C:/root/lib/libRIO.lib\
C:/root/lib/libNet.lib\
C:/root/lib/libHist.lib\
C:/root/lib/libGraf.lib\
C:/root/lib/libGraf3d.lib\
C:/root/lib/libGpad.lib\
C:/root/lib/libTree.lib\
C:/root/lib/libTreePlayer.lib\
C:/root/lib/libRint.lib\
C:/root/lib/libPostscript.lib\
C:/root/lib/libMatrix.lib\
C:/root/lib/libPhysics.lib\
C:/root/lib/libMathCore.lib\
C:/root/lib/libThread.lib\
C:/root/lib/libProof.lib\
C:/root/lib/libSpectrum.lib\
C:/root/lib/libGui.lib\
C:/root/lib/libMinuit.lib

RC_FILE = myapp.rc
