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
