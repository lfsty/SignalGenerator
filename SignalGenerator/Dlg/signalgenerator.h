#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QMainWindow>
#include "serversetting.h"
#include "ampsetting.h"
#include "channelwidget.h"
#include <QTimer>
#include <QFile>
#include "channel.h"
#include <QJsonDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class SignalGenerator;
}
QT_END_NAMESPACE

class SignalGenerator : public QMainWindow
{
    Q_OBJECT

public:
    SignalGenerator(QWidget* parent = nullptr);
    ~SignalGenerator();
private:
    QTimer m_timer_gendata;
    QTimer m_timer_update_serverdata_s;
    QString m_save_file_path = "";

private slots:
    void on_m_setting_server_action_triggered();

    void on_m_setting_generator_action_triggered();

    void on_m_pushbutton_add_ch_clicked();

    void on_m_timer_gendata_timeout();

    void on_m_copy_channel_clicked(Channel* pChannel);

    void on_m_del_channel_clicked(ChannelWidget* pChannelWidget);

    void on_m_file_open_action_triggered();

    void on_m_file_save_action_triggered();

    void on_m_file_save_as_action_triggered();

    void on_m_pushbutton_send_frame_clicked();

    void on_m_pushbutton_mark_clicked();

    void keyPressEvent(QKeyEvent* e);


    void on_m_file_eeg_open_triggered();

private:
    Ui::SignalGenerator* ui;
    ServerSetting* m_serversetting_dialog = nullptr;
    AmpSetting* m_ampsetting_dialog = nullptr;
    QList<ChannelWidget*> m_channel_widget_list;

private:
    void UpDateChannelNum();
    void AddChnnel(ChannelWidget* new_ch_widget);
    QByteArray GenSettingDataByteArray();
    void ClearAllChannelWidget();
private:
    int m_send_frame_s = 0;
};
#endif // SIGNALGENERATOR_H
