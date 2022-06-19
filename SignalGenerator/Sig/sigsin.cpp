#include "sigsin.h"
SimSig::SigSin::SigSin()
{
    SetType(SigType::Sin);
}

SimSig::SigSin::~SigSin()
{

}

int SimSig::SigSin::GetFreq()
{
    return m_freq;
}

void SimSig::SigSin::SetFreq(int freq)
{
    m_freq = freq;
}

int SimSig::SigSin::GetPhaseMs()
{
    return m_phase_ms;
}

void SimSig::SigSin::SetPhaseMs(int phase_ms)
{
    m_phase_ms = phase_ms;
}

float SimSig::SigSin::GenData(qint64 tm_ms)
{
    float x = static_cast<float>((tm_ms + GetPhaseMs())) / 1000;
    return (qSin(2 * M_PI * GetFreq() * x)) * GetAmp();
}

void SimSig::SigSin::SetDescription()
{
    m_description_str = QString::asprintf("频率:%d,相位:%d,幅度:%d", GetFreq(), GetFreq(), GetAmp());
}

QString SimSig::SigSin::GetTypeStr()
{
    return "Sin";
}

SimSig::SigDef *SimSig::SigSin::GetCopy()
{
    SigSin *new_sig = new SigSin();
    new_sig->SetAmp(GetAmp());
    new_sig->SetFreq(GetFreq());
    new_sig->SetPhaseMs(GetPhaseMs());
    new_sig->SetDescription();
    return new_sig;
}

QJsonObject SimSig::SigSin::GenJsonData()
{
    QJsonObject data_obj = __super::GenJsonData();
    data_obj["Freq"] = GetFreq();
    data_obj["Phase"] = GetPhaseMs();
    return data_obj;
}

void SimSig::SigSin::ParseJsonData(QJsonObject data_obj)
{
    __super::ParseJsonData(data_obj);
    SetFreq(data_obj["Freq"].toInt());
    SetPhaseMs(data_obj["Phase"].toInt());
}
