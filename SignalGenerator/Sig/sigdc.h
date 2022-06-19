#ifndef SIGDC_H
#define SIGDC_H
#include "sigdef.h"

namespace SimSig
{

    class SigDc: public SigDef
    {
    public:
        SigDc();
        virtual ~SigDc();
    private:
        int m_dc = 0;
    public:
        void SetDC(int dc);
        int GetDC();
        virtual float GenData(qint64 tm_ms);
        virtual void SetDescription();
        virtual QString GetTypeStr();
        virtual SigDef *GetCopy();
        virtual QJsonObject GenJsonData();
        virtual void ParseJsonData(QJsonObject data_obj);
    };
}
#endif // SIGDC_H
