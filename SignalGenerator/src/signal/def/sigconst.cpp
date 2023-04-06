
#include "sigconst.h"

SigConst::SigConst(QObject* parent)
    : SigDef{parent}
{

}

qreal SigConst::GenData(qreal x)
{
    return m_const_data * m_amp;
}

QString SigConst::GetDescription()
{
    return "test_desc";
}

void SigConst::SetDC(const qreal& const_data)
{
    m_const_data = const_data;
}

qreal SigConst::GetDC()
{
    return m_const_data;
}
