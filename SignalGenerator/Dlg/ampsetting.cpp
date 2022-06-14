#include "ampsetting.h"
#include "ui_ampsetting.h"

AmpSetting::AmpSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AmpSetting)
{
    ui->setupUi(this);
}

AmpSetting::~AmpSetting()
{
    delete ui;
}

QString AmpSetting::GetSrate()
{
    return ui->m_combobox_srate->currentText();
}

void AmpSetting::SetSrate(int srate)
{
    ui->m_combobox_srate->setCurrentText(QString().number(srate));
}
