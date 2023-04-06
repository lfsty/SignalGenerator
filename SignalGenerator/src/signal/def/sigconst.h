
#ifndef SIGCONST_H
#define SIGCONST_H

#include "sigdef.h"



class SigConst : public SigDef
{
public:
    explicit SigConst(QObject* parent = nullptr);
public:
    virtual qreal GenData(qreal x);
    virtual QString GetDescription();
private:
    qreal m_const_data = 0;
public:
    void SetDC(const qreal& const_data);
    qreal GetDC();
};

#endif // SIGCONST_H
