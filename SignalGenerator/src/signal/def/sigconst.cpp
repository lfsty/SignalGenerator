
#include "sigconst.h"

SigConst::SigConst(QObject* parent)
    : SigDef{parent}
{

}

qreal SigConst::GenData(const quint64& t_ms)
{
    return m_const_data * GetAmp();
}

QString SigConst::GetDescription()
{
    return QString("信号类型:%1 ; 直流分量:%2 ; 放大倍数:%3").arg("Const").arg(m_const_data).arg(GetAmp());
}

const SigType SigConst::GetSigType()
{
    return SigType::Const;
}

SigDef* SigConst::GetCopy()
{
    SigConst* _new_sig = new SigConst(this->parent());
    _new_sig->SetDC(GetDC());
    _new_sig->SetAmp(GetAmp());
    return _new_sig;
}

void SigConst::SetDC(const qreal& const_data)
{
    m_const_data = const_data;
}

qreal SigConst::GetDC()
{
    return m_const_data;
}
