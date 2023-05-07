
#ifndef SIGREALEEG_H
#define SIGREALEEG_H

#include "sigdef.h"
#include <QList>

class SigRealEEG : public SigDef
{
public:
    explicit SigRealEEG(QObject* parent = nullptr);
public:
    virtual qreal GenData(const quint64& t_ms);
    virtual QString GetDescription();
    virtual const SigType GetSigType();
    virtual SigDef* GetCopy();
public:
    void SetEEGData(const QList<double>& eeg_data);
private:
    QList<double> m_data;
};

#endif // SIGREALEEG_H
