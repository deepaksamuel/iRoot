#ifndef IROOTSYNTAXHIGHLIGHTER_H
#define IROOTSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class IROOTSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    IROOTSyntaxHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // IROOTSYNTAXHIGHLIGHTER_H
