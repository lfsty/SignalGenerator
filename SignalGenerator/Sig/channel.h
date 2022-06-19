#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QString>
#include <QList>
#include "sigall.h"
#include <QJsonArray>
#include <QJsonValue>
class Channel : public QObject
{
    Q_OBJECT
    friend class ChannelSetting;
public:
    explicit Channel(QObject *parent = nullptr);
    ~Channel();
    Channel *GetCopy(QObject *parent);

private:
    QString m_ch_name = "";
    QList<SimSig::SigDef *> m_list_sig;
    SimSig::SigDef *GenSigFromJsonObj(QJsonObject data_obj);

public:
    QString GetChName();
    void SetChName(QString name);
    QJsonObject GenJsonObject();
    void ParseJsonObject(QJsonObject data_obj);
    float GenData(quint64 tm_ms);

signals:

};

#endif // CHANNEL_H
