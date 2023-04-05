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

QString Channel::GetChName()
{
    return m_ch_name;
}

void Channel::SetChName(QString name)
{
    m_ch_name = name;
}

QJsonObject Channel::GenJsonObject()
{
    QJsonObject data_obj;
    QJsonArray sig_array;
    for(auto sig : m_list_sig)
    {
        sig_array.append(QJsonValue(sig->GenJsonData()));
    }

    data_obj["ChName"] = GetChName();
    data_obj["SigData"] = sig_array;

    return data_obj;
}

void Channel::ParseJsonObject(QJsonObject data_obj)
{
    SetChName(data_obj["ChName"].toString());
    if(data_obj["SigData"].isArray())
    {
        QJsonArray sig_array = data_obj["SigData"].toArray();
        for(auto iter : sig_array)
        {
            SimSig::SigDef* m_sig = GenSigFromJsonObj(iter.toObject());
            m_list_sig.push_back(m_sig);
        }
    }
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

void Channel::SetRealEEGChannel(quint64 length, float* data)
{
    SimSig::SigDef* m_sig = new SimSig::SigRealEEG(length, data);
    m_list_sig.push_back(m_sig);
}

Channel* Channel::GetCopy(QObject* parent)
{
    Channel* new_channel = new Channel(parent);
    new_channel->SetChName(this->GetChName());
    for(auto sig : m_list_sig)
    {
        new_channel->m_list_sig.push_back(sig->GetCopy());
    }


    return new_channel;
}

SimSig::SigDef* Channel::GenSigFromJsonObj(QJsonObject data_obj)
{
    SimSig::SigDef* m_sig = nullptr;
    switch(static_cast<SimSig::SigType>(data_obj["Type"].toInt()))
    {
        case SimSig::SigType::Sin :
            {
                m_sig = new SimSig::SigSin();
                break;
            }
        case SimSig::SigType::Cos :
            {
                m_sig = new SimSig::SigCos();
                break;
            }
        case SimSig::SigType::Dc:
            {
                m_sig = new SimSig::SigDc();
                break;
            }
        case SimSig::SigType::WhiteNoise:
            {
                qDebug() << "white noise";
                break;
            }
        case SimSig::SigType::Main50:
            {
                m_sig = new SimSig::SigMain(SimSig::SigMain::Main50);
                break;
            }
        case SimSig::SigType::Main60:
            {
                m_sig = new SimSig::SigMain(SimSig::SigMain::Main60);
                break;
            }
        default:
            break;
    }
    if(m_sig != nullptr)
    {
        m_sig->ParseJsonData(data_obj);
        m_sig->SetDescription();
    }
    return m_sig;
}
