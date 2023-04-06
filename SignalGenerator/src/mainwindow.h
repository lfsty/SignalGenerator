#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>


#include "signal/dlg/channelwidget.h"
#include "signal/dlg/channelsetting.h"
#include "signal/totalchannelwork.h"
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
private slots:
    void on_m_pushbutton_add_ch_clicked();

private:
    Ui::MainWindow* ui;

private:
    ChannelSetting m_dlg_channel_setting;

    QThread m_thread_channel;
    TotalChannelWork m_channel_work;
signals:
    void sig_AddNewChannel(int);
    void sig_DelChannel(int);
    void sig_UpdateChannelName(int);
};

#endif // MAINWINDOW_H
