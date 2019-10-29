#include "IROOTPad.h"
//Improves the gPad access


int IROOTPad::xbins =100;
int IROOTPad::ybins =100;
int IROOTPad::zbins =100;


IROOTPad::IROOTPad()
{


}

TH1* IROOTPad::GetAxisHistogram()
{
    TH1* aHist =0;
    if(gPad!=NULL){
        TList *listPrim = gPad->GetListOfPrimitives();
        for(int ii=0;ii<listPrim->GetEntries();ii++){
            TObject *obj =listPrim->At(ii);
            TClass c(obj->ClassName());
            TList *listMethods = c.GetListOfAllPublicMethods();

            if(listMethods->FindObject("GetXaxis") && listMethods->FindObject("GetYaxis") ){ // removed zaxis comparison as graphs do not have one
                TObjArray *objArray = new TObjArray();
                objArray->Expand(0); // needed to shrink the size
                if(obj->InheritsFrom("TH1")){
                    aHist = (TH1*) obj; // type cast: it seems all the object draw axis using TH1
                    break ; //is needed for preventing TGraph which comes last in the list from taking precedence
                }
                else if(obj->ClassName()==TString("THStack")){
                    aHist = ((THStack*) obj)->GetHistogram(); // type cast: it seems all the object draw axis using TH1
                    break;
                }
                else if(obj->ClassName()==TString("TF1")){
                    aHist = ((TF1*)obj)->GetHistogram();
                    break;

                }
                else if(obj->ClassName()==TString("TGraph")){ // added
                    aHist = ((TGraph*)obj)->GetHistogram();
                    break;
                }
                else if(obj->ClassName()==TString("TGraph2D")){ // added
                    aHist = (TH1*)((TGraph2D*)obj)->GetHistogram();
                    break;
                }
                // other objects should be appended here

            }
        }

    }
    return aHist;
}

void IROOTPad::SetXAxisTitle(QString title, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                aHist->SetXTitle(title.toLatin1().data());
                gPad->Update();
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}
void IROOTPad::SetYAxisTitle(QString title, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                aHist->SetYTitle(title.toLatin1().data());
                gPad->Update();
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}
void IROOTPad::SetZAxisTitle(QString title, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                aHist->SetZTitle(title.toLatin1().data());
                gPad->Update();
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}
QString IROOTPad::GetXAxisTitle()
{
    TH1* aHist = GetAxisHistogram();
    if(aHist!=NULL && gPad!=NULL){
        if(aHist->GetXaxis()!=NULL)
            return aHist->GetXaxis()->GetTitle();
    }
}
QString IROOTPad::GetYAxisTitle()
{

    TH1* aHist = GetAxisHistogram();
    if(aHist!=NULL && gPad!=NULL){
        if(aHist->GetYaxis()!=NULL)
            return aHist->GetYaxis()->GetTitle();
    }
}
QString IROOTPad::GetZAxisTitle()
{
    TH1* aHist = GetAxisHistogram();
    if(aHist!=NULL && gPad!=NULL){
        if(aHist->GetZaxis()!=NULL)
            return aHist->GetZaxis()->GetTitle();
    }
}

void IROOTPad::CentreTitle(bool centre, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetXaxis()!=NULL){
                    aHist->GetXaxis()->CenterTitle(centre);
                }
                if(aHist->GetYaxis()!=NULL){
                    aHist->GetYaxis()->CenterTitle(centre);
                }
                if(aHist->GetZaxis()!=NULL){
                    aHist->GetZaxis()->CenterTitle(centre);
                }
                RedrawAxis();

            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}

void IROOTPad::SetLabelOffset(double offset, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL){
                if(aHist->GetXaxis()!=NULL){
                    aHist->GetXaxis()->SetLabelOffset(offset);
                }
                if(aHist->GetYaxis()!=NULL){
                    aHist->GetYaxis()->SetLabelOffset(offset);
                }
                if(aHist->GetZaxis()!=NULL){
                    aHist->GetZaxis()->SetLabelOffset(offset);
                }
                if(gPad!=NULL)gPad->Update();
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}

