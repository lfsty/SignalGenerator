#include "channel.h"

Channel::Channel(QObject* parent)
    : QObject{parent}
{

}

Channel::~Channel()
{
    for(auto sig : m_list_sig)
    {
        m_list_sig.removeOne(sig);
        delete sig;
        sig = nullptr;
    }
}

Channel::Channel(const Channel& _channel)
{
    // 信号复制，没写
    m_ch_data = _channel.m_ch_data;
}

ChannelData& Channel::GetChData()
{
    return m_ch_data;
}

qreal Channel::GenData(const quint64& t_ms)
{
    qreal data = 0;
    for(auto sig : m_list_sig)
    {
        data += sig->GenData(t_ms);
    }
    return data;
}

