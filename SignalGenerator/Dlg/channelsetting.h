#ifndef CHANNELSETTING_H
#define CHANNELSETTING_H

#include <QDialog>
#include "sigsetting.h"
#include <QString>
#include <QList>
#include "channel.h"

//绘图
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QLegendMarker>

#define CHART_POINT 1000
namespace Ui
{
    class ChannelSetting;
}

class ChannelSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ChannelSetting(Channel *from_channel, QWidget *parent = nullptr);
    ~ChannelSetting();
public:
    QString GetChName();
    Channel *GetChannel();
private:
    Channel *m_channel = nullptr;

private slots:
    void on_m_pushbutton_addsig_clicked();

    void on_update_data_chart();

//绘图
private:
    Ui::ChannelSetting *ui;
    QChart *m_chart = nullptr;
    QLineSeries *m_line_series = nullptr;
    QValueAxis *m_axisY = nullptr;
    QValueAxis *m_axisX = nullptr;
private:
    void init_chart();
};

#endif // CHANNELSETTING_H
