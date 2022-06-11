#include "sigcos.h"

SimSig::SigCos::SigCos()
{
    SetType(SigType::Cos);
}

SimSig::SigCos::~SigCos()
{

}

int SimSig::SigCos::GetFreq()
{
    return m_freq;
}

void SimSig::SigCos::SetFreq(int freq)
{
    m_freq = freq;
}

int SimSig::SigCos::GetPhaseMs()
{
    return m_phase_ms;
}

void SimSig::SigCos::SetPhaseMs(int phase_ms)
{
    m_phase_ms = phase_ms;
}

float SimSig::SigCos::GenData(qint64 tm_ms)
{
    float x = static_cast<float>((tm_ms + GetPhaseMs())) / 1000;
    return (qCos(2 * M_PI * GetFreq() * x)) * GetAmp();
}

void SimSig::SigCos::SetDescription()
{
    m_description_str = QString::asprintf("频率:%d,相位:%d,幅度:%d", GetFreq(), GetFreq(), GetAmp());
}

QString SimSig::SigCos::GetTypeStr()
{
    return "Cos";
}

SimSig::SigDef *SimSig::SigCos::GetCopy()
{
    SigCos *new_sig = new SigCos();
    new_sig->SetAmp(GetAmp());
    new_sig->SetFreq(GetFreq());
    new_sig->SetPhaseMs(GetPhaseMs());
    new_sig->SetDescription();
    return new_sig;
}
