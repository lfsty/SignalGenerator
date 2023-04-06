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

#include "channelwidget.h"

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
    void OnSettingChannel(ChannelWidget* channel_widget);
private slots:

private:
    Ui::ChannelSetting* ui;

};

#endif // CHANNELSETTING_H
