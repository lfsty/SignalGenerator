#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dlg_channel_setting(this)
{
    ui->setupUi(this);
    {
        // 导联工作线程
        m_channel_work.moveToThread(&m_thread_channel);
        connect(this, &MainWindow::sig_AddNewChannel, &m_channel_work, &TotalChannelWork::AddNewChannel);
        connect(this, &MainWindow::sig_DelChannel, &m_channel_work, &TotalChannelWork::DelChannel);
        connect(this, &MainWindow::sig_UpdateChannelName, &m_channel_work, &TotalChannelWork::UpDateChannelName);
        connect(&m_channel_work, &TotalChannelWork::sig_UpDateChannelName, this, [ = ](int index, QString ch_name)
        {
            ChannelWidget* _tmp_chan_widget = static_cast<ChannelWidget*>(ui->m_verlayout_ch->itemAt(index)->widget());
            _tmp_chan_widget->SetChName(QString::number(index + 1) + "." + ch_name);
        });
        m_thread_channel.start();
    }
    {
        // 导联设置界面
        m_dlg_channel_setting.setModal(true);
        connect(&m_dlg_channel_setting, &ChannelSetting::sig_channel_data_changed, this, &MainWindow::UpdateChannel);
    }


    {
        //test
        connect(ui->m_pushbutton_send_frame, &QPushButton::clicked, this, [ = ]()
        {
            static quint64 _t_ms = 0;
            emit sig_ManGenFrameData(_t_ms);
            _t_ms += 10;
        });
        connect(this, &MainWindow::sig_ManGenFrameData, &m_channel_work, &TotalChannelWork::GenFrameData);
        connect(&m_channel_work, &TotalChannelWork::sig_GenFrameData, this, [ = ](const quint64 & t_ms, const QList<float>& framedata)
        {
            qDebug() << t_ms << "s";
            for(auto _data : framedata)
            {
                qDebug() << _data;
            }

            //    QByteArray _framedata;
            //    QDataStream _framedata_stream(&_framedata, QIODevice::WriteOnly);
            //    for(auto ch : m_list_channel)
            //    {
            //        _framedata_stream << ch->GenData(t_ms);
            //    }
        });
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

    // 删除导联
    connect(channel_widget, &ChannelWidget::sig_del_this, this, [ = ](ChannelWidget * channel_widget)
    {
        emit sig_DelChannel(ui->m_verlayout_ch->indexOf(channel_widget));
        ui->m_verlayout_ch->removeWidget(channel_widget);
        UpdateChannel();
        channel_widget->deleteLater();
    });
    // 拷贝导联
    connect(channel_widget, &ChannelWidget::sig_copy, this, [ = ](ChannelWidget * channel_widget)
    {
        emit sig_AddNewChannel(ui->m_verlayout_ch->indexOf(channel_widget));
        ChannelWidget* _channel_widget = new ChannelWidget(this);
        AddChannelWidget(_channel_widget);
    });
    //设置导联
    connect(channel_widget, &ChannelWidget::sig_open_settingDlg, this, [ = ](ChannelWidget * channel_widget)
    {
        int _ch_index = ui->m_verlayout_ch->indexOf(channel_widget);
        m_dlg_channel_setting.OnSettingChannel(m_channel_work.GetChannelAt(_ch_index));
    });

    UpdateChannel();
}

void MainWindow::UpdateChannel()
{
    int _current_ch_num =  ui->m_verlayout_ch->count() - 2;

    //更新导联名称
    for(int i = 0; i < _current_ch_num; i++)
    {
        emit sig_UpdateChannelName(i);
    }

    ui->m_lineedit_ch_num->setText(QString::number(_current_ch_num));
}


void MainWindow::on_m_pushbutton_add_ch_clicked()
{
    ChannelWidget* _channel_widget = new ChannelWidget(this);
    emit sig_AddNewChannel(-1);
    AddChannelWidget(_channel_widget);
}

