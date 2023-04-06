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

float Channel::GenData(quint64 tm_ms)
{
    float data = 0;
//    for(auto sig : m_list_sig)
//    {
//        data += sig->GenData(tm_ms);
//    }
    return data;
}
