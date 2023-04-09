#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QFile>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dlg_channel_setting(this)
    , m_dlg_server_setting(this)
{
    ui->setupUi(this);
    {
        // 导联工作线程
        m_channel_work.moveToThread(&m_thread_channel);
        connect(this, &MainWindow::sig_AddNewChannel, &m_channel_work, &TotalChannelWork::AddNewChannel);
        connect(this, &MainWindow::sig_DelChannel, &m_channel_work, &TotalChannelWork::DelChannel);
        connect(this, &MainWindow::sig_UpdateChannelName, &m_channel_work, &TotalChannelWork::UpDateChannelName);
        connect(this, &MainWindow::sig_AddNewChannelFromJson, &m_channel_work, &TotalChannelWork::AddNewChannelFromJson);
        connect(this, &MainWindow::sig_AddNewChannelFromRealEEG, &m_channel_work, &TotalChannelWork::AddNewChannelFromRealEEG);
        connect(&m_channel_work, &TotalChannelWork::sig_ChannelAdded, this, [ = ]()
        {
            ChannelWidget* _channel_widget = new ChannelWidget(this);
            AddChannelWidget(_channel_widget);
        });
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
        m_dlg_server_setting.setModal(true);

        //服务器设置
        m_communicate.MoveToThread(&m_thread_communicate);
        //打开
        connect(this, &MainWindow::sig_OpenTCPServer, &m_dlg_server_setting, &ServerSetting::OpenTCPServer);
        connect(&m_dlg_server_setting, &ServerSetting::sig_open_TCPServer, &m_communicate, &Communicate::OpenTCPServer);

        //关闭
        connect(this, &MainWindow::sig_CloseTCPServer, &m_communicate, &Communicate::CloseTCPServer);
        connect(this, &MainWindow::sig_destroy, &m_communicate, &Communicate::CloseTCPServer);

        //状态切换
        connect(&m_communicate, &Communicate::sig_TCPServer_status_changed, this, &MainWindow::TCPServerStatusChanged);
        connect(&m_communicate, &Communicate::sig_TCPServer_status_changed, &m_dlg_server_setting, &ServerSetting::TCPServerStatusChanged);

        m_thread_communicate.start();
    }
    {
        //时钟信号
        m_timer_gen.SetTimerInterval(1000 / ui->m_comboBox_freq_select->currentText().toInt());
        m_timer_gen.MoveToThread(&m_thread_timer);
        connect(this, &MainWindow::sig_SetTimerInterval, &m_timer_gen, &TimerGen::SetTimerInterval);
        //帧数统计
        connect(&m_communicate, &Communicate::sig_TCP_frame_send, &m_timer_gen, &TimerGen::FrameSended);
        connect(&m_timer_gen, &TimerGen::sig_frame_count_s, ui->m_lcd_send_frame_num, static_cast<void(QLCDNumber::*)(int)>(&QLCDNumber::display));

        connect(this, &MainWindow::sig_destroy, &m_timer_gen, &TimerGen::Destroy);

        //时钟开关
        connect(&m_communicate, &Communicate::sig_TCPServer_status_changed, &m_timer_gen, &TimerGen::SetTimerEnable);

        //信号传递
        connect(ui->m_pushbutton_send_frame, &QPushButton::clicked, &m_timer_gen, &TimerGen::ManGenTimer);
        connect(&m_timer_gen, &TimerGen::sig_genTimer_ms, &m_channel_work, &TotalChannelWork::GenFrameData);
        connect(&m_channel_work, &TotalChannelWork::sig_GenFrameData, &m_communicate, &Communicate::OnGenData);
        m_thread_timer.start();
    }

    {
        ui->m_pushbutton_mark->setVisible(false);
        ui->m_pushButton_open_siggen->setText("Closed");
        ui->m_pushButton_open_siggen->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui->m_pushbutton_send_frame->setVisible(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    m_thread_channel.quit();
    m_thread_channel.wait();

    m_thread_communicate.quit();
    m_thread_communicate.wait();

    m_thread_timer.quit();
    m_thread_timer.wait();

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

bool MainWindow::SaveChSettingDataToFile(QString file_path)
{
    QJsonArray _all_channel_data = m_channel_work.GenAllChJsonData();
    if(_all_channel_data.isEmpty())
    {
        return false;
    }
    QFile _saveFile(file_path);
    if(_saveFile.open(QIODevice::WriteOnly))
    {
        QJsonDocument _data_json_doc;
        _data_json_doc.setArray(_all_channel_data);
        _saveFile.write(_data_json_doc.toJson(QJsonDocument::Indented));
        _saveFile.close();
        return true;
    }
    return false;
}

void MainWindow::ClearAllChannel()
{
    int _current_ch_num =  ui->m_verlayout_ch->count() - 2;

    //更新导联名称
    for(int i = _current_ch_num - 1; i >= 0; i--)
    {
        emit sig_DelChannel(i);
        ui->m_verlayout_ch->itemAt(i)->widget()->deleteLater();
    }
    ui->m_lineedit_ch_num->setText("0");
}


void MainWindow::on_m_pushbutton_add_ch_clicked()
{
    emit sig_AddNewChannel(-1);
}


void MainWindow::on_m_setting_server_action_triggered()
{
    m_dlg_server_setting.show();
}


void MainWindow::on_m_pushButton_open_siggen_clicked()
{
    if(ui->m_pushButton_open_siggen->text() == "Closed")
    {
        emit sig_OpenTCPServer();
    }
    else if(ui->m_pushButton_open_siggen->text() == "Open")
    {
        emit sig_CloseTCPServer();
    }
}

void MainWindow::TCPServerStatusChanged(bool enable)
{
    if(enable)
    {
        ui->m_pushButton_open_siggen->setText("Open");
        ui->m_pushButton_open_siggen->setStyleSheet("background-color: rgb(85, 255, 0);");
        ui->m_comboBox_freq_select->setEnabled(false);
        ui->m_pushButton_setting_init->setEnabled(false);

        for(int i = 0; i < ui->m_verlayout_ch->count(); i++)
        {
            QWidget* _tmp_widget = ui->m_verlayout_ch->itemAt(i)->widget();
            if(_tmp_widget != nullptr)
            {
                _tmp_widget->setEnabled(false);
            }
        }
    }
    else
    {
        ui->m_pushButton_open_siggen->setText("Closed");
        ui->m_pushButton_open_siggen->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui->m_comboBox_freq_select->setEnabled(true);
        ui->m_pushButton_setting_init->setEnabled(true);

        for(int i = 0; i < ui->m_verlayout_ch->count(); i++)
        {
            QWidget* _tmp_widget = ui->m_verlayout_ch->itemAt(i)->widget();
            if(_tmp_widget != nullptr)
            {
                _tmp_widget->setEnabled(true);
            }
        }
    }
}


void MainWindow::on_m_comboBox_freq_select_currentTextChanged(const QString& arg1)
{
    if(arg1 == "0")
    {
        emit sig_SetTimerInterval(0);
        ui->m_pushbutton_send_frame->setVisible(true);
    }
    else
    {
        emit sig_SetTimerInterval(1000 / arg1.toInt());
        ui->m_pushbutton_send_frame->setVisible(false);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    emit sig_destroy();
}


void MainWindow::on_m_file_save_as_action_triggered()
{
    QString _fileName = QFileDialog::getSaveFileName(this, "打开文件", "", "信号发生器设置文件(*.json)");
    if(!_fileName.isEmpty())
    {
        if(SaveChSettingDataToFile(_fileName))
        {
            m_file_setting_path = _fileName;
        }
    }
}


void MainWindow::on_m_file_save_action_triggered()
{
    if(m_file_setting_path.isEmpty())
    {
        //没有读取文件，另存为
        on_m_file_save_as_action_triggered();
    }
    else
    {
        SaveChSettingDataToFile(m_file_setting_path);
    }
}


void MainWindow::on_m_file_open_action_triggered()
{
    QString _fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "信号发生器设置文件(*.json)");
    if(_fileName.isEmpty())
    {
        return;
    }

    QFile _loadFile(_fileName);
    if (_loadFile.open(QIODevice::ReadOnly))
    {
        QByteArray _allData = _loadFile.readAll();
        _loadFile.close();

        QJsonParseError _jsonError;
        QJsonDocument _data_doc = QJsonDocument::fromJson(_allData, &_jsonError);
        if(!_data_doc.isNull() && (_jsonError.error == QJsonParseError::NoError))
        {
            ClearAllChannel();
            m_file_setting_path = _fileName;
            QJsonArray _ch_data_array = _data_doc.array();
            for(auto iter : _ch_data_array)
            {
                emit sig_AddNewChannelFromJson(iter.toObject());
            }
        }
    }
}


void MainWindow::on_m_file_eeg_open_triggered()
{
    QString _fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "真实脑电文件(*.bin)");

    if (!_fileName.isNull())
    {
        QFile loadFile(_fileName);
        if (loadFile.open(QIODevice::ReadOnly))
        {
            ClearAllChannel();

            ui->m_file_open_action->setEnabled(false);
            ui->m_file_save_action->setEnabled(false);
            ui->m_file_save_as_action->setEnabled(false);
            ui->m_pushbutton_add_ch->setVisible(false);
            ui->m_comboBox_freq_select->setEnabled(false);

            QByteArray total_file_data = loadFile.readAll();
            loadFile.close();

            QByteArray _ch_num_byte = total_file_data.mid(0, 4);
            total_file_data.remove(0, 4);
            QByteArray _srate_byte = total_file_data.mid(0, 4);
            total_file_data.remove(0, 4);
            QByteArray _length_byte = total_file_data.mid(0, 4);
            total_file_data.remove(0, 4);

            quint32 _ch_num;
            memcpy(&_ch_num, _ch_num_byte.data(), sizeof(quint32));
            quint32 _srate;
            memcpy(&_srate, _srate_byte.data(), sizeof(quint32));
            ui->m_comboBox_freq_select->setCurrentText(QString::number(_srate));
            quint32 _length;
            memcpy(&_length, _length_byte.data(), sizeof(quint32));

            QStringList _ch_name_list;
            for(int i = 0; i < _ch_num; i++)
            {
                quint8 _tmp_ch_name_length = total_file_data.mid(0, 1).toHex().toUInt();
                total_file_data.remove(0, 1);
                QString _tmp_ch_name = total_file_data.mid(0, _tmp_ch_name_length);
                total_file_data.remove(0, _tmp_ch_name_length);
                _ch_name_list.push_back(_tmp_ch_name);
            }

            for(int i = 0; i < _ch_num; i++)
            {
                QList<float> data;
                data.resize(_length);
                memcpy(data.data(), total_file_data.data(), sizeof(float) *_length );
                total_file_data.remove(0, sizeof(float) *_length);
                emit sig_AddNewChannelFromRealEEG(_ch_name_list.at(i), data);
            }
        }
    }
}


void MainWindow::on_m_pushButton_setting_init_clicked()
{
    ClearAllChannel();
    ui->m_file_open_action->setEnabled(true);
    ui->m_file_save_action->setEnabled(true);
    ui->m_file_save_as_action->setEnabled(true);
    ui->m_pushbutton_add_ch->setVisible(true);
    ui->m_comboBox_freq_select->setEnabled(true);
}

