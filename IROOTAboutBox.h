#ifndef IROOTABOUTBOX_H
#define IROOTABOUTBOX_H

#include <QDialog>

namespace Ui {
class IROOTAboutBox;
}

class IROOTAboutBox : public QDialog
{
    Q_OBJECT
    
public:
    explicit IROOTAboutBox(QWidget *parent = 0);
    ~IROOTAboutBox();
    
private:
    Ui::IROOTAboutBox *ui;
};

#endif // IROOTABOUTBOX_H
