#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QString>
#include <QList>

#include "sigdef.h"

struct ChannelData
{
    QString m_ch_name = "Default Name";
};

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(QObject* parent = nullptr);
    ~Channel();
    Channel(const Channel& _channel);
public:
    ChannelData& GetChData();
    qreal GenData(const quint64& t_ms);
private:
    QList<SigDef*> m_list_sig;
    ChannelData m_ch_data;
signals:

};

#endif // CHANNEL_H
