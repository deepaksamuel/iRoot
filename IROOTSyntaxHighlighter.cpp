#include "IROOTSyntaxHighlighter.h"

IROOTSyntaxHighlighter::IROOTSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b"
                    << "\\bTGraph\\b"
                    << "\\bTH1C\\b"<<"\\bTH1S\\b"<< "\\bTH1I\\b"<<"\\bTH1F\\b"<< "\\bTH1D\\b"
                    << "\\bTH2C\\b"<<"\\bTH2S\\b"<< "\\bTH2I\\b"<<"\\bTH2F\\b"<< "\\bTH2D\\b"
                    << "\\bTH3C\\b"<<"\\bTH3S\\b"<< "\\bTH3I\\b"<<"\\bTH3F\\b"<< "\\bTH3D\\b"
                    << "\\bTProfile\\b"<<"\\bTProfile2D\\b"<< "\\bTProfile3D\\b"
                    << "\\bBool_t\\b"<<"\\bDouble32_t\\b"<< "\\bDouble_t\\b"
                    << "\\bChar_t\\b"<<"\\bFloat16_t\\b"<< "\\bFloat_t\\b"<<"\\bUInt_t\\b"<<"\\bULong64_t\\b"
                    << "\\bInt_t\\b"<<"\\bLong64_t\\b"<< "\\bLong_t\\b"<<"\\bShort_t\\b"<<"\\bULong_t\\b"<<"\\bUShort_t\\b"
                    << "\\bOption_t\\b"<<"\\bTGraph\\b"<< "\\bTGraph2D\\b"
                    << "\\bTFile\\b"<<"\\bTTree\\b"<< "\\bTBranch\\b"
                    << "\\bTime_t\\b"<<"\\bTAxis\\b"<< "\\bTF1\\b"<<"\\bTH2\\b"<<"\\bTStyle\\b"
                     << "\\bTROOT\\b"<<"\\bTChain\\b"<< "\\bTSelector\\b";


    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
//! [0] //! [1]
    }
//! [1]

//! [2]
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);
//! [2]

//! [3]
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);
//! [3]

//! [4]
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
//! [4]

//! [5]
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [5]

//! [6]
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}
//! [6]

//! [7]
void IROOTSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
//! [7] //! [8]
    setCurrentBlockState(0);
//! [8]

//! [9]
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

//! [9] //! [10]
    while (startIndex >= 0) {
//! [10] //! [11]
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
//! [11]
