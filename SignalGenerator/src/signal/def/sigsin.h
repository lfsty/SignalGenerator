
#ifndef SIGSIN_H
#define SIGSIN_H

#include "sigdef.h"



class SigSin : public SigDef
{
public:
    explicit SigSin(QObject* parent = nullptr);
public:
    virtual qreal GenData(const quint64& t_ms);
    virtual QString GetDescription();
    virtual const SigType GetSigType();
    virtual SigDef* GetCopy();
private:
    quint64 m_phase = 0;
    qreal m_freq = 0;
public:
    quint64 GetPhase()
    {
        return m_phase;
    }
    void SetPhase(quint64 phase)
    {
        m_phase = phase;
    }
    qreal GetFreq()
    {
        return m_freq;
    }
    void SetFreq(qreal freq)
    {
        m_freq = freq;
    }
};

#endif // SIGSIN_H
