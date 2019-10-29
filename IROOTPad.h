#ifndef IROOTPAD_H
#define IROOTPAD_H

#include <TCanvas.h>
#include <TH1.h>
#include <TGaxis.h>
#include <TList.h>
#include <TClass.h>
#include <THStack.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TLegend.h>
#include <TColor.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TFrame.h>
#include <TMultiGraph.h>


#include <QString>
#include <QStringList>
#include <QTimer>
#include <QDebug>

typedef struct {
    QString         fileName;///< file name
    int             loopN;///< current loop number
    QString         drawString;///< current draw string
    QString         cutString;///< Chi.sq value using the pattern matching
    QString         fitFunction;///< fitFunction
    QString         objectName;///< object name
    QString         fitOptions;///< fit options
    int             DOF;       ///< degrees of freedom
    double          chiSq;      ///< chi square
    QList<double>     par;  ///< the value of parameters
    QList<double>     parErr; ///< the value of errors
    double          entries; ///< the entries if histogram
    double          mean;///< the statistical mean if histogram
    double          rms;///< the statistical rms if histogram
} fitResult;



class IROOTPad
{



public:
    IROOTPad();
    ~IROOTPad(){}
    static      void       SetXAxisTitle(QString title, bool allPads=false);
    static      void       SetYAxisTitle(QString title, bool allPads=false);
    static      void       SetZAxisTitle(QString title, bool allPads=false);
    static     QString     GetXAxisTitle();
    static     QString     GetYAxisTitle();
    static     QString     GetZAxisTitle();
    static     void        CentreTitle(bool centre, bool allPads=false);
    static     void        SetLabelOffset(double offset,bool allPads=false);
    static     void        SetTitleOffset(double offse, bool allPads=false);
    static     void        SetTicksX(int major, int minor, int semiminor, bool allPads=false );
    static     void        SetTicksY(int major, int minor, int semiminor, bool allPads=false );
    static     void        SetTicksZ(int major, int minor, int semiminor, bool allPads=false );
    //static     void        SetTicks(int major, int minor, int semiminor, bool allPads=false);
    static     void        SetDecimals(int decimals, bool allPads=false);
    static     void        SetXLimits(float begin, float end, bool allPads=false);
    static     void        SetYLimits(float begin, float end, bool allPads=false);
    static     void        SetZLimits(float begin, float end, bool allPads=false);
    static     void        SetXAxisAsTime(bool set=false, QString timeformat="", QString timeoffset="", bool allPads=false);
    static     void        SetYAxisAsTime(bool set=false, QString timeformat="", QString timeoffset="", bool allPads=false);
    static     void        SetTimeFormat(TAxis* a, QString format, QString timeOffset, bool allPads=false);
    static     TH1*        GetAxisHistogram();
    static     QStringList FitData(QString objectName, QString fitFunction, QStringList parNames, QList<double> initValues, QString options, double rangeMin, double rangeMax, fitResult &fR, bool statsOnPad=false);
    static     QStringList PlotFunction(QString plotFunction, QStringList parName, QList<double> initValues, double rangeMin, double rangeMax );
    static     int         GetNPads();
    static     bool        IsFittable(TObject *);
    static     QStringList GetListofFittables(TVirtualPad *pad);
    static     void         SetFont(int font, QString objectType);
    static     int          GetFontIndex(int index, bool italic, bool bold);
    static     void         SetFontSize(double size, QString objectType);
    static     void         SetFontColor(int red, int green, int blue, QString objectType);
    static     void         RedrawAxis();
    static     void         RemoveLastPrimitive();
    static     QString      SavePlot(QString filename, QString file_extension);
    static     void         SetUniqueLineColors();
    static     void         SetUniqueMarkerColors();
    static     void         SetUniqueMarkerStyles();
    static     void         CreateLegends(QString opt="l");
    static      int         xbins;
    static      int         ybins;
    static      int         zbins;

protected:

private:



};

#endif // IROOTPAD_H
