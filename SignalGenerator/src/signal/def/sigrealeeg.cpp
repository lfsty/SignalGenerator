
#include "sigrealeeg.h"

SigRealEEG::SigRealEEG(QObject* parent)
    : SigDef{parent}
{

}

qreal SigRealEEG::GenData(const quint64& t_ms)
{
    return m_data.at(t_ms % m_data.size());
}

QString SigRealEEG::GetDescription()
{
    return "Real EEG";
}

const SigType SigRealEEG::GetSigType()
{
    return SigType::Real;
}

SigDef* SigRealEEG::GetCopy()
{
    return nullptr;
}

void SigRealEEG::SetEEGData(const QList<float>& eeg_data)
{
    m_data = eeg_data;
}

