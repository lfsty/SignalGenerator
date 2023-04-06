
#ifndef SIGDEF_H
#define SIGDEF_H


#include <QObject>


class SigDef : public QObject
{
    Q_OBJECT
public:
    explicit SigDef(QObject* parent = nullptr);
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
    virtual qreal GenData(qreal x) = 0;
    virtual QString GetDescription() = 0;
signals:

};

#endif // SIGDEF_H
