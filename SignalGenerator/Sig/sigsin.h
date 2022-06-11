#ifndef SIGSIN_H
#define SIGSIN_H
#include "sigdef.h"

namespace SimSig
{
    class SigSin : public SigDef
    {
    public:
        SigSin();
        virtual ~SigSin();
    private:
        int m_freq = 1000;
        int m_phase_ms = 0;
    public:
        int GetFreq();
        void SetFreq(int freq);
        int GetPhaseMs();
        void SetPhaseMs(int phase_ms);
        virtual float GenData(qint64 tm_ms);
        virtual void SetDescription();
        virtual QString GetTypeStr();
        virtual SigDef *GetCopy();
    };
}

#endif // SIGSIN_H
