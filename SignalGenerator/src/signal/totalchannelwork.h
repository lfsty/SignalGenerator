
#ifndef TOTALCHANNELWORK_H
#define TOTALCHANNELWORK_H

#include <QObject>
#include <QList>
#include "def/channel.h"
#include <QJsonArray>
#include "function/BDFHeader.h"

class TotalChannelWork : public QObject
{
    Q_OBJECT
public:
    explicit TotalChannelWork(QObject* parent = nullptr);
public:
    Channel* GetChannelAt(int index);
    QJsonArray GenAllChJsonData();

public slots:
    void UpDateChannelName(int index);
    void GenFrameData(const quint64& t_ms);
    void DelChannel(const int& index);
    void AddNewChannel(const int& copy_index = -1);
    void AddNewChannelFromJson(const QJsonObject& sig_list_json_data);
//    void AddNewChannelFromRealEEG(const QString ch_name, const QList<float> eeg_data);
    void AddNewChannelFromRealEEGNeuracle(const std::string dir_path);
private:
    QList<Channel*> m_list_channel;
signals:
    void sig_UpDateChannelName(int index, QString ch_name);
    void sig_GenFrameData(const QList<float>& framedata);
    void sig_ChannelAdded();
    void sig_RealEEGDataLoaded(BDF_INFO);
};

#endif // TOTALCHANNELWORK_H
