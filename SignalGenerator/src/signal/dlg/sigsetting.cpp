#include "sigsetting.h"
#include "ui_sigsetting.h"
#include "../def/sigconst.h"
#include "../def/sigsin.h"

SigSetting::SigSetting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SigSetting)
{
    ui->setupUi(this);
    SwitchPage(0);
    SetDataDefault();
}

SigSetting::~SigSetting()
{
    delete ui;
}

SigDef* SigSetting::OnSettingSig(SigDef* sig)
{
    this->show();
    SetDataDefault();
    if(sig != nullptr)
    {
        //重新配置信号
        SwitchPage(sig->GetSigType());
        switch(sig->GetSigType())
        {
            case SigType::Const:
                {
                    SigConst* _tmp_sig = dynamic_cast<SigConst*>(sig);
                    ui->m_doubleSpinBox_dc_amp->setValue(_tmp_sig->GetAmp());
                    ui->m_doubleSpinBox_dc_data->setValue(_tmp_sig->GetDC());
                    break;
                }
            case SigType::Sin:
                {
                    SigSin* _tmp_sig = dynamic_cast<SigSin*>(sig);
                    ui->m_doubleSpinBox_sin_amp->setValue(_tmp_sig->GetAmp());
                    ui->m_doubleSpinBox_sin_freq->setValue(_tmp_sig->GetFreq());
                    ui->m_spinBox_sin_phase->setValue(_tmp_sig->GetPhase());
                    break;
                }
        }
    }
    else
    {
        SwitchPage(0);
    }

    SigDef* _tmp_sig = nullptr;

    this->exec();

    switch(ui->m_comboBox_sigtype->currentIndex())
    {
        case SigType::Const:
            {
                SigConst* _tmp_sig_const = new SigConst();
                _tmp_sig_const->SetAmp(ui->m_doubleSpinBox_dc_amp->value());
                _tmp_sig_const->SetDC(ui->m_doubleSpinBox_dc_data->value());
                _tmp_sig = _tmp_sig_const;
                break;
            }
        case SigType::Sin:
            {
                SigSin* _tmp_sig_sin = new SigSin();
                _tmp_sig_sin->SetAmp(ui->m_doubleSpinBox_sin_amp->value());
                _tmp_sig_sin->SetFreq(ui->m_doubleSpinBox_sin_freq->value());
                _tmp_sig_sin->SetPhase(ui->m_spinBox_sin_phase->value());
                _tmp_sig = _tmp_sig_sin;
                break;
            }
    }
    assert(_tmp_sig != nullptr);
    return _tmp_sig;
}

void SigSetting::on_m_comboBox_sigtype_currentIndexChanged(int index)
{
    ui->m_stackedWidget_sig_setting->setCurrentIndex(index);
}

void SigSetting::SwitchPage(int index)
{
    assert(index >= 0 && index < ui->m_comboBox_sigtype->count());

    ui->m_stackedWidget_sig_setting->setCurrentIndex(index);
    ui->m_comboBox_sigtype->setCurrentIndex(index);
}

void SigSetting::SetDataDefault()
{
    {
        //直流
        ui->m_doubleSpinBox_dc_data->setValue(1);
        ui->m_doubleSpinBox_dc_amp->setValue(1);
    }
    {
        //sin
        ui->m_doubleSpinBox_sin_amp->setValue(1);
        ui->m_doubleSpinBox_sin_freq->setValue(1);
        ui->m_spinBox_sin_phase->setValue(0);
    }

}

