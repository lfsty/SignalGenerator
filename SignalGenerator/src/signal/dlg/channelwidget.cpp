#include "channelwidget.h"
#include "ui_channelwidget.h"

ChannelWidget::ChannelWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground);
}

ChannelWidget::~ChannelWidget()
{
    delete ui;
}

void ChannelWidget::SetChName(QString ch_name)
{
    ui->m_label_name->setText(ch_name);
}

void ChannelWidget::on_m_pushButton_delete_clicked()
{
    emit sig_del_this(this);
}


void ChannelWidget::on_m_pushButton_copy_clicked()
{
    emit sig_copy(this);
}


void ChannelWidget::on_m_pushButton_setting_clicked()
{
    emit sig_open_settingDlg(this);
}

