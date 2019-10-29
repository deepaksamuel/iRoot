#ifndef IROOTTREEFORMULACHECKER_H
#define IROOTTREEFORMULACHECKER_H

#include <QString>
#include <QDebug>
#include "TTreeFormula.h"
#include "TROOT.h"
class IROOTTreeFormulaChecker : public TTreeFormula
{
public:
    IROOTTreeFormulaChecker(const char* name, const char* formula, TTree* tree);
    void Analyze(const char *schain, Int_t &err, Int_t offset);
    QString GetErrorString();
private:
    QString fError;

};

#endif // IROOTTREEFORMULACHECKER_H
