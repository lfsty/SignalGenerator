#include "channelwidget.h"
#include "ui_channelwidget.h"

ChannelWidget::ChannelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);
    m_channelsetting_dialog = new ChannelSetting(this);
    m_channelsetting_dialog->setModal(true);
}

ChannelWidget::ChannelWidget(QWidget *parent, ChannelWidget *from_cpoy_widget):
    QWidget(parent),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);
    m_channelsetting_dialog = new ChannelSetting(this, from_cpoy_widget->m_channelsetting_dialog);
    m_channelsetting_dialog->setModal(true);
    SetChannelName(from_cpoy_widget->ui->m_label_name->text());
}

ChannelWidget::~ChannelWidget()
{
    delete ui;
    if(m_channelsetting_dialog != nullptr)
    {
        m_channelsetting_dialog->deleteLater();
    }
}

float ChannelWidget::GenData(quint64 tm_ms)
{
    return m_channelsetting_dialog->GenData(tm_ms);
}

void ChannelWidget::SetChannelName(QString ch_name)
{
    ui->m_label_name->setText(ch_name);
    m_channelsetting_dialog->SetChName(ch_name);
}

void ChannelWidget::on_m_pushButton_copy_clicked()
{
    emit CopyChannel(this);
}


void ChannelWidget::on_m_pushButton_setting_clicked()
{
    m_channelsetting_dialog->show();
    if(m_channelsetting_dialog->exec())
    {
        SetChannelName(m_channelsetting_dialog->GetChName());
    }
    else
    {
        qDebug() << "cancel";
    }
}


void ChannelWidget::on_m_pushButton_delete_clicked()
{
    this->deleteLater();
    emit ChannelDelete();
}
