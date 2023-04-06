#include "channelwidget.h"
#include "ui_channelwidget.h"

ChannelWidget::ChannelWidget(QWidget* parent, const ChannelData& channel_data) :
    QWidget(parent),
    m_ch_data(channel_data),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground);
}

ChannelWidget::~ChannelWidget()
{
    delete ui;
}

ChannelData& ChannelWidget::GetChannelData()
{
    return m_ch_data;
}

void ChannelWidget::SetChannelData(const ChannelData& channel_data)
{
    m_ch_data = channel_data;
}

void ChannelWidget::UpDateName()
{
    ui->m_label_name->setText(QString::number(m_ch_data.m_ch_index) + "." + m_ch_data.m_ch_name);
}

void ChannelWidget::on_m_pushButton_delete_clicked()
{
    this->deleteLater();
}


void ChannelWidget::on_m_pushButton_copy_clicked()
{
    ChannelWidget* _new_channel = new ChannelWidget(this->parentWidget(), m_ch_data);

    emit sig_copy(_new_channel);
}


void ChannelWidget::on_m_pushButton_setting_clicked()
{
    emit sig_open_settingDlg(this);
}

