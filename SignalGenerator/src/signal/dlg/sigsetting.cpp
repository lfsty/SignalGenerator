#include "sigsetting.h"
#include "ui_sigsetting.h"

SigSetting::SigSetting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SigSetting)
{
    ui->setupUi(this);
    ui->m_stackedWidget_sig_setting->setCurrentIndex(0);
    ui->m_comboBox_sigtype->setCurrentIndex(0);
}

SigSetting::~SigSetting()
{
    delete ui;
}

void SigSetting::OnSettingSig(SigDef* sig)
{
    this->show();
}

void SigSetting::on_m_comboBox_sigtype_currentIndexChanged(int index)
{
    ui->m_stackedWidget_sig_setting->setCurrentIndex(index);
}

