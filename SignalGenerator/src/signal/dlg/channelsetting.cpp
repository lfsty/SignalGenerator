#include "channelsetting.h"
#include "ui_channelsetting.h"

ChannelSetting::ChannelSetting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ChannelSetting)
{
    ui->setupUi(this);
}

ChannelSetting::~ChannelSetting()
{
    delete ui;
}

void ChannelSetting::OnSettingChannel(ChannelWidget* channel_widget)
{
    this->show();

    qDebug() << channel_widget->GetChannelData().m_ch_name;

}


