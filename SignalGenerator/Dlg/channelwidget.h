#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include "channelsetting.h"
#include <QString>
namespace Ui
{
    class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelWidget(QWidget *parent = nullptr);
    ChannelWidget(QWidget *parent, ChannelWidget *from_cpoy_widget);
    ~ChannelWidget();
    float GenData(quint64 tm_ms);
public:
    void SetChannelName(QString ch_name);

private slots:
    void on_m_pushButton_copy_clicked();

    void on_m_pushButton_setting_clicked();

    void on_m_pushButton_delete_clicked();

private:
    Ui::ChannelWidget *ui;
    ChannelSetting *m_channelsetting_dialog = nullptr;

signals:
    void ChannelDelete();
    void CopyChannel(ChannelWidget *);

};

#endif // CHANNELWIDGET_H
