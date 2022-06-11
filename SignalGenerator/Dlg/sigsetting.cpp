#include "sigsetting.h"
#include "ui_sigsetting.h"

SigSetting::SigSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SigSetting)
{
    ui->setupUi(this);
    ui->m_lineedit_dc->setValidator(new QIntValidator(ui->m_lineedit_dc));
    ui->m_lineedit_sin_amp->setValidator(new QIntValidator(ui->m_lineedit_sin_amp));
    ui->m_lineedit_sin_freq->setValidator(new QIntValidator(ui->m_lineedit_sin_freq));
    ui->m_lineedit_sin_phase->setValidator(new QIntValidator(ui->m_lineedit_sin_phase));
}

SigSetting::~SigSetting()
{
    delete ui;
}

SimSig::SigDef *SigSetting::GetSig()
{
    return m_sig;
}

void SigSetting::on_m_combobox_sig_type_currentTextChanged(const QString &arg1)
{
    //页面切换
    ui->m_stack_page_sig->setCurrentIndex(ui->m_combobox_sig_type->currentIndex());
}


void SigSetting::on_buttonBox_accepted()
{
    switch(ui->m_combobox_sig_type->currentIndex())
    {
        case 0:
            {
                SimSig::SigSin *sig = new SimSig::SigSin();
                sig->SetAmp(ui->m_lineedit_sin_amp->text().toInt());
                sig->SetFreq(ui->m_lineedit_sin_freq->text().toInt());
                sig->SetPhaseMs(ui->m_lineedit_sin_phase->text().toInt());
                m_sig = sig;
                break;
            }
        case 1:
            {
                SimSig::SigCos *sig = new SimSig::SigCos();
                sig->SetAmp(ui->m_lineedit_cos_amp->text().toInt());
                sig->SetFreq(ui->m_lineedit_cos_freq->text().toInt());
                sig->SetPhaseMs(ui->m_lineedit_cos_phase->text().toInt());
                m_sig = sig;
                break;
            }
        case 2:
            {
                SimSig::SigDc *sig = new SimSig::SigDc();
                sig->SetDC(ui->m_lineedit_dc->text().toInt());
                m_sig = sig;
                break;
            }
        case 3:
            {
                qDebug() << "white noise";
                break;
            }
        case 4:
            {
                SimSig::SigMain *sig = new SimSig::SigMain(SimSig::SigMain::Main50);
                sig->SetAmp(ui->m_lineedit_main_50_amp->text().toInt());
                m_sig = sig;
                break;
            }
        case 5:
            {
                SimSig::SigMain *sig = new SimSig::SigMain(SimSig::SigMain::Main60);
                sig->SetAmp(ui->m_lineedit_main_60_amp->text().toInt());
                m_sig = sig;
                break;
                break;
            }
        default:
            break;
    }
    if(m_sig != nullptr)
    {
        m_sig->SetDescription();
    }
}

void SigSetting::showEvent(QShowEvent *event)
{
    ui->m_combobox_sig_type->setCurrentIndex(0);
    ui->m_stack_page_sig->setCurrentIndex(0);
}

