#ifndef SIGSETTING_H
#define SIGSETTING_H

#include <QDialog>
#include "../def/sigdef.h"

namespace Ui
{
    class SigSetting;
}

class SigSetting : public QDialog
{
    Q_OBJECT

public:
    explicit SigSetting(QWidget* parent = nullptr);
    ~SigSetting();
public:
    SigDef* OnSettingSig(SigDef* sig = nullptr);

private slots:
    void on_m_comboBox_sigtype_currentIndexChanged(int index);
private:
    void SwitchPage(int index);
    void SetDataDefault();
private:
    Ui::SigSetting* ui;
};

#endif // SIGSETTING_H
