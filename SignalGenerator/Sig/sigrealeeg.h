#ifndef SIGREALEEG_H
#define SIGREALEEG_H
#include "sigdef.h"
namespace SimSig
{
    class SigRealEEG: public SigDef
    {
    public:
        SigRealEEG(quint64 length, float* data);
        virtual ~SigRealEEG();
    private:
        virtual float GenData(qint64 tm_ms);
        float* m_data = nullptr;
        quint64 m_length = 0;
    };
}

#endif // SIGREALEEG_H
