#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include <QString>
#include "../def/channel.h"

namespace Ui
{
    class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelWidget(QWidget* parent = nullptr, const ChannelData& channel_data = {});
    ~ChannelWidget();
public:
    ChannelData& GetChannelData();
    void SetChannelData(const ChannelData& channel_data);
    void UpDateName();
private:
    ChannelData m_ch_data;

private slots:
    void on_m_pushButton_delete_clicked();

    void on_m_pushButton_copy_clicked();

    void on_m_pushButton_setting_clicked();

private:
    Ui::ChannelWidget* ui;

signals:
    void sig_copy(ChannelWidget*);
    void sig_open_settingDlg(ChannelWidget*);

};

#endif // CHANNELWIDGET_H
