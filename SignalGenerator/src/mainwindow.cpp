#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dlg_channel_setting(this)
{
    ui->setupUi(this);
    {
        // UI界面初始化
        connect(this, &MainWindow::sig_ChannelNumChanged, this, &MainWindow::UpdateChannelNum);
    }
    {
        // 导联设置界面
        m_dlg_channel_setting.setModal(true);
    }
    {
        // 导联
        m_channel_work.moveToThread(&m_thread_channel);
        connect(this, &MainWindow::sig_AddNewChannel, &m_channel_work, &TotalChannelWork::AddNewChannel);
        connect(this, &MainWindow::sig_DelChannel, &m_channel_work, &TotalChannelWork::DelChannel);
        m_thread_channel.start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    m_thread_channel.quit();
    m_thread_channel.wait();
}

void MainWindow::AddChannelWidget(ChannelWidget* channel_widget)
{
    ui->m_verlayout_ch->insertWidget(ui->m_verlayout_ch->indexOf(ui->m_pushbutton_add_ch), channel_widget);

    emit sig_AddNewChannel(channel_widget->GetChannelData().m_ch_index);

    // 删除导联
    connect(channel_widget, &ChannelWidget::destroyed, this, [ = ](QObject * pwidget)
    {
        ChannelWidget* _tmp_chan_widget = static_cast<ChannelWidget*>(pwidget);
        emit sig_DelChannel(ui->m_verlayout_ch->indexOf(_tmp_chan_widget));
        ui->m_verlayout_ch->removeWidget(_tmp_chan_widget);
        UpdateChannelNum();
    });
    // 拷贝导联
    connect(channel_widget, &ChannelWidget::sig_copy, this, &MainWindow::AddChannelWidget);
    //设置导联
    connect(channel_widget, &ChannelWidget::sig_open_settingDlg, &m_dlg_channel_setting, &ChannelSetting::OnSettingChannel);

    emit sig_ChannelNumChanged();
}

void MainWindow::UpdateChannelNum()
{
    int _current_ch_num =  ui->m_verlayout_ch->count() - 2;

    //更新index
    for(int i = 0; i < _current_ch_num; i++)
    {
        ChannelWidget* _tmp_chan_widget = static_cast<ChannelWidget*>(ui->m_verlayout_ch->itemAt(i)->widget());
        _tmp_chan_widget->GetChannelData().m_ch_index = i + 1;
        _tmp_chan_widget->UpDateName();
    }

    ui->m_lineedit_ch_num->setText(QString::number(_current_ch_num));
}


void MainWindow::on_m_pushbutton_add_ch_clicked()
{
    ChannelWidget* _channel_widget = new ChannelWidget(this);

    AddChannelWidget(_channel_widget);
}