void IROOTPad::SetTitleOffset(double offse, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL){
                if(aHist->GetXaxis()!=NULL){
                    aHist->GetXaxis()->SetTitleOffset(offse);
                }
                if(aHist->GetYaxis()!=NULL){
                    aHist->GetYaxis()->SetTitleOffset(offse);
                }
                if(aHist->GetZaxis()!=NULL){
                    aHist->GetZaxis()->SetTitleOffset(offse);
                }
                if(gPad!=NULL)gPad->Update();
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}

void IROOTPad::SetTicksX(int major, int minor, int semiminor , bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetXaxis()!=NULL){
                    aHist->GetXaxis()->SetNdivisions(major,minor,semiminor,kFALSE);
                    gPad->Update();
                }
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }

}
void IROOTPad::SetTicksY(int major, int minor, int semiminor , bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetYaxis()!=NULL){
                    aHist->GetYaxis()->SetNdivisions(major,minor,semiminor,kFALSE);
                    gPad->Update();
                }
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }

}
void IROOTPad::SetTicksZ(int major, int minor, int semiminor, bool allPads )
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetZaxis()!=NULL){
                    aHist->GetZaxis()->SetNdivisions(major,minor,semiminor,kFALSE);
                    gPad->Update();
                }
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}

void IROOTPad::SetDecimals(int decimals, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            if(gPad!=NULL){
                TGaxis::SetMaxDigits(decimals);
                RedrawAxis();
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }

}

void IROOTPad::SetXLimits(float begin, float end, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetXaxis()!=NULL){
                    aHist->GetXaxis()->SetLimits(begin,end);
                    aHist->GetXaxis()->SetRangeUser(begin,end);
                    //                    aHist->SetMinimum(begin);
                    //                    aHist->SetMaximum(end);
                    RedrawAxis();

                }
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}

void IROOTPad::SetYLimits(float begin, float end, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetYaxis()!=NULL){
                    aHist->GetYaxis()->SetLimits(begin,end);
                    aHist->GetYaxis()->SetRangeUser(begin,end);
                    RedrawAxis();
                }
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }


}

void IROOTPad::SetZLimits(float begin, float end, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetZaxis()!=NULL){
                    aHist->GetZaxis()->SetLimits(begin,end);
                    aHist->GetZaxis()->SetRangeUser(begin,end);
                    RedrawAxis();
                }
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }

}

void IROOTPad::SetXAxisAsTime(bool set, QString timeformat, QString timeoffset, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetXaxis()!=NULL){
                    if(set==true)
                        SetTimeFormat(aHist->GetXaxis(),timeformat,timeoffset);
                    else
                        SetTimeFormat(aHist->GetXaxis(),"NONE","");
                }
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }



}

void IROOTPad::SetYAxisAsTime(bool set, QString timeformat, QString timeoffset, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL && gPad!=NULL){
                if(aHist->GetYaxis()!=NULL){
                    if(set==true)
                        SetTimeFormat(aHist->GetYaxis(),timeformat,timeoffset);
                    else
                        SetTimeFormat(aHist->GetYaxis(),"NONE","");
                }
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }

}
void IROOTPad::SetTimeFormat(TAxis* a, QString format, QString timeOffset, bool allPads)
{
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){

            if(!allPads) ii=currentPad;

            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(a!=NULL && gPad!=NULL){
                if(format=="NONE"){
                    a->SetTimeDisplay(0);
                }
                else{
                    a->SetTimeFormat(format.append("%F").append(timeOffset).toLatin1().data());

                }
                gPad->Update();
            }
            gPad->GetCanvas()->Update();
            if(!allPads)break;
        }
        gPad->GetCanvas()->cd(currentPad);
    }
}


