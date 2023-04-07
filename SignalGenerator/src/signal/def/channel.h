#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPointF>

#include "sigdef.h"
#define EXAMPLE_DATA_NUM 1000

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
    void AddSig(SigDef* sig);
    void DelSig(int index);
    //设置信号，本质为删除index的，在index处添加新的
    void SetSig(int index, SigDef* sig);
    const QList<QPointF>& GetExampleData();
    const QList<SigDef*> GetSigList();

    ChannelData& GetChData();
    qreal GenData(const quint64& t_ms);
private:
    void SigChanged();
private:
    QList<SigDef*> m_list_sig;
    ChannelData m_ch_data;
    QList<QPointF> m_example_data;
signals:
    void sig_example_data_changed(const QList<QPointF>&);
    // 设置此信号parent，多线程引起
    void sig_update_parent(SigDef* sig);
};

#endif // CHANNEL_H
