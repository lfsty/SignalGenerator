#ifndef CHANNELSETTING_H
#define CHANNELSETTING_H

#include <QDialog>
#include <QString>
#include <QList>

//绘图
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QLegendMarker>

#include "../def/channel.h"

#define CHART_POINT 1000
namespace Ui
{
    class ChannelSetting;
}

class ChannelSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ChannelSetting(QWidget* parent = nullptr);
    ~ChannelSetting();
    void OnSettingChannel(Channel* channel);
private slots:


private:
    Ui::ChannelSetting* ui;
    QChart* m_chart = nullptr;
    QLineSeries* m_line_series = nullptr;
    QValueAxis* m_axisY = nullptr;
    QValueAxis* m_axisX = nullptr;
signals:
    void sig_channel_data_changed();

};

#endif // CHANNELSETTING_H
