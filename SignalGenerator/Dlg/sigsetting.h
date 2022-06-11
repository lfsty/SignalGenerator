#ifndef SIGSETTING_H
#define SIGSETTING_H
#include <QDialog>
#include "sigdef.h"
#include "sigdc.h"
#include "sigsin.h"
#include "sigcos.h"
#include "sigmain.h"
namespace Ui
{
    class SigSetting;
}

class SigSetting : public QDialog
{
    Q_OBJECT

public:
    explicit SigSetting(QWidget *parent = nullptr);
    ~SigSetting();
private:
    SimSig::SigDef *m_sig = nullptr;
public:
    SimSig::SigDef *GetSig();

private slots:
    void on_m_combobox_sig_type_currentTextChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void showEvent(QShowEvent *event);

private:
    Ui::SigSetting *ui;
};

#endif // SIGSETTING_H
