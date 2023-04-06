
#ifndef SIGDEF_H
#define SIGDEF_H


#include <QObject>


class SigDef : public QObject
{
    Q_OBJECT
public:
    explicit SigDef(QObject* parent = nullptr);
private:
    float m_amp = 1;
public:
    virtual float GenData(qint64 tm_ms) = 0;
    virtual QString GetDescription() = 0;
signals:

};

#endif // SIGDEF_H
