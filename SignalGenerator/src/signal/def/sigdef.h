
#ifndef SIGDEF_H
#define SIGDEF_H


#include <QObject>
#include <QString>
enum SigType
{
    Const = 0,
    Sin
};

class SigDef : public QObject
{
    Q_OBJECT
public:
    explicit SigDef(QObject* parent = nullptr): QObject{parent} {};
protected:
    qreal m_amp = 1;
public:
    qreal GetAmp()
    {
        return m_amp;
    }
    void SetAmp(qreal amp)
    {
        m_amp = amp;
    }
public:
    virtual qreal GenData(const quint64& t_ms) = 0;
    virtual QString GetDescription() = 0;
    virtual const SigType GetSigType() = 0;
    virtual SigDef* GetCopy() = 0;
signals:

};

#endif // SIGDEF_H