QStringList IROOTPad::FitData(QString objectName, QString fitFunction, QStringList parNames, QList <double> initValues, QString opt, double rangeMin, double rangeMax, fitResult &fR, bool statsOnPad)
{
    QStringList list;

    static int nFit=0;
    if(gPad!=NULL){
        QByteArray func_name_str=QString("iRootFit%1").arg(nFit++).toLatin1().data();
        QByteArray func_str= fitFunction.toLatin1();
        QByteArray option = opt.toLatin1();

        fR.objectName = objectName;

        int         Nparams = func_str.count("[");
        TObject* object =0;

        if(objectName=="LASTPRIMITIVE"){ // fit the last primitive in the pad
            QStringList list = GetListofFittables(gPad);
            if(list.count()>0){
                objectName      = list.at(list.count()-1);
                fR.objectName   = objectName;
            }
        }
        if(objectName.contains(".")&& objectName.contains("[") && objectName.contains("]")){ //if object id is given instead
            objectName = objectName.split("]").at(0);
            objectName.remove("[");
            int padN = objectName.split(".").at(0).toInt();
            int primitiveN = objectName.split(".").at(1).toInt();
            gPad->GetCanvas()->cd(padN);
            TList *l = gPad->GetCanvas()->GetPad(padN)->GetListOfPrimitives();
            object = l->At(primitiveN);
        }
        else
            object  = gPad->FindObject(objectName.toLatin1().data());

        if(object==NULL){
            list.append(QString("Error: Cannot find object %1 in current pad!").arg(objectName));
            return list;
        }
        else{
            TClass c(object->ClassName());
            TList *l        = c.GetListOfAllPublicMethods();
            TMethod *method =  (TMethod*)l->FindObject("Fit");
            if(method ==NULL){
                list.append(QString("Error: No fit method exists for object %1").arg(objectName));
                return list;
            }

        }
        if(Nparams>10){
            list.append("Error: Number of parameters exceeds 10!");
            return list;
        }



        TFormula formula;
        if(formula.Compile(func_str.data())!=0){
            list.append(QString("Error: Cannot compile function %1").arg(fitFunction));
            return list;
        }

        TF1 *myfit = new TF1(func_name_str.data(),func_str.data(),rangeMin,rangeMax);

        if(myfit==NULL){
            list.append("Error: Fit failed !");
            return list;
        }

        // initializing parameters
        QString     parname;
        QByteArray  parname_str;
        double      initvalue;

        for(int ii=0;ii<myfit->GetNpar();ii++){
            if(ii<parNames.count()){
                parname     = parNames.at(ii);
            }
            else
                parname = QString("p%1").arg(ii);
            parname_str = parname.toLatin1();
            if(ii<initValues.count()){
                initvalue   = initValues.at(ii);
            }
            else
                initvalue   =  0;
            myfit->SetParName(ii,parname_str.data());
            myfit->SetParameter(ii,initvalue);
        }

        //setting the values
        TClass c(object->ClassName());
        TList *l = c.GetListOfAllPublicMethods();
        TMethod *method =  (TMethod*)l->FindObject("Fit");
        if(method !=NULL){
            TObjString *funcName    = new TObjString(func_name_str.data());
            TObjString *options     = new TObjString(option.data());
            TObjString *goptions    = new TObjString("SAME");
            TObjString *min         = new TObjString(QString("%1").arg(rangeMin).toLatin1().data());
            TObjString *max         = new TObjString(QString("%1").arg(rangeMax).toLatin1().data());
            TObjArray *objArray     = new TObjArray();
            objArray->Add(funcName);
            objArray->Add(options);
            objArray->Add(goptions);
            objArray->Add(min);
            objArray->Add(max);
            objArray->Expand(5); // needed to shrink the size
            object->Execute(method,objArray);
            if(!statsOnPad){
                gStyle->SetOptFit(0);
                gROOT->ForceStyle();
            }
            else{
                gStyle->SetOptFit(1111);
                gROOT->ForceStyle();
            }

        }
        // check if its a histogram
        TH1 *hist = dynamic_cast<TH1*> (object);
        if(hist!=NULL){
            fR.entries = hist->GetEntries();
            fR.mean    = hist->GetMean();
            fR.rms     = hist->GetRMS();
        }
        else{
            fR.entries = 0;
            fR.mean    = 0;
            fR.rms     = 0;
        }

        int ii;
        list.append(fR.objectName);

        for(ii=0;ii<myfit->GetNpar();ii++){
            list.append(QString("%1\t%2\t±\t%3").arg(myfit->GetParName(ii)).arg(myfit->GetParameter(ii)).arg(myfit->GetParError(ii)));
            fR.par.append(myfit->GetParameter(ii));
            fR.parErr.append(myfit->GetParError(ii));
        }
        fR.chiSq = myfit->GetChisquare();
        fR.DOF = myfit->GetNDF();
        list.append(QString("Chisq\t%2").arg(myfit->GetChisquare()));
        list.append(QString("NDF\t%2").arg(myfit->GetNDF()));
    }
    return list;
}


