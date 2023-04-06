
#ifndef TOTALCHANNELWORK_H
#define TOTALCHANNELWORK_H

#include <QObject>
#include <QList>
#include "def/channel.h"

class TotalChannelWork : public QObject
{
    Q_OBJECT
public:
    explicit TotalChannelWork(QObject* parent = nullptr);
public:
    void AddNewChannel(const int& copy_index = -1);
    void DelChannel(const int& index);
private:
    QList<Channel*> m_list_channel;

signals:

};

#endif // TOTALCHANNELWORK_H
