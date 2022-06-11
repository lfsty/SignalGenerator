#ifndef CHANNELSETTING_H
#define CHANNELSETTING_H

#include <QDialog>
#include "sigsetting.h"
#include <QString>
#include "sigdc.h"
#include <QList>
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
    explicit ChannelSetting(QWidget *parent = nullptr);
    ChannelSetting(QWidget *parent, ChannelSetting *from_copy_widget);
    ~ChannelSetting();
public:
    QString GetChName();
    void SetChName(QString name);
    float GenData(quint64 tm_ms);
private:
    QList<SimSig::SigDef *> m_list_sig;
    QList<SimSig::SigDef *> GetSigListCopy();


private slots:
    void on_m_pushbutton_addsig_clicked();

    void on_update_data_chart();


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
