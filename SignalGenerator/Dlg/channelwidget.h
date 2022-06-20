#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include "channelsetting.h"
#include <QString>
#include "channel.h"
namespace Ui
{
    class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelWidget(QWidget *parent = nullptr, QString ch_name = "");
    ChannelWidget(QWidget *parent, Channel *from_cpoy_channel);
    ~ChannelWidget();
    float GenData(quint64 tm_ms);
    QJsonObject GenJsonObject();
    void ParseJsonObject(QJsonObject data_obj);
private:
    Channel *m_channel = nullptr;

private slots:
    void on_m_pushButton_copy_clicked();

    void on_m_pushButton_setting_clicked();
public slots:
    void on_m_pushButton_delete_clicked();

private:
    Ui::ChannelWidget *ui;

signals:
    void ChannelDelete(ChannelWidget *);
    void CopyChannel(Channel *);

};

#endif // CHANNELWIDGET_H
