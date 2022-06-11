#ifndef SIGMAIN_H
#define SIGMAIN_H

#include "sigdef.h"

namespace SimSig
{

    class SigMain: public SigDef
    {
    public:
        enum MainFreqType
        {
            Main50,
            Main60
        };
    private:
        MainFreqType m_main_freq_type = Main50;
    public:
        SigMain(MainFreqType main_type);
        virtual ~SigMain();
    public:
        virtual float GenData(qint64 tm_ms);
        virtual void SetDescription();
        virtual QString GetTypeStr();
        virtual SigDef *GetCopy();
    };
}
#endif // SIGMAIN_H
