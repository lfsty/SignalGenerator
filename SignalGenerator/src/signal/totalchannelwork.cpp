
#include "totalchannelwork.h"

TotalChannelWork::TotalChannelWork(QObject* parent)
    : QObject{parent}
{

}

void TotalChannelWork::AddNewChannel(const int& copy_index)
{
    // copy_index<0表示新建 >0表示拷贝
    // 还没写
    Channel* _tmp_channel = new Channel(this);
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

