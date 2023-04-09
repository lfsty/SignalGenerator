
#include "sigsin.h"
#include <QtMath>

SigSin::SigSin(QObject* parent)
    : SigDef{parent}
{

}

qreal SigSin::GenData(const quint64& t_ms)
{
    qreal x = static_cast<qreal>((t_ms + GetPhase())) / 1000;
    return (qSin(2 * M_PI * GetFreq() * x)) * GetAmp();
}

QString SigSin::GetDescription()
{
    return QString("信号类型:%1 ; 放大倍数:%2 ; 频率:%3Hz ; 相位:%4ms").arg("Sin").arg(GetAmp()).arg(GetFreq()).arg(GetPhase());
}

const SigType SigSin::GetSigType()
{
    return SigType::Sin;
}

SigDef* SigSin::GetCopy()
{
    SigSin* _new_sig = new SigSin(this->parent());
    _new_sig->SetAmp(GetAmp());
    _new_sig->SetFreq(GetFreq());
    _new_sig->SetPhase(GetPhase());
    return _new_sig;
}

QJsonObject SigSin::GenJsonData()
{
    QJsonObject _json_data = __super::GenJsonData();
    _json_data["Freq"] = GetFreq();
    _json_data["Phase"] = static_cast<qint64>(GetPhase());
    return _json_data;
}

