#include "channel.h"

Channel::Channel(QObject *parent)
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

QString Channel::GetChName()
{
    return m_ch_name;
}

void Channel::SetChName(QString name)
{
    m_ch_name = name;
}

float Channel::GenData(quint64 tm_ms)
{
    float data = 0;
    for(auto sig : m_list_sig)
    {
        data += sig->GenData(tm_ms);
    }
    return data;
}

Channel *Channel::GetCopy(QObject *parent)
{
    Channel *new_channel = new Channel(parent);
    new_channel->SetChName(this->GetChName());
    for(auto sig : m_list_sig)
    {
        new_channel->m_list_sig.push_back(sig->GetCopy());
    }


    return new_channel;
}
