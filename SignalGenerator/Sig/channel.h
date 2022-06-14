#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QString>
#include <QList>
#include "sigall.h"
class Channel : public QObject
{
    Q_OBJECT
    friend class ChannelSetting;
public:
    explicit Channel(QObject *parent = nullptr);
    ~Channel();

private:
    QString m_ch_name = "";
    QList<SimSig::SigDef *> m_list_sig;

public:
    QString GetChName();
    void SetChName(QString name);
public:
    float GenData(quint64 tm_ms);
    Channel *GetCopy(QObject *parent);

signals:

};

#endif // CHANNEL_H