QStringList IROOTPad::PlotFunction(QString plotFunction, QStringList parNames, QList<double> initValues, double rangeMin, double rangeMax)
{
    QStringList list;
    QByteArray func_name_str="iRootFit";
    QByteArray func_str= plotFunction.toLatin1();


    TFormula formula;
    if(formula.Compile(func_str.data())!=0){
        list.append(QString("Error: Cannot compile function %1").arg(plotFunction));
        return list;
    }

    TF1 *myfit = new TF1(func_name_str.data(),func_str.data(),rangeMin,rangeMax);

    if(myfit==NULL){
        list.append("Error: Fit failed !");
        return list;
    }

    // initializing parameters
    QString     parname;
    QByteArray  parname_str;
    double      initvalue;

    for(int ii=0;ii<myfit->GetNpar();ii++){
        if(ii<parNames.count()){
            parname     = parNames.at(ii);
        }
        else
            parname = QString("p%1").arg(ii);
        parname_str = parname.toLatin1();
        if(ii<initValues.count()){
            initvalue   = initValues.at(ii);
        }
        else
            initvalue   =  0;
        myfit->SetParName(ii,parname_str.data());
        myfit->SetParameter(ii,initvalue);
    }

    myfit->Draw("l");
    return list;


}


// counts the number of pads in the current pad
int IROOTPad::GetNPads()
{
    //count the number of pads in pad
    if (!gPad) return -1;
    TCanvas* canvas = gPad->GetCanvas();
    if(canvas!=NULL){
        Int_t npads = ((TList*) canvas->GetListOfPrimitives())->GetSize();
        return npads;
    }
    return -1;
}

bool IROOTPad::IsFittable(TObject *obj)
{
    TClass c(obj->ClassName());
    TList *l = c.GetListOfAllPublicMethods();
    TMethod *method =  (TMethod*)l->FindObject("Fit");
    if(method!=NULL)
        return true;
    else return false;
}

// get the list of fittable objects in the pad
QStringList IROOTPad::GetListofFittables(TVirtualPad *pad)
{
    QStringList list;
    TVirtualPad* p = pad;
    if(p==0)p=gPad;
    if(p!=NULL){
        bool containsHist=false;
        bool containsGraph =false;
        for(int ii=0;ii<p->GetListOfPrimitives()->GetEntries();ii++){
            if(IsFittable(p->GetListOfPrimitives()->At(ii))){
                list.append(QString("[%1.%2] %3").arg(p->GetNumber()).arg(ii).arg(p->GetListOfPrimitives()->At(ii)->GetName()));
                if( p->GetListOfPrimitives()->At(ii)->InheritsFrom("TH2")){ containsHist=true;}
                if( p->GetListOfPrimitives()->At(ii)->ClassName()==TString("TGraph")) {containsGraph=true;}
            }
        }
        if(containsHist && containsGraph) list.removeAt(0);
    }

    return list;

}

//// get the current iCanvas
//iCanvas* IROOTPad::GetCurrentCanvas()
//{






//}

