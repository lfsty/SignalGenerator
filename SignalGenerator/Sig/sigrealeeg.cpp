#include "sigrealeeg.h"

SimSig::SigRealEEG::SigRealEEG(quint64 length, float* data)
{
    m_length = length;
    m_data = new float[length];
    memcpy(m_data, data, length * sizeof(float));
}

SimSig::SigRealEEG::~SigRealEEG()
{
    delete[] m_data;
}

float SimSig::SigRealEEG::GenData(qint64 tm_ms)
{
    return m_data[tm_ms % m_length];
}
