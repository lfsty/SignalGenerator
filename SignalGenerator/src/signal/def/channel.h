#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QString>
#include <QList>

#include "sigdef.h"

struct ChannelData
{
    QString m_ch_name = "Default Name";
    int m_ch_index = -1;
};

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(QObject* parent = nullptr);
    ~Channel();

private:
    QList<SigDef*> m_list_sig;

public:
    float GenData(quint64 tm_ms);
signals:

};

#endif // CHANNEL_H
