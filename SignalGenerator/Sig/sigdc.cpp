#include "sigdc.h"
#include <QDebug>
SimSig::SigDc::SigDc()
{
    SetType(SigType::Dc);
}

SimSig::SigDc::~SigDc()
{

}

void SimSig::SigDc::SetDC(int dc)
{
    m_dc = dc;
}

int SimSig::SigDc::GetDC()
{
    return m_dc;
}

float SimSig::SigDc::GenData(qint64 tm_ms)
{
    return m_dc;
}

void SimSig::SigDc::SetDescription()
{
    m_description_str = QString::asprintf("直流幅度:%d", GetDC());
}

QString SimSig::SigDc::GetTypeStr()
{
    return "DC";
}

SimSig::SigDef *SimSig::SigDc::GetCopy()
{
    SigDc *new_sig = new SigDc();
    new_sig->SetAmp(GetAmp());
    new_sig->SetDC(GetDC());
    new_sig->SetDescription();
    return new_sig;
}

QJsonObject SimSig::SigDc::GenJsonData()
{
    QJsonObject data_obj = __super::GenJsonData();
    data_obj["DC"] = GetDC();
    return data_obj;
}

void SimSig::SigDc::ParseJsonData(QJsonObject data_obj)
{
    __super::ParseJsonData(data_obj);
    SetDC(data_obj["DC"].toInt());
}
