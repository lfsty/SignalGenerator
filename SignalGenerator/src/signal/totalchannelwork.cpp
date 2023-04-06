
#include "totalchannelwork.h"
#include <QDataStream>
#include <QIODevice>
TotalChannelWork::TotalChannelWork(QObject* parent)
    : QObject{parent}
{

}

void TotalChannelWork::AddNewChannel(const int& copy_index)
{
    // copy_index<0表示新建 >0表示拷贝
    assert(copy_index < m_list_channel.size());
    Channel* _tmp_channel = nullptr;
    if(copy_index >= 0)
    {
        _tmp_channel = new Channel(*m_list_channel.at(copy_index));
    }
    else
    {
        _tmp_channel = new Channel(this);
    }
    m_list_channel.push_back(_tmp_channel);
}

void TotalChannelWork::DelChannel(const int& index)
{
    if(index >= m_list_channel.size() || index < 0)
    {
        return;
    }
    else
    {
        m_list_channel[index]->deleteLater();
        m_list_channel.removeAt(index);
    }
}

void TotalChannelWork::UpDateChannelName(int index)
{
    assert(index < m_list_channel.size() && index >= 0);
    emit sig_UpDateChannelName(index, m_list_channel.at(index)->GetChData().m_ch_name);
}

void TotalChannelWork::GenFrameData()
{
    int freq = 500;
    static qreal t_ms = 0;
    t_ms += 1.0 / freq;

    QByteArray _framedata;
    QDataStream _framedata_stream(&_framedata, QIODevice::WriteOnly);
    for(auto ch : m_list_channel)
    {
        _framedata_stream << ch->GenData(t_ms);
    }

}

Channel* TotalChannelWork::GetChannelAt(int index)
{
    assert(index < m_list_channel.size() && index >= 0);
    return m_list_channel.at(index);
}

