#include "IROOTTreeFormulaChecker.h"
static Int_t gMAXOP=1000,gMAXPAR=1000,gMAXCONST=1000;
const Int_t  gMAXSTRINGFOUND = 10;
const UInt_t kOptimizationError = BIT(19);

IROOTTreeFormulaChecker::IROOTTreeFormulaChecker(const char *name, const char *formula, TTree *tree)
    :TTreeFormula(name, formula, tree)
{

}

void IROOTTreeFormulaChecker::Analyze(const char *schain, Int_t &err, Int_t offset){
    Int_t valeur,find,n,i,j,k,lchain,nomb,virgule,inter,nest;
    Int_t compt,compt2,compt3,compt4;
    Bool_t inString;
    Double_t vafConst;
    ULong_t vafConst2;
    Bool_t parenthese;
    TString s,chaine_error,chaine1ST;
    TString s1,s2,s3,ctemp;

    TString chaine = schain;
    TFormula *oldformula;
    Int_t modulo,plus,puiss10,puiss10bis,moins,multi,divi,puiss,et,ou,petit,grand,egal,diff,peteg,grdeg,etx,oux,rshift,lshift,tercond,terelse;
    char t;
    TString slash("/"), escapedSlash("\\/");
    Int_t inter2 = 0;
    SetNumber(0);
    Int_t actionCode,actionParam;
    Int_t err_hint = 0;

    //*-*- Verify correct matching of parenthesis and remove unnecessary parenthesis.
    //*-*  ========================================================================
    lchain = chaine.Length();
    //if (chaine(lchain-2,2) == "^2") chaine = "sq(" + chaine(0,lchain-2) + ")";
    parenthese = kTRUE;
    lchain = chaine.Length();
    while (parenthese && lchain>0 && err==0){
        compt  = 0;
        compt2 = 0;
        inString = false;
        lchain = chaine.Length();
        if (lchain==0) err=4;
        else {
            for (i=1; i<=lchain; ++i) {
                if (chaine(i-1,1) == "\"") inString = !inString;
                if (!inString) {
                    if (chaine(i-1,1) == "[") compt2++;
                    if (chaine(i-1,1) == "]") compt2--;
                    if (chaine(i-1,1) == "(") compt++;
                    if (chaine(i-1,1) == ")") compt--;
                }
                if (compt < 0) err = 40; // more open parentheses than close parentheses
                if (compt2< 0) err = 42; // more ] than [
                if (compt==0 && (i!=lchain || lchain==1)) parenthese = kFALSE;
                // if (lchain<3 && chaine(0,1)!="(" && chaine(lchain-1,1)!=")") parenthese = kFALSE;
            }
            if (compt > 0) err = 41; // more ( than )
            if (compt2> 0) err = 43; // more [ than ]
            if (parenthese) chaine = chaine(1,lchain-2);
        }
    } // while parantheses

    if (lchain==0) err=4; // empty string
    modulo=plus=moins=multi=divi=puiss=et=ou=petit=grand=egal=diff=peteg=grdeg=etx=oux=rshift=lshift=tercond=terelse=0;

    //*-*- Look for simple operators
    //*-*  =========================

    if (err==0) {
        compt = compt2 = compt3 = compt4 = 0;puiss10=0;puiss10bis = 0;
        inString = false;
        j = lchain;
        Bool_t isdecimal = 1; // indicates whether the left part is decimal.

        for (i=1;i<=lchain; i++) {

            puiss10=puiss10bis=0;
            if (i>2) {
                t = chaine[i-3];
                isdecimal = isdecimal && (strchr("0123456789.",t)!=0);
                if (isdecimal) {
                    if ( chaine[i-2] == 'e' || chaine[i-2] == 'E' ) puiss10 = 1;
                } else if ( strchr("+-/[]()&|><=!*/%^\\",t) ) {
                    isdecimal = 1; // reset after delimiter
                }
            }
            if (j>2) {
                if (chaine[j-2] == 'e' || chaine[j-2] == 'E') {
                    Bool_t isrightdecimal = 1;

                    for(k=j-3; k>=0 && isrightdecimal; --k) {
                        t = chaine[k];
                        isrightdecimal = isrightdecimal && (strchr("0123456789.",t)!=0);
                        if (!isrightdecimal) {
                            if (strchr("+-/[]()&|><=!*/%^\\",t)!=0) {
                                puiss10bis = 1;
                            }
                        }
                    }
                    if (k<0 && isrightdecimal)  puiss10bis = 1;
                }
            }
            if (puiss10 && (i<=lchain)) {
                t = chaine[i];
                puiss10 = (strchr("0123456789.",t)!=0);
            }
            if (puiss10bis && (j<=lchain)) {
                t = chaine[j];
                puiss10bis = (strchr("0123456789.",t)!=0);
            }

            if (chaine(i-1,1) == "\"") inString = !inString;
            if (inString) continue;
            if (chaine(i-1,1) == "[") compt2++;
            if (chaine(i-1,1) == "]") compt2--;
            if (chaine(i-1,1) == "(") compt++;
            if (chaine(i-1,1) == ")") compt--;
            if (chaine(j-1,1) == "[") compt3++;
            if (chaine(j-1,1) == "]") compt3--;
            if (chaine(j-1,1) == "(") compt4++;
            if (chaine(j-1,1) == ")") compt4--;
            if (chaine(i-1,2)=="&&" && !inString && compt==0 && compt2==0 && et==0) {et=i;puiss=0;}
            if (chaine(i-1,2)=="||" && compt==0 && compt2==0 && ou==0) {puiss10=0; ou=i;}
            if (chaine(i-1,1)=="&"  && compt==0 && compt2==0 && etx==0) {etx=i;puiss=0;}
            if (chaine(i-1,1)=="|"  && compt==0 && compt2==0 && oux==0) {puiss10=0; oux=i;}
            if (chaine(i-1,2)==">>" && compt==0 && compt2==0 && rshift==0) {puiss10=0; rshift=i;}
            if (chaine(i-1,1)==">"  && compt==0 && compt2==0 && rshift==0 && grand==0)
            {puiss10=0; grand=i;}
            if (chaine(i-1,2)=="<<" && compt==0 && compt2==0 && lshift==0) {puiss10=0; lshift=i;}
            if (chaine(i-1,1)=="<"  && compt==0 && compt2==0 && lshift==0 && petit==0)
            {puiss10=0; petit=i;
                // Check whether or not we have a template names! (actually this can
                // only happen in TTreeFormula.
                for(int ip = i,depth=0; ip < lchain; ++ip) {
                    char c = chaine(ip);
                    // The characteres allowed in the template parameter are alpha-numerical characters,
                    // underscores, comma, <, > and scope operator.
                    if (isalnum(c) || c=='_' || c==',') continue;
                    if (c==':' && chaine(ip+1)==':') { ++ip; continue; }
                    if (c=='<') { ++depth; continue; }
                    if (c=='>') {
                        if (depth) { --depth; continue; }
                        else {
                            // We reach the end of the template parameter.
                            petit = 0;
                            i = ip+1;
                            break;
                        }
                    }
                    // Character not authorized within a template parameter
                    break;
                }
                if (petit==0) {
                    // We found a template parameter and modified i
                    continue; // the for(int i ,...)
                }
            }
            if ((chaine(i-1,2)=="<=" || chaine(i-1,2)=="=<") && compt==0 && compt2==0
                    && peteg==0) {peteg=i; puiss10=0; petit=0;}
            if ((chaine(i-1,2)=="=>" || chaine(i-1,2)==">=") && compt==0 && compt2==0
                    && grdeg==0) {puiss10=0; grdeg=i; grand=0;}
            if (chaine(i-1,2) == "==" && compt == 0 && compt2 == 0 && egal == 0) {puiss10=0; egal=i;}
            if (chaine(i-1,2) == "!=" && compt == 0 && compt2 == 0 && diff == 0) {puiss10=0; diff=i;}
            if (i>1 && chaine(i-1,1) == "+" && compt == 0 && compt2 == 0 && puiss10==0) plus=i;
            if (chaine(j-1,1) == "-" && chaine(j-2,1) != "*" && chaine(j-2,1) != "/"
                    && chaine(j-2,1)!="^" && compt3==0 && compt4==0 && moins==0 && puiss10bis==0) moins=j;
            if (chaine(i-1,1)=="%" && compt==0 && compt2==0 && modulo==0) {puiss10=0; modulo=i;}
            if (chaine(i-1,1)=="*" && compt==0 && compt2==0 && multi==0)  {puiss10=0; multi=i;}
            if (chaine(j-1,1)=="/" && chaine(j-2,1)!="\\"
                    && compt4==0 && compt3==0 && divi==0)
            {
                puiss10=0; divi=j;
            }
            if (chaine(j-1)=='^' && compt4==0 && compt3==0 && puiss==0) {puiss10=0; puiss=j;}
            if (chaine(i-1)=='?' && compt == 0 && compt2 == 0 && tercond == 0) {puiss10=0; tercond=i;}
            if (chaine(i-1)==':' && tercond && compt == 0 && compt2 == 0 && terelse == 0) {
                if (i>2 && chaine(i-2)!=':' && chaine(i)!=':') {
                    puiss10=0; terelse=i;
                }
            }

            j--;
        }

        //*-*- If operator found, analyze left and right part of the statement
        //*-*  ===============================================================

        actionParam = 0;
        if (tercond && terelse) {
            if (tercond == 1 || terelse == lchain || tercond == (terelse-1) ) {
                err = 5;
                chaine_error = "?:";
            } else {
                // Condition
                ctemp = chaine(0,tercond-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;

                fExpr[fNoper] = "?: condition jump";
                actionCode = kJumpIf;
                actionParam = 0;
                SetAction(fNoper,actionCode, actionParam);
                Int_t optloc = fNoper++;

                // Expression executed if condition is true.
                ctemp = chaine(tercond,terelse-tercond-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                actionParam = fNoper; // We want to skip the next instruction ('else jump'), so we set the param to the current cursor and the next instruction will be skip by the ++i in the eval loop
                SetAction(optloc, actionCode, actionParam);

                fExpr[fNoper] = "?: else jump";
                actionCode = kJump;
                actionParam = 0;
                // Set jump target.
                SetAction(fNoper,actionCode, actionParam);
                optloc = fNoper++;

                // Expression executed if condition is false.
                ctemp = chaine(terelse,lchain-terelse);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                // Set jump target.
                actionParam = fNoper - 1; // We need to not skip the next instruction, so we compensate for the ++i in the eval loop
                SetAction(optloc, actionCode, actionParam);

                if (IsString(optloc-1) != IsString(fNoper-1)) {
                    err = 45;
                    chaine_error = "?:";
                }
            }
        } else if (ou != 0) {    //check for ||
            if (ou==1 || ou==lchain-1) {
                err=5;
                chaine_error="||";
            }
            else {
                ctemp = chaine(0,ou-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;

                fExpr[fNoper] = "|| checkpoint";
                actionCode = kBoolOptimize;
                actionParam = 2;
                SetAction(fNoper,actionCode, actionParam);
                Int_t optloc = fNoper++;

                ctemp = chaine(ou+1,lchain-ou-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "||";
                actionCode = kOr;
                SetAction(fNoper,actionCode, 0);

                SetAction( optloc, GetAction(optloc), GetActionParam(optloc) + (fNoper-optloc) * 10);
                fNoper++;
                if (!CheckOperands(optloc-1,fNoper-1,err)) return;
            }
        } else if (et!=0) {
            if (et==1 || et==lchain-1) {
                err=5;
                chaine_error="&&";
            }
            else {
                ctemp = chaine(0,et-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;

                fExpr[fNoper] = "&& checkpoint";
                actionCode = kBoolOptimize;
                actionParam = 1;
                SetAction(fNoper,actionCode,actionParam);

                Int_t optloc = fNoper++;

                ctemp = chaine(et+1,lchain-et-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "&&";
                actionCode = kAnd;
                SetAction(fNoper,actionCode,0);

                SetAction(optloc, GetAction(optloc), GetActionParam(optloc) + (fNoper-optloc) * 10);
                fNoper++;
                if (!CheckOperands(optloc-1,fNoper-1,err)) return;
            }
        } else if (oux!=0) {
            if (oux==1 || oux==lchain) {
                err=5;
                chaine_error="|";
            }
            else {
                ctemp = chaine(0,oux-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                UInt_t leftopr = fNoper-1;
                ctemp = chaine(oux,lchain-oux);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "|";
                actionCode = kBitOr;
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
                if (!CheckOperands(leftopr,fNoper-1,err)) return;
            }
        } else if (etx!=0) {
            if (etx==1 || etx==lchain) {
                err=5;
                chaine_error="&";
            }
            else {
                ctemp = chaine(0,etx-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                UInt_t leftopr = fNoper-1;
                ctemp = chaine(etx,lchain-etx);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "&";
                actionCode = kBitAnd;
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
                if (!CheckOperands(leftopr,fNoper-1,err)) return;
            }
        } else if (petit != 0) {
            if (petit==1 || petit==lchain) {
                err=5;
                chaine_error="<";
            }
            else {
                ctemp = chaine(0,petit-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                UInt_t leftopr = fNoper-1;
                ctemp = chaine(petit,lchain-petit);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "<";
                actionCode = kLess;
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
                if (!CheckOperands(leftopr,fNoper-1,err)) return;
            }
        } else if (grand != 0) {
            if (grand==1 || grand==lchain) {
                err=5;
                chaine_error=">";
            }
            else {
                ctemp = chaine(0,grand-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                UInt_t leftopr = fNoper-1;
                ctemp = chaine(grand,lchain-grand);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = ">";
                actionCode = kGreater;
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
                if (!CheckOperands(leftopr,fNoper-1,err)) return;
            }
        } else if (peteg != 0) {
            if (peteg==1 || peteg==lchain-1) {
                err=5;
                chaine_error="<=";
            }
            else {
                ctemp = chaine(0,peteg-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                ctemp = chaine(peteg+1,lchain-peteg-1);
                UInt_t leftopr = fNoper-1;
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "<=";
                actionCode = kLessThan;
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
                if (!CheckOperands(leftopr,fNoper-1,err)) return;
            }
        } else if (grdeg != 0) {
            if (grdeg==1 || grdeg==lchain-1) {
                err=5;
                chaine_error="=>";
            }
            else {
                ctemp = chaine(0,grdeg-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                UInt_t leftopr = fNoper-1;
                ctemp = chaine(grdeg+1,lchain-grdeg-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = ">=";
                actionCode = kGreaterThan;
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
                if (!CheckOperands(leftopr,fNoper-1,err)) return;
            }
        } else if (egal != 0) {
            if (egal==1 || egal==lchain-1) {
                err=5;
                chaine_error="==";
            }
            else {
                ctemp = chaine(0,egal-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                Int_t optloc = fNoper-1;

                ctemp = chaine(egal+1,lchain-egal-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "==";
                actionCode = kEqual;

                Bool_t isstring = IsString(fNoper-1);
                if (IsString(optloc) != isstring) {
                    err = 45;
                    chaine_error = "==";
                } else if (isstring) {
                    actionCode = kStringEqual;
                }
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
            }
        } else if (diff != 0) {
            if (diff==1 || diff==lchain-1) {
                err=5;
                chaine_error = "!=";
            }
            else {
                ctemp = chaine(0,diff-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                Int_t optloc = fNoper-1;

                ctemp = chaine(diff+1,lchain-diff-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "!=";
                actionCode = kNotEqual;

                Bool_t isstring = IsString(fNoper-1);
                if (IsString(optloc) != isstring) {
                    err = 45;
                    chaine_error = "!=";
                } else if (isstring) {
                    actionCode = kStringNotEqual;
                }
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
            }
        } else if (plus != 0) {
            if (plus==lchain) {
                err=5;
                chaine_error = "+";
            }
            else {
                ctemp = chaine(0,plus-1);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                UInt_t leftopr = fNoper-1;
                ctemp = chaine(plus,lchain-plus);
                Analyze(ctemp.Data(),err,offset); if (err) return;
                fExpr[fNoper] = "+";
                actionCode = kAdd;
                SetAction(fNoper,actionCode,actionParam);
                fNoper++;
                if (!CheckOperands(leftopr,fNoper-1,err)) return;
            }
        } else {
            if (moins != 0) {
                if (moins == 1) {
                    ctemp = chaine(moins,lchain-moins);
                    Analyze(ctemp.Data(),err,offset); if (err) return;
                    fExpr[fNoper] = "-";
                    actionCode = kSignInv;
                    SetAction(fNoper,actionCode,actionParam);
                    ++fNoper;
                    if (!CheckOperands(fNoper-1,err)) return;
                } else {
                    if (moins == lchain) {
                        err=5;
                        chaine_error = "-";
                    } else {
                        ctemp = chaine(0,moins-1);
                        Analyze(ctemp.Data(),err,offset); if (err) return;
                        UInt_t leftopr = fNoper-1;
                        ctemp = chaine(moins,lchain-moins);
                        Analyze(ctemp.Data(),err,offset); if (err) return;
                        fExpr[fNoper] = "-";
                        actionCode = kSubstract;
                        SetAction(fNoper,actionCode,actionParam);
                        fNoper++;
                        if (!CheckOperands(leftopr,fNoper-1,err)) return;
                    }
                }
            } else if (modulo != 0) {
                if (modulo == 1 || modulo == lchain) {
                    err=5;
                    chaine_error="%";
                } else {
                    ctemp = chaine(0,modulo-1);
                    Analyze(ctemp.Data(),err,offset); if (err) return;
                    UInt_t leftopr = fNoper-1;
                    ctemp = chaine(modulo,lchain-modulo);
                    Analyze(ctemp.Data(),err,offset); if (err) return;
                    fExpr[fNoper] = "%";
                    actionCode = kModulo;
                    SetAction(fNoper,actionCode,actionParam);
                    fNoper++;
                    if (!CheckOperands(leftopr,fNoper-1,err)) return;
                }
            } else if (rshift != 0) {
                if (rshift == 1 || rshift == lchain) {
                    err=5;
                    chaine_error=">>";
                } else {
                    ctemp = chaine(0,rshift-1);
                    Analyze(ctemp.Data(),err,offset); if (err) return;
                    UInt_t leftopr = fNoper-1;
                    ctemp = chaine(rshift+1,lchain-rshift-1);
                    Analyze(ctemp.Data(),err,offset); if (err) return;
                    fExpr[fNoper] = ">>";
                    actionCode = kRightShift;
                    SetAction(fNoper,actionCode,actionParam);
                    fNoper++;
                    if (!CheckOperands(leftopr,fNoper-1,err)) return;
                }
            } else if (lshift != 0) {
                if (lshift == 1 || lshift == lchain) {
                    err=5;
                    chaine_error=">>";
                } else {
                    ctemp = chaine(0,lshift-1);
                    Analyze(ctemp.Data(),err,offset); if (err) return;
                    UInt_t leftopr = fNoper-1;
                    ctemp = chaine(lshift+1,lchain-lshift-1);
                    Analyze(ctemp.Data(),err,offset); if (err) return;
                    fExpr[fNoper] = ">>";
                    actionCode = kLeftShift;
                    SetAction(fNoper,actionCode,actionParam);
                    fNoper++;
                    if (!CheckOperands(leftopr,fNoper-1,err)) return;
                }
            } else {
                if (multi != 0) {
                    if (multi == 1 || multi == lchain) {
                        err=5;
                        chaine_error="*";
                    }
                    else {
                        ctemp = chaine(0,multi-1);
                        Analyze(ctemp.Data(),err,offset); if (err) return;
                        UInt_t leftopr = fNoper-1;
                        ctemp = chaine(multi,lchain-multi);
                        Analyze(ctemp.Data(),err,offset); if (err) return;
                        fExpr[fNoper] = "*";
                        actionCode = kMultiply;
                        SetAction(fNoper,actionCode,actionParam);
                        fNoper++;
                        if (!CheckOperands(leftopr,fNoper-1,err)) return;
                    }
                } else {
                    if (divi != 0) {
                        if (divi == 1 || divi == lchain) {
                            err=5;
                            chaine_error = "/";
                        }
                        else {
                            ctemp = chaine(0,divi-1);
                            Analyze(ctemp.Data(),err,offset); if (err) return;
                            UInt_t leftopr = fNoper-1;
                            ctemp = chaine(divi,lchain-divi);
                            Analyze(ctemp.Data(),err,offset); if (err) return;
                            fExpr[fNoper] = "/";
                            actionCode = kDivide;
                            SetAction(fNoper,actionCode,actionParam);
                            fNoper++;
                            if (!CheckOperands(leftopr,fNoper-1,err)) return;
                        }
                    } else {
                        if (puiss != 0) {
                            if (puiss == 1 || puiss == lchain) {
                                err = 5;
                                chaine_error = "**";
                            }
                            else {
                                if (chaine(lchain-2,2) == "^2") {
                                    ctemp = "sq(" + chaine(0,lchain-2) + ")";
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                } else {
                                    ctemp = chaine(0,puiss-1);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    UInt_t leftopr = fNoper-1;
                                    ctemp = chaine(puiss,lchain-puiss);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "^";
                                    actionCode = kpow;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(leftopr,fNoper-1,err)) return;
                                }
                            }
                        } else {

                            find=0;

                            //*-*- Check for a numerical expression
                            {
                                Bool_t hasDot = kFALSE;
                                Bool_t isHexa = kFALSE;
                                Bool_t hasExpo= kFALSE;
                                if ((chaine(0,2)=="0x")||(chaine(0,2)=="0X")) isHexa=kTRUE;
                                for (j=0; j<chaine.Length() && err==0; j++) {
                                    t=chaine[j];
                                    if (!isHexa) {
                                        if (j>0 && (chaine(j,1)=="e" || chaine(j,2)=="e+" || chaine(j,2)=="e-" || chaine(j,1)=="E" || chaine(j,2)=="E+" || chaine(j,2)=="E-")) {
                                            if (hasExpo) {
                                                err=26;
                                                chaine_error=chaine;
                                            }
                                            hasExpo = kTRUE;
                                            // The previous implementation allowed a '.' in the exponent.
                                            // That information was ignored (by sscanf), we now make it an error
                                            // hasDot = kFALSE;
                                            hasDot = kTRUE;  // forbid any additional '.'
                                            if (chaine(j,2)=="e+" || chaine(j,2)=="e-" || chaine(j,2)=="E+" || chaine(j,2)=="E-") j++;
                                        }
                                        else {
                                            if (chaine(j,1) == "." && !hasDot) hasDot = kTRUE; // accept only one '.' in the number
                                            else {
                                                // The previous implementation was allowing ANYTHING after the '.' and thus
                                                // made legal code like '2.3 and fpx' and was just silently ignoring the
                                                // 'and fpx'.
                                                if (!strchr("0123456789",t) && (chaine(j,1)!="+" || j!=0)) {
                                                    err = 30;
                                                    chaine_error=chaine;
                                                }
                                            }
                                        }
                                    }
                                    else {
                                        if (!strchr("0123456789abcdefABCDEF",t) && (j>1)) {
                                            err = 30;
                                            chaine_error=chaine;
                                        }
                                    }
                                }
                                if (fNconst >= gMAXCONST) err = 27;
                                if (!err) {
                                    if (!isHexa) {if (sscanf((const char*)chaine,"%lg",&vafConst) > 0) err = 0; else err =1;}
                                    else {if (sscanf((const char*)chaine,"%lx",&vafConst2) > 0) err = 0; else err=1;
                                        vafConst = (Double_t) vafConst2;}
                                    fExpr[fNoper] = chaine;
                                    k = -1;
                                    for (j=0;j<fNconst;j++) {
                                        if (vafConst == fConst[j] ) k= j;
                                    }
                                    if ( k < 0) {  k = fNconst; fNconst++; fConst[k] = vafConst; }
                                    actionCode = kConstant;
                                    actionParam = k;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                }
                                if (err==30) err=0;
                                else find = kTRUE;
                            }


                            //*-*- Look for an already defined expression
                            if (find==0) {
                                oldformula = (TFormula*)gROOT->GetListOfFunctions()->FindObject((const char*)chaine);
                                if (oldformula && strcmp(schain,oldformula->GetTitle())) {
                                    Int_t nprior = fNpar;
                                    Analyze(oldformula->GetExpFormula(),err,fNpar);

                                    if (err) return; // changes fNpar
                                    fNpar = nprior;
                                    find=1;
                                    if (!err) {
                                        Int_t npold = oldformula->GetNpar();
                                        fNpar += npold;
                                        for (Int_t ipar=0;ipar<npold;ipar++) {
                                            fParams[ipar+fNpar-npold] = oldformula->GetParameter(ipar);
                                        }
                                    }
                                }
                            }
                            if (find == 0) {
                                //*-*- Check if chaine is a defined variable.
                                //*-*- Note that DefinedVariable can be overloaded
                                ctemp = chaine;
                                ctemp.ReplaceAll(escapedSlash, slash);
                                Int_t action;
                                k = DefinedVariable(ctemp,action);
                                if (k==-3) {
                                    // Error message already issued
                                    err = 1;
                                } if (k==-2) {
                                    err = 31;
                                    chaine_error = ctemp;
                                } else if ( k >= 0 ) {
                                    fExpr[fNoper] = ctemp;
                                    actionCode = action;
                                    actionParam = k;
                                    SetAction(fNoper,actionCode,actionParam);
                                    if (action==kDefinedString) fNstring++;
                                    else if (k <kMAXFOUND && !fAlreadyFound.TestBitNumber(k)) {
                                        fAlreadyFound.SetBitNumber(k);
                                        fNval++;
                                    }
                                    fNoper++;
                                } else if (chaine(0,1) == "!") {
                                    ctemp = chaine(1,lchain-1);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "!";
                                    actionCode = kNot;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,1)=="\"" && chaine(chaine.Length()-1,1)=="\"") {
                                    //*-* It is a string !!!
                                    fExpr[fNoper] = chaine(1,chaine.Length()-2);
                                    actionCode = kStringConst;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                } else if (chaine(0,4) == "cos(") {
                                    ctemp = chaine(3,lchain-3);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "cos";
                                    actionCode = kcos;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,4) == "sin(") {
                                    ctemp = chaine(3,lchain-3);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "sin";
                                    actionCode = ksin;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,4) == "tan(") {
                                    ctemp = chaine(3,lchain-3);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "tan";
                                    actionCode = ktan;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,5) == "acos(") {
                                    ctemp = chaine(4,lchain-4);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "acos";
                                    actionCode = kacos;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,5) == "asin(") {
                                    ctemp = chaine(4,lchain-4);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "asin";
                                    actionCode = kasin;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,5) == "atan(") {
                                    ctemp = chaine(4,lchain-4);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "atan";
                                    actionCode = katan;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,5) == "cosh(") {
                                    ctemp = chaine(4,lchain-4);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "cosh";
                                    actionCode = kcosh;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,5) == "sinh(") {
                                    ctemp = chaine(4,lchain-4);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "sinh";
                                    actionCode = ksinh;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,5) == "tanh(") {
                                    ctemp = chaine(4,lchain-4);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "tanh";
                                    actionCode = ktanh;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,6) == "acosh(") {
                                    ctemp = chaine(5,lchain-5);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "acosh";
                                    actionCode = kacosh;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,6) == "asinh(") {
                                    ctemp = chaine(5,lchain-5);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "asinh";
                                    actionCode = kasinh;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,6) == "atanh(") {
                                    ctemp = chaine(5,lchain-5);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "atanh";
                                    actionCode = katanh;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,3) == "sq(") {
                                    ctemp = chaine(2,lchain-2);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "sq";
                                    actionCode = ksq;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,4) == "log(") {
                                    ctemp = chaine(3,lchain-3);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "log";
                                    actionCode = klog;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,6) == "log10(") {
                                    ctemp = chaine(5,lchain-5);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "log10";
                                    actionCode = klog10;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,4) == "exp(") {
                                    ctemp = chaine(3,lchain-3);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "exp";
                                    actionCode = kexp;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,4) == "abs(") {
                                    ctemp = chaine(3,lchain-3);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "abs";
                                    actionCode = kabs;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,5) == "sign(") {
                                    ctemp = chaine(4,lchain-4);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "sign";
                                    actionCode = ksign;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine(0,4) == "int(") {
                                    ctemp = chaine(3,lchain-3);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "int";
                                    actionCode = kint;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;
                                } else if (chaine == "rndm" || chaine(0,5) == "rndm(") {
                                    fExpr[fNoper] = "rndm";
                                    actionCode = krndm;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                } else if (chaine(0,5) == "sqrt(") {
                                    ctemp = chaine(4,lchain-4);
                                    Analyze(ctemp.Data(),err,offset); if (err) return;
                                    fExpr[fNoper] = "sqrt";
                                    actionCode = ksqrt;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;;
                                    if (!CheckOperands(fNoper-1,err)) return;

                                    //*-*- Look for an exponential
                                    //*-*  =======================
                                } else if ( chaine == "expo" || chaine(0,5)=="expo("
                                            || (lchain==5 && chaine(1,4)=="expo")
                                            || (lchain==6 && chaine(2,4)=="expo")
                                            || chaine(1,5)=="expo(" || chaine(2,5)=="expo(" ) {
                                    chaine1ST=chaine;
                                    if (chaine(1,4) == "expo") {
                                        ctemp=chaine(0,1);
                                        if (ctemp=="x") {
                                            inter2=0;
                                            if (fNdim < 1) fNdim = 1; }
                                        else if (ctemp=="y") {
                                            inter2=1;
                                            if (fNdim < 2) fNdim = 2; }
                                        else if (ctemp=="z") {
                                            inter2=2;
                                            if (fNdim < 3) fNdim = 3; }
                                        else if (ctemp=="t") {
                                            inter2=3;
                                            if (fNdim < 4) fNdim = 4; }
                                        else {
                                            err=26; // unknown name;
                                            chaine_error=chaine1ST;
                                        }
                                        chaine=chaine(1,lchain-1);
                                        lchain=chaine.Length();
                                    } else inter2=0;
                                    if (chaine(2,4) == "expo") {
                                        if (chaine(0,2) != "xy") {
                                            err=26; // unknown name
                                            chaine_error=chaine1ST;
                                        }
                                        else {
                                            inter2=5;
                                            if (fNdim < 2) fNdim = 2;
                                            chaine=chaine(2,lchain-2);
                                            lchain=chaine.Length();
                                        }
                                    }
                                    if (lchain == 4) {
                                        if (fNpar>=gMAXPAR) err=7; // too many parameters
                                        if (!err) {
                                            fExpr[fNoper] = chaine1ST;
                                            actionCode = kexpo + inter2;
                                            actionParam = offset;
                                            SetAction(fNoper,actionCode,actionParam);
                                            if (inter2 == 5+offset && fNpar < 3+offset) fNpar = 3+offset;
                                            if (fNpar < 2+offset) fNpar = 2+offset;
                                            if (fNpar>=gMAXPAR) err=7; // too many parameters
                                            if (!err) {
                                                fNoper++;
                                                if (fNdim < 1) fNdim = 1;
                                                if (fNpar == 2) SetNumber(200);
                                            }
                                        }
                                    } else if (chaine(4,1) == "(") {
                                        ctemp = chaine(5,lchain-6);
                                        fExpr[fNoper] = chaine1ST;
                                        for (j=0; j<ctemp.Length(); j++) {
                                            t=ctemp[j];
                                            if (strchr("0123456789",t)==0 && (ctemp(j,1)!="+" || j!=0)) {
                                                err=20;
                                                chaine_error=chaine1ST;
                                            }
                                        }
                                        if (err==0) {
                                            sscanf(ctemp.Data(),"%d",&inter);
                                            if (inter>=0) {
                                                inter += offset;
                                                actionCode = kexpo + inter2;
                                                actionParam = inter;
                                                SetAction(fNoper,actionCode,actionParam);
                                                if (inter2 == 5) inter++;
                                                if (inter+2>fNpar) fNpar = inter+2;
                                                if (fNpar>=gMAXPAR) err=7; // too many parameters
                                                if (!err) fNoper++;
                                                if (fNpar == 2) SetNumber(200);
                                            } else err=20;
                                        } else err = 20; // non integer value for parameter number
                                    } else {
                                        err=26; // unknown name
                                        chaine_error=chaine;
                                    }
                                    //*-*- Look for gaus, xgaus,ygaus,xygaus
                                    //*-*  =================================
                                } else if (chaine=="gaus"
                                           || (lchain==5 && chaine(1,4)=="gaus")
                                           || (lchain==6 && chaine(2,4)=="gaus")
                                           || chaine(0,5)=="gaus(" || chaine(1,5)=="gaus(" || chaine(2,5)=="gaus(") {
                                    chaine1ST=chaine;
                                    if (chaine(1,4) == "gaus") {
                                        ctemp=chaine(0,1);
                                        if (ctemp=="x") {
                                            inter2=0;
                                            if (fNdim < 1) fNdim = 1; }
                                        else if (ctemp=="y") {
                                            inter2=1;
                                            if (fNdim < 2) fNdim = 2; }
                                        else if (ctemp=="z") {
                                            inter2=2;
                                            if (fNdim < 3) fNdim = 3; }
                                        else if (ctemp=="t") {
                                            inter2=3;
                                            if (fNdim < 4) fNdim = 4; }
                                        else {
                                            err=26; // unknown name
                                            chaine_error=chaine1ST;
                                        }
                                        chaine=chaine(1,lchain-1);
                                        lchain=chaine.Length();
                                    } else inter2=0;
                                    if (chaine(2,4) == "gaus") {
                                        if (chaine(0,2) != "xy") {
                                            err=26; // unknown name
                                            chaine_error=chaine1ST;
                                        }
                                        else {
                                            inter2=5;
                                            if (fNdim < 2) fNdim = 2;
                                            chaine=chaine(2,lchain-2);
                                            lchain=chaine.Length();
                                            SetNumber(110); // xygaus
                                        }
                                    }
                                    if (lchain == 4 && err==0) {
                                        if (fNpar>=gMAXPAR) err=7; // too many parameters
                                        if (!err) {
                                            fExpr[fNoper] = chaine1ST;
                                            actionCode = kgaus + inter2;
                                            actionParam = offset;
                                            SetAction(fNoper,actionCode,actionParam);
                                            if (inter2 == 5+offset && fNpar < 5+offset) fNpar = 5+offset;
                                            if (3+offset>fNpar) fNpar = 3+offset;
                                            if (fNpar>=gMAXPAR) err=7; // too many parameters
                                            if (!err) {
                                                fNoper++;
                                                if (fNdim < 1) fNdim = 1;
                                                if (fNpar == 3) SetNumber(100);
                                            }
                                        }
                                    } else if (chaine(4,1) == "(" && err==0) {
                                        ctemp = chaine(5,lchain-6);
                                        fExpr[fNoper] = chaine1ST;
                                        for (j=0; j<ctemp.Length(); j++) {
                                            t=ctemp[j];
                                            if (strchr("0123456789",t)==0 && (ctemp(j,1)!="+" || j!=0)) {
                                                err=20;
                                                chaine_error=chaine1ST;
                                            }
                                        }
                                        if (err==0) {
                                            sscanf(ctemp.Data(),"%d",&inter);
                                            if (inter >= 0) {
                                                inter += offset;
                                                actionCode = kgaus + inter2;
                                                actionParam = inter;
                                                SetAction(fNoper,actionCode,actionParam);
                                                if (inter2 == 5) inter += 2;
                                                if (inter+3>fNpar) fNpar = inter+3;
                                                if (fNpar>=gMAXPAR) err=7; // too many parameters
                                                if (!err) fNoper++;
                                                if(fNpar == 3) SetNumber(100);
                                            } else err = 20; // non integer value for parameter number
                                        }
                                    } else if (err==0) {
                                        err=26; // unknown name
                                        chaine_error=chaine1ST;
                                    }
                                    //*-*- Look for landau, xlandau,ylandau,xylandau
                                    //*-*  =================================
                                } else if (chaine=="landau" || (lchain==7 && chaine(1,6)=="landau")
                                           || (lchain==8 && chaine(2,6)=="landau")
                                           || chaine(0,7)=="landau(" || chaine(1,7)=="landau(" || chaine(2,7)=="landau(") {
                                    chaine1ST=chaine;
                                    if (chaine(1,6) == "landau") {
                                        ctemp=chaine(0,1);
                                        if (ctemp=="x") {
                                            inter2=0;
                                            if (fNdim < 1) fNdim = 1; }
                                        else if (ctemp=="y") {
                                            inter2=1;
                                            if (fNdim < 2) fNdim = 2; }
                                        else if (ctemp=="z") {
                                            inter2=2;
                                            if (fNdim < 3) fNdim = 3; }
                                        else if (ctemp=="t") {
                                            inter2=3;
                                            if (fNdim < 4) fNdim = 4; }
                                        else {
                                            err=26; // unknown name
                                            chaine_error=chaine1ST;
                                        }
                                        chaine=chaine(1,lchain-1);
                                        lchain=chaine.Length();
                                    } else inter2=0;
                                    if (chaine(2,6) == "landau") {
                                        if (chaine(0,2) != "xy") {
                                            err=26; // unknown name
                                            chaine_error=chaine1ST;
                                        }
                                        else {
                                            inter2=5;
                                            if (fNdim < 2) fNdim = 2;
                                            chaine=chaine(2,lchain-2);
                                            lchain=chaine.Length();
                                            SetNumber(410);
                                        }
                                    }
                                    if (lchain == 6 && err==0) {
                                        if (fNpar>=gMAXPAR) err=7; // too many parameters
                                        if (!err) {
                                            fExpr[fNoper] = chaine1ST;
                                            actionCode = klandau + inter2;
                                            actionParam = offset;
                                            SetAction(fNoper,actionCode,actionParam);
                                            if (inter2 == 5+offset && fNpar < 5+offset) fNpar = 5+offset;
                                            if (3+offset>fNpar) fNpar = 3+offset;
                                            if (fNpar>=gMAXPAR) err=7; // too many parameters
                                            if (!err) {
                                                fNoper++;
                                                if (fNdim < 1) fNdim = 1;
                                                if (fNpar == 3) SetNumber(400);
                                            }
                                        }
                                    } else if (chaine(6,1) == "(" && err==0) {
                                        ctemp = chaine(7,lchain-8);
                                        fExpr[fNoper] = chaine1ST;
                                        for (j=0; j<ctemp.Length(); j++) {
                                            t=ctemp[j];
                                            if (strchr("0123456789",t)==0 && (ctemp(j,1)!="+" || j!=0)) {
                                                err=20;
                                                chaine_error=chaine1ST;
                                            }
                                        }
                                        if (err==0) {
                                            sscanf(ctemp.Data(),"%d",&inter);
                                            if (inter >= 0) {
                                                inter += offset;
                                                actionCode = klandau + inter2;
                                                actionParam = inter;
                                                SetAction(fNoper,actionCode,actionParam);
                                                if (inter2 == 5) inter += 2;
                                                if (inter+3>fNpar) fNpar = inter+3;
                                                if (fNpar>=gMAXPAR) err=7; // too many parameters
                                                if (!err) fNoper++;
                                                if (fNpar == 3) SetNumber(400);
                                            } else err = 20; // non integer value for parameter number
                                        }
                                    } else if (err==0) {
                                        err=26; // unknown name
                                        chaine_error=chaine1ST;
                                    }
                                    //*-*- Look for a polynomial
                                    //*-*  =====================
                                } else if (chaine(0,3) == "pol" || chaine(1,3) == "pol") {
                                    chaine1ST=chaine;
                                    if (chaine(1,3) == "pol") {
                                        ctemp=chaine(0,1);
                                        if (ctemp=="x") {
                                            inter2=1;
                                            if (fNdim < 1) fNdim = 1; }
                                        else if (ctemp=="y") {
                                            inter2=2;
                                            if (fNdim < 2) fNdim = 2; }
                                        else if (ctemp=="z") {
                                            inter2=3;
                                            if (fNdim < 3) fNdim = 3; }
                                        else if (ctemp=="t") {
                                            inter2=4;
                                            if (fNdim < 4) fNdim = 4; }
                                        else {
                                            err=26; // unknown name;
                                            chaine_error=chaine1ST;
                                        }
                                        chaine=chaine(1,lchain-1);
                                        lchain=chaine.Length();
                                    } else inter2=1;
                                    if (chaine(lchain-1,1) == ")") {
                                        nomb = 0;
                                        for (j=3;j<lchain;j++) if (chaine(j,1)=="(" && nomb == 0) nomb = j;
                                        if (nomb == 3) err = 23; // degree of polynomial not specified
                                        if (nomb == 0) err = 40; // '(' is expected
                                        ctemp = chaine(nomb+1,lchain-nomb-2);
                                        for (j=0; j<ctemp.Length(); j++) {
                                            t=ctemp[j];
                                            if (strchr("0123456789",t)==0 && (ctemp(j,1)!="+" || j!=0)) {
                                                err=20;
                                                chaine_error=chaine1ST;
                                            }
                                        }
                                        if (!err) {
                                            sscanf(ctemp.Data(),"%d",&inter);
                                            if (inter < 0) err = 20;
                                        }
                                    }
                                    else {
                                        nomb = lchain;
                                        inter = 0;
                                    }
                                    if (!err) {
                                        inter--;
                                        ctemp = chaine(3,nomb-3);
                                        if (sscanf(ctemp.Data(),"%d",&n) > 0) {
                                            if (n < 0  ) err = 24; //Degree of polynomial must be positive
                                            if (n >= 20) err = 25; //Degree of polynomial must be less than 20
                                        } else err = 20;
                                    }
                                    if (!err) {
                                        fExpr[fNoper] = chaine1ST;
                                        actionCode = kpol+(inter2-1);
                                        actionParam = n*100+inter+2;
                                        SetAction(fNoper,actionCode,actionParam);
                                        if (inter+n+1>=fNpar) fNpar = inter + n + 2;
                                        if (fNpar>=gMAXPAR) err=7; // too many parameters
                                        if (!err) {
                                            fNoper++;
                                            if (fNdim < 1) fNdim = 1;
                                            SetNumber(300+n);
                                        }
                                    }
                                    //*-*- Look for pow,atan2,etc
                                    //*-*  ======================
                                } else if (chaine(0,4) == "pow(") {
                                    compt = 4; nomb = 0; virgule = 0; nest=0;
                                    while(compt != lchain) {
                                        compt++;
                                        if (chaine(compt-1,1) == "(") nest++;
                                        else if (chaine(compt-1,1) == ")") nest--;
                                        else if (chaine(compt-1,1) == "," && nest==0) {
                                            nomb++;
                                            if (nomb == 1 && virgule == 0) virgule = compt;
                                        }
                                    }
                                    if (nomb != 1) err = 22; // There are plus or minus than 2 arguments for pow
                                    else {
                                        ctemp = chaine(4,virgule-5);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        UInt_t leftopr = fNoper-1;
                                        ctemp = chaine(virgule,lchain-virgule-1);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        fExpr[fNoper] = "^";
                                        actionCode = kpow;
                                        SetAction(fNoper,actionCode,actionParam);
                                        fNoper++;
                                        if (!CheckOperands(leftopr,fNoper-1,err)) return;
                                    }
                                } else if (chaine(0,7) == "strstr(") {
                                    compt = 7; nomb = 0; virgule = 0; nest=0;
                                    inString = false;
                                    while(compt != lchain) {
                                        compt++;
                                        if (chaine(compt-1,1) == "\"") {
                                            inString = !inString;
                                        }  else if (!inString) {
                                            if (chaine(compt-1,1) == "(") nest++;
                                            else if (chaine(compt-1,1) == ")") nest--;
                                            else if (chaine(compt-1,1) == "," && nest==0) {
                                                nomb++;
                                                if (nomb == 1 && virgule == 0) virgule = compt;
                                            }
                                        }
                                    }
                                    if (nomb != 1) err = 28; // There are plus or minus than 2 arguments for strstr
                                    else {
                                        ctemp = chaine(7,virgule-8);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        Int_t optloc = fNoper-1;

                                        ctemp = chaine(virgule,lchain-virgule-1);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        fExpr[fNoper] = "strstr";
                                        actionCode = kstrstr;
                                        SetAction(fNoper,actionCode,actionParam);
                                        fNoper++;

                                        if ( !IsString(optloc) || !IsString(fNoper-2) ) {
                                            err = 46;
                                            chaine_error = "strstr";
                                        }
                                    }
                                } else if (chaine(0,4) == "min(") {
                                    compt = 4; nomb = 0; virgule = 0; nest=0;
                                    while(compt != lchain) {
                                        compt++;
                                        if (chaine(compt-1,1) == "(") nest++;
                                        else if (chaine(compt-1,1) == ")") nest--;
                                        else if (chaine(compt-1,1) == "," && nest==0) {
                                            nomb++;
                                            if (nomb == 1 && virgule == 0) virgule = compt;
                                        }
                                    }
                                    if (nomb != 1) {
                                        err = 44; // There are plus or minus than 2 arguments for min
                                        err_hint = 3;
                                    }
                                    else {
                                        ctemp = chaine(4,virgule-5);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        UInt_t leftopr = fNoper-1;
                                        ctemp = chaine(virgule,lchain-virgule-1);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        fExpr[fNoper] = "min";
                                        actionCode = kmin;
                                        SetAction(fNoper,actionCode,actionParam);
                                        fNoper++;
                                        if (!CheckOperands(leftopr,fNoper-1,err)) return;
                                    }
                                } else if (chaine(0,4) == "max(") {
                                    compt = 4; nomb = 0; virgule = 0; nest=0;
                                    while(compt != lchain) {
                                        compt++;
                                        if (chaine(compt-1,1) == "(") nest++;
                                        else if (chaine(compt-1,1) == ")") nest--;
                                        else if (chaine(compt-1,1) == "," && nest==0) {
                                            nomb++;
                                            if (nomb == 1 && virgule == 0) virgule = compt;
                                        }
                                    }
                                    if (nomb != 1) {
                                        err = 44; // There are plus or minus than 2 arguments for min
                                        err_hint = 3;
                                    }
                                    else {
                                        ctemp = chaine(4,virgule-5);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        UInt_t leftopr = fNoper-1;
                                        ctemp = chaine(virgule,lchain-virgule-1);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        fExpr[fNoper] = "max";
                                        actionCode = kmax;
                                        SetAction(fNoper,actionCode,actionParam);
                                        fNoper++;
                                        if (!CheckOperands(leftopr,fNoper-1,err)) return;
                                    }

                                } else if (chaine(0,6) == "atan2(") {
                                    compt = 6; nomb = 0; virgule = 0; nest=0;
                                    while(compt != lchain) {
                                        compt++;
                                        if (chaine(compt-1,1) == "(") nest++;
                                        else if (chaine(compt-1,1) == ")") nest--;
                                        else if (chaine(compt-1,1) == "," && nest==0) {
                                            nomb++;
                                            if (nomb == 1 && virgule == 0) virgule = compt;
                                        }
                                    }
                                    if (nomb != 1) err = 21;  //{ There are plus or minus than 2 arguments for atan2
                                    else {
                                        ctemp = chaine(6,virgule-7);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        UInt_t leftopr = fNoper-1;
                                        ctemp = chaine(virgule,lchain-virgule-1);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        fExpr[fNoper] = "atan2";
                                        actionCode = katan2;
                                        SetAction(fNoper,actionCode,actionParam);
                                        fNoper++;
                                        if (!CheckOperands(leftopr,fNoper-1,err)) return;
                                    }
                                } else if (chaine(0,5) == "fmod(") {
                                    compt = 5; nomb = 0; virgule = 0; nest=0;
                                    while(compt != lchain) {
                                        compt++;
                                        if (chaine(compt-1,1) == "(") nest++;
                                        else if (chaine(compt-1,1) == ")") nest--;
                                        else if (chaine(compt-1,1) == "," && nest==0) {
                                            nomb++;
                                            if (nomb == 1 && virgule == 0) virgule = compt;
                                        }
                                    }
                                    if (nomb != 1) {
                                        err = 44; // There are plus or minus than 2 arguments for fmod
                                        err_hint = 4;
                                    }
                                    else {
                                        ctemp = chaine(5,virgule-6);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        UInt_t leftopr = fNoper-1;
                                        ctemp = chaine(virgule,lchain-virgule-1);
                                        Analyze(ctemp.Data(),err,offset); if (err) return;
                                        fExpr[fNoper] = "fmod";
                                        actionCode = kfmod;
                                        SetAction(fNoper,actionCode,actionParam);
                                        fNoper++;
                                        if (!CheckOperands(leftopr,fNoper-1,err)) return;
                                    }
                                } else if (AnalyzeFunction(chaine,err,offset) || err) { // The '||err' is to grab an error coming from AnalyzeFunction
                                    if (err) {
                                        chaine_error = chaine;
                                    } else {
                                        // We have a function call. Note that all the work was already,
                                        // eventually done in AnalyzeFunction
                                        //fprintf(stderr,"We found a foreign function in %s\n",chaine.Data());
                                    }
                                } else if (chaine(0,1) == "[" && chaine(lchain-1,1) == "]") {
                                    fExpr[fNoper] = chaine;
                                    fNoper++;
                                    ctemp = chaine(1,lchain-2);
                                    for (j=0; j<ctemp.Length(); j++) {
                                        t=ctemp[j];
                                        if (strchr("0123456789",t)==0 && (ctemp(j,1)!="+" || j!=0)) {
                                            err=20;
                                            chaine_error=chaine1ST; // le numero ? de par[?] n'est pas un entier }
                                        }
                                    }
                                    if (!err) {
                                        sscanf(ctemp.Data(),"%d",&valeur);
                                        actionCode = kParameter;
                                        actionParam = offset + valeur;
                                        SetAction(fNoper-1, actionCode, actionParam);
                                        fExpr[fNoper-1] = "[";
                                        fExpr[fNoper-1] = (fExpr[fNoper-1] + (long int)(valeur+offset)) + "]";
                                    }
                                } else if (chaine == "pi") {
                                    fExpr[fNoper] = "pi";
                                    actionCode = kpi;
                                    SetAction(fNoper,actionCode,actionParam);
                                    fNoper++;
                                }
                                else {
                                    //*-*- None of the above.
                                    //*-*  ==================
                                    err = 30;
                                }
                            }
                        }
                    }
                }
            }
        }

        //   Test  * si y existe :  que x existe
        //         * si z existe :  que x et y existent

        //     nomb = 1;
        //     for (i=1; i<=fNoper; i++) {
        //         if (fOper[i-1] == 97 && nomb > 0) nomb *= -1;
        //         if (fOper[i-1] == 98 && TMath::Abs(nomb) != 2) nomb *= 2;
        //         if (fOper[i-1] == 99 && TMath::Abs(nomb) != 20 && TMath::Abs(nomb) != 10) nomb *= 10;
        //     }
        //     if (nomb == 10)  err = 10; //{variable z sans x et y }
        //     if (nomb == 20)  err = 11; //{variables z et y sans x }
        //     if (nomb == 2)   err = 12; //{variable y sans x }
        //     if (nomb == -10) err = 13; //{variables z et x sans y }

        //*-*- Overflows
        if (fNoper>=gMAXOP) err=6; // too many operators

    }

    //*-*- errors!
    if (err>1) {
        fError = "";
        chaine_error = "\""+chaine_error+"\"";
        switch(err) {
        case  2 : fError = " Invalid Floating Point Operation"; break;
        case  4 : fError = " Empty String"; break;
        case  5 : fError = " Invalid Syntax " + chaine_error; break;
        case  6 : fError = " Too many operators !"; break;
        case  7 : fError = " Too many parameters !"; break;
        case 10 : fError = " z specified but not x and y"; break;
        case 11 : fError = " z and y specified but not x"; break;
        case 12 : fError = " y specified but not x"; break;
        case 13 : fError = " z and x specified but not y"; break;
        case 20 : fError = " Non integer value for parameter number : " + chaine_error; break;
        case 21 : fError = " ATAN2 requires two arguments"; break;
        case 22 : fError = " POW requires two arguments"; break;
        case 23 : fError = " Degree of polynomial not specified"; break;
        case 24 : fError = " Degree of polynomial must be positive"; break;
        case 25 : fError = " Degree of polynomial must be less than 20"; break;
        case 26 : fError = " Unknown name : " + chaine_error; break;
        case 27 : fError = " Too many constants in expression"; break;
        case 28 : fError = " strstr requires two arguments"; break;
        case 29 : fError = " TFormula can only call interpreted and compiled functions that return a numerical type: " + chaine_error; break;
        case 30 : fError = " Bad numerical expression : " + chaine_error; break;
        case 31 : fError = " Part of the Variable " + chaine_error; fError += " exists but some of it is not accessible or useable"; break;
        case 40 : fError = " '(' is expected"; break;
        case 41 : fError = " ')' is expected"; break;
        case 42 : fError = " '[' is expected"; break;
        case 43 : fError = " ']' is expected"; break;
        case 44 : fError = " The function '" + chaine(0,err_hint) + "' requires two arguments."; break;
        case 45 : fError = "The operator " + chaine_error + " requires a numerical operand."; break;
        case 46 : fError = "Both operands of the operator " + chaine_error + " have to be either numbers or strings."; break;
        case 47 : fError = chaine_error + " requires 2 string arguments"; break;
        }
        //        Error("Compile", "%s", er.Data());
        //err=1;
       // qDebug()<<"IROOT Error:"<<fError<<err;
    }

}

QString IROOTTreeFormulaChecker::GetErrorString()
{
 return fError;
}
