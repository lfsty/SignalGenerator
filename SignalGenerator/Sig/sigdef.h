#ifndef SIGDEF_H
#define SIGDEF_H
#include <QtGlobal>
#include <QtMath>
#include <QString>
#include <QJsonObject>
namespace SimSig
{
    enum SigType
    {
        Base = 0,
        Sin,
        Cos,
        Dc,
        WhiteNoise,
        Main50,
        Main60,
        RealEEG
    };

    class SigDef
    {
    public:
        SigDef();
        virtual ~SigDef();
    private:
        //信号类型
        SigType m_type = SigType::Base;
        //放大倍数
        int m_amp = 1;
    protected:
        QString m_description_str;
    protected:
        void SetType(SigType type);
    public:
        SigType GetType();
        int GetAmp();
        void SetAmp(int amp);
        virtual float GenData(qint64 tm_ms) = 0;
        virtual QString GetDescription();
        virtual void SetDescription() {};
        virtual QString GetTypeStr()
        {
            return "";
        };
        virtual SigDef* GetCopy()
        {
            return nullptr;
        };
        virtual QJsonObject GenJsonData();
        virtual void ParseJsonData(QJsonObject data_obj);
    };
}

#endif // SIGDEF_H
