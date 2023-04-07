
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
    Channel* GetChannelAt(int index);

public slots:
    void UpDateChannelName(int index);
    void GenFrameData(const quint64& t_ms);
private:
    QList<Channel*> m_list_channel;

signals:
    void sig_UpDateChannelName(int index, QString ch_name);
    void sig_GenFrameData(const quint64& t_ms, const QList<float>& framedata);
};

#endif // TOTALCHANNELWORK_H
