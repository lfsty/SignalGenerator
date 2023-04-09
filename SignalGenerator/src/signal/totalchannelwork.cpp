
#include "totalchannelwork.h"
#include <QDataStream>
#include <QIODevice>
#include <QDebug>
#include "def/sigconst.h"
#include "def/sigsin.h"
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
    emit sig_ChannelAdded();
}

void TotalChannelWork::AddNewChannelFromJson(const QJsonObject& sig_list_json_data)
{
    Channel* _tmp_channel = new Channel(this);
    _tmp_channel->GetChData().m_ch_name = sig_list_json_data["ChName"].toString();

    QJsonArray _tmp_sig_array = sig_list_json_data["SigData"].toArray();
    for(auto single_sig_json_data : _tmp_sig_array)
    {
        QJsonObject _sig_data = single_sig_json_data.toObject();
        SigDef* _tmp_sig = nullptr;
        switch(_sig_data["Type"].toInt())
        {
            case SigType::Const:
                {
                    SigConst* _tmp_sig_const = new SigConst();
                    _tmp_sig_const->SetAmp(_sig_data["Amp"].toDouble());
                    _tmp_sig_const->SetDC(_sig_data["DC"].toDouble());
                    _tmp_sig = _tmp_sig_const;
                    break;
                }
            case SigType::Sin:
                {
                    SigSin* _tmp_sig_sin = new SigSin();
                    _tmp_sig_sin->SetAmp(_sig_data["Amp"].toDouble());
                    _tmp_sig_sin->SetFreq(_sig_data["Freq"].toDouble());
                    _tmp_sig_sin->SetPhase(_sig_data["Phase"].toInteger());
                    _tmp_sig = _tmp_sig_sin;
                    break;
                }
        }
        assert(_tmp_sig != nullptr);
        _tmp_channel->AddSig(_tmp_sig);
    }
    m_list_channel.push_back(_tmp_channel);
    emit sig_ChannelAdded();
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

void TotalChannelWork::GenFrameData(const quint64& t_ms)
{
    QList<float> _frame_data;
    for(auto ch : m_list_channel)
    {
        _frame_data << ch->GenData(t_ms);
    }

    emit sig_GenFrameData(_frame_data);
}

Channel* TotalChannelWork::GetChannelAt(int index)
{
    assert(index < m_list_channel.size() && index >= 0);
    return m_list_channel.at(index);
}

QJsonArray TotalChannelWork::GenAllChJsonData()
{
    QJsonArray _ch_json_data_array;
    for(auto ch : m_list_channel)
    {
        _ch_json_data_array.append(QJsonValue(ch->GenChJsonData()));
    }
    return _ch_json_data_array;
}