int IROOTPad::GetFontIndex(int index, bool italic, bool bold)
{

    //Text font code = 10*fontnumber + precision

    //         1 :       times-medium-i-normal      "Times New Roman"
    //        2 :       times-bold-r-normal        "Times New Roman"
    //        3 :       times-bold-i-normal        "Times New Roman"
    //        4 :       helvetica-medium-r-normal  "Arial"
    //        5 :       helvetica-medium-o-normal  "Arial"
    //        6 :       helvetica-bold-r-normal    "Arial"
    //        7 :       helvetica-bold-o-normal    "Arial"
    //        8 :       courier-medium-r-normal    "Courier New"
    //        9 :       courier-medium-o-normal    "Courier New"
    //       10 :       courier-bold-r-normal      "Courier New"
    //       11 :       courier-bold-o-normal      "Courier New"
    //       12 :       symbol-medium-r-normal     "Symbol"
    //       13 :       times-medium-r-normal      "Times New Roman"
    //       14 :                                  "Wingdings"
    int precision = 2;
    if(index==0){ //times
        if(bold){
            if(italic)
                return 10*3 + precision;
            else
                return 10*2 + precision;
        }
        else{
            if(italic)
                return 10*1 + precision;
            else
                return 10*13 + precision;
        }
    }
    else if(index==1){ //arial
        if(bold){
            if(italic)
                return 10*7 + precision;
            else
                return 10*6 + precision;
        }
        else{
            if(italic)
                return 10*5 + precision;
            else
                return 10*4 + precision;
        }
    }
    else if(index==2){ //courier
        if(bold){
            if(italic)
                return 10*11 + precision;
            else
                return 10*10 + precision;
        }
        else{
            if(italic)
                return 10*9 + precision;
            else
                return 10*8 + precision;
        }
    }
    else if(index==3)//symbol
        return 10*12 + precision;
    else if(index==4)//wingdings
        return 10*14 + precision;
    else
        return 10*13 + precision;
}
void IROOTPad::SetFont(int font, QString objectType){
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){
            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL){
                if(objectType=="axis"){
                    aHist->SetLabelFont(font,"XYZ");
                    //gStyle->SetLabelFont(font,"XYZ");
                }
                else if(objectType=="title"){
                    aHist->SetTitleFont(font,"XYZT");
                    //gStyle->SetTitleFont(font,"XYZT");
                    TPaveText *t = dynamic_cast<TPaveText*> (gPad->FindObject("title"));
                    if(t){
                        t->SetTextFont(font);
                    }
                }
                else if(objectType=="stats"){
                    TPaveStats *s = dynamic_cast<TPaveStats*> (aHist->FindObject("stats"));
                    if(s){
                        s->SetTextFont(font);
                        //gStyle->SetStatFont(font);

                    }
                }
                else if(objectType=="legend"){
                    TLegend* l = (TLegend*)(gPad->FindObject("TPave"));
                    if(l){
                        l->SetTextFont(font);
                        //gStyle->SetLegendFont(font);

                    }

                }
                gPad->Update();
                gPad->GetCanvas()->cd(currentPad);
            }

        }
    }
}

void IROOTPad::SetFontSize(double size, QString objectType){
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){
            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            if(aHist!=NULL){
                if(objectType=="axis"){
                    aHist->SetLabelSize(size,"XYZ");
                    //gStyle->SetLabelSize(size,"XYZ");
                    gPad->Update();
                }
                else if(objectType=="title"){
                    aHist->SetTitleSize(size,"XYZT");
                    //gStyle->SetTitleSize(size,"XYZT");
                    TPaveText *t = dynamic_cast<TPaveText*> (gPad->FindObject("title"));
                    if(t){
                        t->SetTextSize(size);
                    }
                    gPad->Update();
                }
                else if(objectType=="stats"){
                    TPaveStats *s = dynamic_cast<TPaveStats*> (aHist->FindObject("stats"));
                    if(s){
                        s->SetTextSize(size);
                        //gStyle->SetStatFontSize(size);
                        gPad->Update();
                    }
                }
                else if(objectType=="legend"){
                    TLegend* l = (TLegend*)(gPad->FindObject("TPave"));
                    if(l){
                        l->SetTextSize(size);
                        //gStyle->SetTextSize(size);
                        gPad->Update();
                    }

                }
                gPad->Update();
                gPad->GetCanvas()->cd(currentPad);
            }

        }

    }
}

