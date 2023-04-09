#include "channel.h"

Channel::Channel(QObject* parent)
    : QObject{parent}
{
    {
        m_example_data.resize(EXAMPLE_DATA_NUM);
        for(int i = 0 ; i < EXAMPLE_DATA_NUM; i++)
        {
            m_example_data[i] = QPointF(double(i) / 1000, GenData(i));
        }
    }
    connect(this, &Channel::sig_update_parent, this, [ = ](SigDef * sig)
    {
        sig->setParent(this);
    });
}

Channel::~Channel()
{
    //其实没必要
    for(auto sig : m_list_sig)
    {
        m_list_sig.removeOne(sig);
        sig->deleteLater();
    }
}

Channel::Channel(const Channel& _channel)
{
    // 信号复制，没写
    this->setParent(_channel.parent());

    m_ch_data = _channel.m_ch_data;
    m_example_data = _channel.m_example_data;
    for(SigDef* _sig : _channel.m_list_sig)
    {
        SigDef* _new_sig = _sig->GetCopy();
        _new_sig->setParent(this);
        AddSig(_new_sig);
    }
}

void Channel::AddSig(SigDef* sig)
{
    assert(sig != nullptr);
    assert(sig->thread() == this->thread());
    emit sig_update_parent(sig);
//    sig->setParent(this);
    m_list_sig.append(sig);
    SigChanged();
}

void Channel::DelSig(int index)
{
    assert(index < m_list_sig.size() && index >= 0);
    SigDef* psig = m_list_sig.at(index);
    m_list_sig.removeAt(index);
    psig->deleteLater();
    SigChanged();
}

void Channel::SetSig(int index, SigDef* sig)
{
    assert(sig != nullptr);
    assert(index < m_list_sig.size() && index >= 0);
    DelSig(index);
//    sig->setParent(this);
    emit sig_update_parent(sig);
    m_list_sig.insert(index, sig);
    SigChanged();
}

QJsonObject Channel::GenChJsonData()
{
    QJsonObject _json_obj;
    QJsonArray _sig_array;
    for(auto sig : m_list_sig)
    {
        _sig_array.append(QJsonValue(sig->GenJsonData()));
    }
    _json_obj["ChName"] = m_ch_data.m_ch_name;
    _json_obj["SigData"] = _sig_array;

    return _json_obj;
}

const QList<QPointF>& Channel::GetExampleData()
{
    return m_example_data;
}

const QList<SigDef*> Channel::GetSigList()
{
    return m_list_sig;
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

void Channel::SigChanged()
{
    for(int i = 0 ; i < EXAMPLE_DATA_NUM; i++)
    {
        m_example_data[i] = QPointF(double(i) / 1000, GenData(i));
    }
    emit sig_example_data_changed(m_example_data);
}

