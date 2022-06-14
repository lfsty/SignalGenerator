#include "channelwidget.h"
#include "ui_channelwidget.h"

ChannelWidget::ChannelWidget(QWidget *parent, QString ch_name) :
    QWidget(parent),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);
    m_channel = new Channel(this);

    ui->m_label_name->setText(ch_name);
    m_channel->SetChName(ch_name);

    setAttribute(Qt::WA_StyledBackground);
}

ChannelWidget::ChannelWidget(QWidget *parent, Channel *from_cpoy_channel):
    QWidget(parent),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);

    if(from_cpoy_channel != nullptr)
    {
        m_channel = from_cpoy_channel->GetCopy(this);
        ui->m_label_name->setText(m_channel->GetChName());
    }
    else
    {
        m_channel = new Channel(this);
    }

    setAttribute(Qt::WA_StyledBackground);
}

ChannelWidget::~ChannelWidget()
{
    delete ui;
    if(m_channel != nullptr)
    {
        m_channel->deleteLater();
    }
}

float ChannelWidget::GenData(quint64 tm_ms)
{
    return m_channel->GenData(tm_ms);
}

void ChannelWidget::on_m_pushButton_copy_clicked()
{
    emit CopyChannel(this->m_channel);
}


void ChannelWidget::on_m_pushButton_setting_clicked()
{
    ChannelSetting w(m_channel, this);
    w.setModal(true);
    w.show();
    if(w.exec())
    {
        m_channel->deleteLater();
        m_channel = w.GetChannel();
        ui->m_label_name->setText(w.GetChName());
        m_channel->SetChName(w.GetChName());
    }
}


void ChannelWidget::on_m_pushButton_delete_clicked()
{
    this->deleteLater();
    emit ChannelDelete();
}
