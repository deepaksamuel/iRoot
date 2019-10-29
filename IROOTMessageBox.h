#ifndef IROOTMESSAGEBOX_H
#define IROOTMESSAGEBOX_H

#include <QWidget>
#include <QTimer>


namespace Ui {
class IROOTMessageBox;
}

class IROOTMessageBox : public QWidget
{
    Q_OBJECT
    
public:
    explicit IROOTMessageBox(QWidget *parent = 0);
    void        ShowMessage(QString message, QString type, QWidget *w);
    //void        Append(QString message, QString type, int x, int y);
    void        ShowMessages(int milliseconds);
    ~IROOTMessageBox();
    
private slots:
    void hideMessage();


    void on_close_clicked();

private:
    Ui::IROOTMessageBox *ui;
    QTimer *hideTimer;
};

#endif // IROOTMESSAGEBOX_H
