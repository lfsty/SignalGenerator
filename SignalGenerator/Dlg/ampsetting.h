#ifndef AMPSETTING_H
#define AMPSETTING_H

#include <QDialog>

namespace Ui
{
    class AmpSetting;
}

class AmpSetting : public QDialog
{
    Q_OBJECT

public:
    explicit AmpSetting(QWidget *parent = nullptr);
    ~AmpSetting();
    QString GetSrate();

private:
    Ui::AmpSetting *ui;
};

#endif // AMPSETTING_H