void IROOTPad::SetFontColor(int red, int green, int blue, QString objectType){
    if(gPad!=NULL){
        int nPads       = GetNPads();
        int currentPad  = gPad->Pad()->GetNumber();
        for(int ii=0;ii<nPads+1;ii++){
            gPad->GetCanvas()->cd(ii);
            TH1* aHist = GetAxisHistogram();
            TColor c;
            int color = c.GetColor(red, green, blue);
            if(aHist!=NULL){
                if(objectType=="axis"){
                    aHist->SetLabelColor(color,"XYZT");
                    gPad->Update();
                }
                else if(objectType=="title"){
                    //aHist->SetTitle(color,"XYZT");
                    if(aHist->GetXaxis())
                        aHist->GetXaxis()->SetTitleColor(color);
                    if(aHist->GetYaxis())
                        aHist->GetYaxis()->SetTitleColor(color);
                    if(aHist->GetZaxis())
                        aHist->GetZaxis()->SetTitleColor(color);
                    TPaveText *t = dynamic_cast<TPaveText*> (gPad->FindObject("title"));
                    if(t){
                        t->SetTextColor(color);
                    }
                    gPad->Update();
                }
                else if(objectType=="stats"){
                    TPaveStats *s = dynamic_cast<TPaveStats*> (aHist->FindObject("stats"));
                    if(s){
                        s->SetTextColor(color);
                        gPad->Update();
                    }
                }
                else if(objectType=="legend"){
                    TLegend* l = (TLegend*)(gPad->FindObject("TPave"));
                    if(l){
                        l->SetTextColor(color);
                        gPad->Update();
                    }

                }
                gPad->Update();
                gPad->GetCanvas()->cd(currentPad);
            }

        }

    }
}

void IROOTPad::RedrawAxis()
{
    if(gPad){
        gPad->RedrawAxis();
        RemoveLastPrimitive();
        gPad->Update();
    }

}
void IROOTPad::RemoveLastPrimitive()
{
    if(gPad){
        int nprim = gPad->GetListOfPrimitives()->GetEntries();
        gPad->GetListOfPrimitives()->At(nprim-1)->Delete();
    }
}

QString IROOTPad::SavePlot(QString filename, QString file_extension)
{
    bool saveAll=false;
    if(gPad==NULL){ // check for active canvas
        return "Error: No active canvases";
    }
    gPad->GetCanvas()->Update();
    gPad->GetCanvas()->SetName("iRoot");
    if(saveAll)
        gPad->GetCanvas()->Print(filename.toLatin1().data(),file_extension.toLatin1().data());
    else
        gPad->GetCanvas()->Print(filename.append(".").append(file_extension).toLatin1().data(),file_extension.toLatin1().data());
    return QString("Plot saved in file %1").arg(filename);
}

void IROOTPad::SetUniqueLineColors()
{

    if(gPad!=NULL){
        TList *listPrim = gPad->GetListOfPrimitives();
        int color=1;
        for(int ii=0;ii<listPrim->GetEntries();ii++){
            TObject *obj =listPrim->At(ii);
            if(obj->InheritsFrom("TH1")){
                TH1* h  = dynamic_cast <TH1*> (obj);
                if(h!=NULL)
                    h->SetLineColor(color++);
            }
            else if(obj->ClassName()==TString("TGraph")){ // added
                TGraph *h = dynamic_cast <TGraph*> (obj);
                if(h!=NULL)
                    h->SetLineColor(color++);
            }
            else if(obj->ClassName()==TString("TGraph2D")){ // added
                TGraph2D *h = dynamic_cast <TGraph2D*> (obj);
                if(h!=NULL)
                    h->SetLineColor(color++);
            }
            if(color==10)color=30;
            if(color==40)color=1;
        }
    }
}

void IROOTPad::SetUniqueMarkerColors()
{
    if(gPad!=NULL){
        TList *listPrim = gPad->GetListOfPrimitives();
        int color=1;
        for(int ii=0;ii<listPrim->GetEntries();ii++){
            TObject *obj =listPrim->At(ii);
            if(obj->InheritsFrom("TH1")){
                TH1* h  = dynamic_cast <TH1*> (obj);
                if(h!=NULL)
                    h->SetMarkerColor(color++);
            }
            else if(obj->ClassName()==TString("TGraph")){ // added
                TGraph *h = dynamic_cast <TGraph*> (obj);
                if(h!=NULL)
                    h->SetMarkerColor(color++);
            }
            else if(obj->ClassName()==TString("TGraph2D")){ // added
                TGraph2D *h = dynamic_cast <TGraph2D*> (obj);
                if(h!=NULL)
                    h->SetMarkerColor(color++);
            }
            if(color==10)color=30;
            if(color==40)color=1;
        }
    }
}

