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

#include <QTableWidgetItem>

#include "../def/channel.h"
#include "sigsetting.h"

//#define CHART_POINT 1000

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
    void on_m_pushButton_add_sig_clicked();

    void on_m_tableWidget_sig_itemDoubleClicked(QTableWidgetItem* item);

    void on_m_tableWidget_sig_itemClicked(QTableWidgetItem* item);

private:
    Ui::ChannelSetting* ui;
    Channel* m_current_channel = nullptr;
    SigSetting m_dlg_sigsetting;
private:
    //绘图
    QChart* m_chart = nullptr;
    QLineSeries* m_line_series = nullptr;
    QValueAxis* m_axisY = nullptr;
    QValueAxis* m_axisX = nullptr;
private:
    void UpdateChartData(const QList<QPointF>& example_data);
    void UpdateSigsDesp();
signals:
    void sig_channel_data_changed();

};

#endif // CHANNELSETTING_H
