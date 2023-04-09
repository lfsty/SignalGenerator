#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>


#include "signal/dlg/channelwidget.h"
#include "signal/dlg/channelsetting.h"
#include "signal/totalchannelwork.h"
#include "communicate/serversetting.h"
#include "communicate/communicate.h"
#include "timergen/timergen.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    // 向UI界面添加新导联
    void AddChannelWidget(ChannelWidget* channel_widget);
    // 更新当前导联信息
    void UpdateChannel();

    bool SaveChSettingDataToFile(QString file_path);
    void ClearAllChannel();

private slots:
    void on_m_pushbutton_add_ch_clicked();

    void on_m_setting_server_action_triggered();

    void on_m_pushButton_open_siggen_clicked();
    void TCPServerStatusChanged(bool enable);

    void on_m_comboBox_freq_select_currentTextChanged(const QString& arg1);

    void closeEvent(QCloseEvent* event);

    void on_m_file_save_as_action_triggered();

    void on_m_file_save_action_triggered();

    void on_m_file_open_action_triggered();

    void on_m_file_eeg_open_triggered();

    void on_m_pushButton_setting_init_clicked();

private:
    Ui::MainWindow* ui;

private:
    ChannelSetting m_dlg_channel_setting;

    QThread m_thread_timer;
    TimerGen m_timer_gen;

    QThread m_thread_communicate;
    Communicate m_communicate;
    ServerSetting m_dlg_server_setting;

    QThread m_thread_channel;
    TotalChannelWork m_channel_work;

    QString m_file_setting_path = "";
signals:
    void sig_AddNewChannel(int);
    void sig_AddNewChannelFromJson(const QJsonObject& sig_list_json_data);
    void sig_AddNewChannelFromRealEEG(const QString ch_name, const QList<float> eeg_data);
    void sig_DelChannel(int);
    void sig_UpdateChannelName(int);
    void sig_ManGenFrameData(quint64 t_ms);

    // dlg监听
    void sig_OpenTCPServer();
    // communicate监听
    void sig_CloseTCPServer();

    void sig_SetTimerInterval(const int& interval_ms);

    //程序关闭消息
    void sig_destroy();
};

#endif // MAINWINDOW_H