void IROOTPad::SetUniqueMarkerStyles()
{
    if(gPad!=NULL){
        TList *listPrim = gPad->GetListOfPrimitives();
        int style=2;
        for(int ii=0;ii<listPrim->GetEntries();ii++){
            TObject *obj =listPrim->At(ii);
            if(obj->InheritsFrom("TH1")){
                TH1* h  = dynamic_cast <TH1*> (obj);
                if(h!=NULL)
                    h->SetMarkerStyle(style++);
            }
            else if(obj->ClassName()==TString("TGraph")){ // added
                TGraph *h = dynamic_cast <TGraph*> (obj);
                if(h!=NULL)
                    h->SetMarkerStyle(style++);
            }
            else if(obj->ClassName()==TString("TGraph2D")){ // added
                TGraph2D *h = dynamic_cast <TGraph2D*> (obj);
                if(h!=NULL)
                    h->SetMarkerStyle(style++);
            }
            if(style==6)style=8;
            if(style==9)style=21;
            if(style==34)style=2;
        }
    }
}

void IROOTPad::CreateLegends(QString type)
{
    //if(ui->buildLegends->isChecked())
    {

        if(gPad!=NULL){
            TList *lop=gPad->GetListOfPrimitives();
            if (!lop) return ;
            TLegend *leg=0;
            TIter next(lop);
            TString mes;
            TObject *o=0;
            while( (o=next()) ) {
                if((o->InheritsFrom(TAttLine::Class()) || o->InheritsFrom(TAttMarker::Class()) ||
                    o->InheritsFrom(TAttFill::Class())) &&
                        ( !(o->InheritsFrom(TFrame::Class())) && !(o->InheritsFrom(TPave::Class())) )) {
                    if (!leg) leg = new TLegend(0.5, 0.67, 0.88, 0.88, "");
                    if (o->InheritsFrom(TNamed::Class()) && strlen(((TNamed *)o)->GetTitle()))
                        mes = ((TNamed *)o)->GetTitle();
                    else if (strlen(o->GetName()))
                        mes = o->GetName();
                    else
                        mes = o->ClassName();
                    TString opt("");
                    opt = type.toLatin1().data();
//                    if (o->InheritsFrom(TAttLine::Class()))   opt += "l";
//                    if (o->InheritsFrom(TAttMarker::Class())) opt += "p";
//                    if (o->InheritsFrom(TAttFill::Class()))   opt += "f";
                    leg->AddEntry(o,mes.Data(),opt.Data());
                } else if ( o->InheritsFrom(TMultiGraph::Class() ) ) {
                    if (!leg) leg = new TLegend(0.5, 0.67, 0.88, 0.88, "");
                    TList * grlist = ((TMultiGraph *)o)->GetListOfGraphs();
                    TIter nextgraph(grlist);
                    TGraph * gr;
                    TObject * obj;
                    while ((obj = nextgraph())) {
                        gr = (TGraph*) obj;
                        if      (strlen(gr->GetTitle())) mes = gr->GetTitle();
                        else if (strlen(gr->GetName()))  mes = gr->GetName();
                        else                             mes = gr->ClassName();
                        leg->AddEntry( obj, mes.Data(), "lpf" );
                    }
                } else if ( o->InheritsFrom(THStack::Class() ) ) {
                    if (!leg) leg = new TLegend(0.5, 0.67, 0.88, 0.88, "");
                    TList * hlist = ((THStack *)o)->GetHists();
                    TIter nexthist(hlist);
                    TH1 * hist;
                    TObject * obj;
                    while ((obj = nexthist())) {
                        hist = (TH1*) obj;
                        if      (strlen(hist->GetTitle())) mes = hist->GetTitle();
                        else if (strlen(hist->GetName()))  mes = hist->GetName();
                        else                               mes = hist->ClassName();
                        leg->AddEntry( obj, mes.Data(), "lpf" );
                    }
                }
            }
            if (leg) {
                TVirtualPad *gpadsave;
                gpadsave = gPad;
                gPad->cd();
                leg->Draw();
                leg->SetTextFont(gStyle->GetLegendFont());
                leg->SetFillColor(gStyle->GetLegendFillColor());
                leg->SetBorderSize(gStyle->GetLegendBorderSize());
                gpadsave->cd();
                //  RefreshListofObjects();
            }


        }
    }


}
