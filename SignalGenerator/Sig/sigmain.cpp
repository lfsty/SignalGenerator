#include "sigmain.h"

SimSig::SigMain::SigMain(MainFreqType main_type)
{
    m_main_freq_type = main_type;
    switch(main_type)
    {
        case Main50:
            SetType(SigType::Main50);
            break;
        case Main60:
            SetType(SigType::Main60);
            break;
    }
}

SimSig::SigMain::~SigMain()
{

}


float SimSig::SigMain::GenData(qint64 tm_ms)
{
    float x = static_cast<float>((tm_ms)) / 1000;
    switch(m_main_freq_type)
    {
        case Main50:
            return (qSin(2 * M_PI * 50 * x)) * GetAmp();
            break;
        case Main60:
            return (qSin(2 * M_PI * 60 * x)) * GetAmp();
            break;
    }
}

void SimSig::SigMain::SetDescription()
{
    m_description_str = QString::asprintf("工频干扰幅度:%d", GetAmp());
}

QString SimSig::SigMain::GetTypeStr()
{
    switch(m_main_freq_type)
    {
        case Main50:
            return "50Hz";
            break;
        case Main60:
            return "60Hz";
            break;
    }
}

SimSig::SigDef *SimSig::SigMain::GetCopy()
{
    SigMain *new_sig = new SigMain(m_main_freq_type);
    new_sig->SetAmp(GetAmp());
    new_sig->SetDescription();
    return new_sig;
}

