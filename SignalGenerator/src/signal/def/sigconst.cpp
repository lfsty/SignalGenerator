
#include "sigconst.h"

SigConst::SigConst(QObject* parent)
    : SigDef{parent}
{

}

qreal SigConst::GenData(const quint64& t_ms)
{
    return m_const_data * m_amp;
}

QString SigConst::GetDescription()
{
    return "直流分量：" + QString::number(m_const_data * m_amp);
}

void SigConst::SetDC(const qreal& const_data)
{
    m_const_data = const_data;
}

qreal SigConst::GetDC()
{
    return m_const_data;
}
