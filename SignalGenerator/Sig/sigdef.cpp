#include "sigdef.h"


SimSig::SigDef::SigDef()
{

}

SimSig::SigDef::~SigDef()
{

}

SimSig::SigType SimSig::SigDef::GetType()
{
    return m_type;
}

void SimSig::SigDef::SetType(SigType type)
{
    m_type = type;
}

int SimSig::SigDef::GetAmp()
{
    return m_amp;
}

void SimSig::SigDef::SetAmp(int amp)
{
    m_amp = amp;
}

QString SimSig::SigDef::GetDescription()
{
    return m_description_str;
}

QJsonObject SimSig::SigDef::GenJsonData()
{
    QJsonObject data_obj;
    data_obj["Type"] = GetType();
    data_obj["Amp"] = GetAmp();
    return data_obj;
}

void SimSig::SigDef::ParseJsonData(QJsonObject data_obj)
{
    SetType(static_cast<SigType>(data_obj["Type"].toInt()));
    SetAmp(data_obj["Amp"].toInt());
}
