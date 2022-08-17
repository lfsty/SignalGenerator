#include "signalgenerator.h"
#include "ui_signalgenerator.h"

SignalGenerator::SignalGenerator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SignalGenerator)
{
    ui->setupUi(this);

    //新建服务器设置对话框
    if(m_serversetting_dialog == nullptr)
    {
        m_serversetting_dialog = new ServerSetting(this);
        m_serversetting_dialog->setModal(true);
    }

    //放大器设置对话框
    if(m_ampsetting_dialog == nullptr)
    {
        m_ampsetting_dialog = new AmpSetting(this);
        m_ampsetting_dialog->setModal(true);
    }

    m_timer_gendata.setInterval(1000 / ui->m_lineedit_srate->text().toInt());
    m_timer_gendata.setTimerType(Qt::PreciseTimer);
    connect(&m_timer_gendata, &QTimer::timeout, this, &SignalGenerator::on_m_timer_gendata_timeout);
    connect(m_serversetting_dialog, &ServerSetting::ServerStateChanged, this, [ = ]()
    {
        if(m_serversetting_dialog->IsServerOn() && !m_timer_gendata.isActive())
        {
            if(ui->m_lineedit_srate->text().toInt() != 0)
            {
                m_timer_gendata.start();
            }
            ui->scrollArea->setDisabled(true);
            m_ampsetting_dialog->setDisabled(true);
        }
        else if(!m_serversetting_dialog->IsServerOn() )
        {
            if(m_timer_gendata.isActive())
            {
                m_timer_gendata.stop();
            }
            ui->scrollArea->setDisabled(false);
            m_ampsetting_dialog->setDisabled(false);
        }
    });


    m_timer_update_serverdata_s.setInterval(1000);
    m_timer_update_serverdata_s.setTimerType(Qt::PreciseTimer);
    connect(&m_timer_update_serverdata_s, &QTimer::timeout, this, [ = ]()
    {
//        this->setWindowTitle("当前每秒发送帧数:" + QString::number(m_send_frame_s));
        ui->m_lcd_send_frame_num->display(m_send_frame_s);
//        QPalette lcdpat = ui->m_lcd_send_frame_num->palette();
//        if(m_send_frame_s >=  ui->m_lineedit_srate->text().toInt())
//        {
//            lcdpat.setColor(QPalette::Normal, QPalette::WindowText, Qt::green);
//        }
//        else
//        {
//            lcdpat.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
//        }
//        ui->m_lcd_send_frame_num->setPalette(lcdpat);
        m_send_frame_s = 0;
    });
    m_timer_update_serverdata_s.start();

    //添加样式文件
    QFile qssFile(":/style/default.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        this->setStyleSheet(QLatin1String(qssFile.readAll()));
        qssFile.close();
    }

    ui->m_pushbutton_send_frame->setVisible(false);

}

SignalGenerator::~SignalGenerator()
{
    //删除服务器设置对话框
    if(m_serversetting_dialog != nullptr)
    {
        m_serversetting_dialog->deleteLater();
    }
    //删除放大器设置对话框
    if(m_ampsetting_dialog != nullptr)
    {
        m_ampsetting_dialog->deleteLater();
    }

//    while(QLayoutItem *item = ui->m_verlayout_ch->takeAt(0))
//    {
//        if (QWidget *widget = item->widget())
//        {
//            widget->deleteLater();
//        }
//    }

    delete ui;
}

// 服务器设置点击
void SignalGenerator::on_m_setting_server_action_triggered()
{
    m_serversetting_dialog->show();
    m_serversetting_dialog->exec();
}


void SignalGenerator::on_m_setting_generator_action_triggered()
{
    m_ampsetting_dialog->SetSrate(ui->m_lineedit_srate->text().toInt());
    m_ampsetting_dialog->show();
    if(m_ampsetting_dialog->exec())
    {
        QString srate_str = m_ampsetting_dialog->GetSrate();
        ui->m_lineedit_srate->setText(srate_str);
        if(srate_str.toInt() != 0)
        {
            m_timer_gendata.setInterval(1000 / m_ampsetting_dialog->GetSrate().toInt());
            ui->m_pushbutton_send_frame->setVisible(false);
        }
        else
        {
            m_timer_gendata.setInterval(0);
            ui->m_pushbutton_send_frame->setVisible(true);
        }
    }
}


void SignalGenerator::on_m_pushbutton_add_ch_clicked()
{
    //导联默认名
    QString default_ch_name = "ch" + QString::number(ui->m_lineedit_ch_num->text().toInt() + 1);

    ChannelWidget *channel_widget = new ChannelWidget(this, default_ch_name);

    AddChnnel(channel_widget);
}

void SignalGenerator::on_m_timer_gendata_timeout()
{
    static qint64 time_ms = 0;
    if(m_serversetting_dialog != nullptr && m_serversetting_dialog->IsServerOn())
    {
        QByteArray data;
        QByteArray tmp;
        tmp.resize(sizeof(float));
        for(int i = 0; i < ui->m_verlayout_ch->count(); i++)
        {
            ChannelWidget *chan_widget = qobject_cast<ChannelWidget *>(ui->m_verlayout_ch->itemAt(i)->widget());
            if(chan_widget != 0)
            {
                float ch_data = chan_widget->GenData(time_ms);
                memcpy(tmp.data(), &ch_data, sizeof(ch_data));
                data.push_back(tmp);
            }
        }

        if(m_serversetting_dialog->TcpSend(data) || m_serversetting_dialog->SerialPortSend(data))
        {
            m_send_frame_s++;
        }
        time_ms++;

    }
}

void SignalGenerator::on_m_copy_channel_clicked(Channel *pChannel)
{
    ChannelWidget *new_channelwidget = new ChannelWidget(this, pChannel);
    AddChnnel(new_channelwidget);
}

void SignalGenerator::on_m_del_channel_clicked(ChannelWidget *pChannelWidget)
{
    m_channel_widget_list.removeOne(pChannelWidget);
    UpDateChannelNum();
}

void SignalGenerator::UpDateChannelNum()
{
//    int count = 0;
//    for(int i = 0; i < ui->m_verlayout_ch->count(); i++)
//    {
//        ChannelWidget *chan_widget = qobject_cast<ChannelWidget *>(ui->m_verlayout_ch->itemAt(i)->widget());
//        if(chan_widget != 0)
//        {
//            count++;
//        }
//    }

    ui->m_lineedit_ch_num->setText(QString::number(m_channel_widget_list.count()));

}

void SignalGenerator::AddChnnel(ChannelWidget *new_ch_widget)
{
    ui->m_verlayout_ch->insertWidget(ui->m_verlayout_ch->indexOf(ui->m_pushbutton_add_ch), new_ch_widget);
    //复制导联
    connect(new_ch_widget, &ChannelWidget::CopyChannel, this, &SignalGenerator::on_m_copy_channel_clicked);

    //导联删除时更新主界面导联数
    connect(new_ch_widget, &ChannelWidget::ChannelDelete, this, &SignalGenerator::on_m_del_channel_clicked);

    m_channel_widget_list.push_back(new_ch_widget);

    UpDateChannelNum();
}

QByteArray SignalGenerator::GenSettingDataByteArray()
{
    QJsonObject data_json_obj;
    QJsonDocument data_json_doc;
    QByteArray data_json_byte;

    QJsonArray ch_json_array;
    for(int i = 0; i < ui->m_verlayout_ch->count(); i++)
    {
        ChannelWidget *chan_widget = qobject_cast<ChannelWidget *>(ui->m_verlayout_ch->itemAt(i)->widget());
        if(chan_widget != 0)
        {
            ch_json_array.push_back(QJsonValue(chan_widget->GenJsonObject()));
        }
    }
    data_json_obj["ChData"] = ch_json_array;

    data_json_doc.setObject(data_json_obj);
    data_json_byte = data_json_doc.toJson(QJsonDocument::Indented);

    return data_json_byte;
}

void SignalGenerator::ClearAllChannelWidget()
{
    for(int i = 0; i < ui->m_verlayout_ch->count(); i++)
    {
        ChannelWidget *chan_widget = qobject_cast<ChannelWidget *>(ui->m_verlayout_ch->itemAt(i)->widget());
        if(chan_widget != 0)
        {
            chan_widget->on_m_pushButton_delete_clicked();
        }
    }
}

void SignalGenerator::on_m_file_open_action_triggered()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "信号发生器设置文件(*.json)");

    if (!fileName.isNull())
    {
        QFile loadFile(fileName);
        if (loadFile.open(QIODevice::ReadOnly))
        {
            QByteArray allData = loadFile.readAll();
            loadFile.close();

            QJsonParseError jsonError;
            QJsonDocument data_doc = QJsonDocument::fromJson(allData, &jsonError);
            if(!data_doc.isNull() && (jsonError.error == QJsonParseError::NoError))
            {
                ClearAllChannelWidget();
                m_save_file_path = fileName;

                QJsonObject data_root_obj = data_doc.object();
                QJsonArray ch_data_array = data_root_obj["ChData"].toArray();
                for(auto iter : ch_data_array)
                {
                    QJsonObject single_ch_obj = iter.toObject();
                    ChannelWidget *channel_widget = new ChannelWidget(this);
                    channel_widget->ParseJsonObject(single_ch_obj);
                    AddChnnel(channel_widget);
                }
            }
        }
    }
}


void SignalGenerator::on_m_file_save_action_triggered()
{


    if(m_save_file_path.isEmpty())
    {
        on_m_file_save_as_action_triggered();
    }
    else
    {
        bool save_file_result = false;
        QFile loadFile(m_save_file_path);
        if (loadFile.open(QIODevice::WriteOnly))
        {
            loadFile.write(GenSettingDataByteArray());
            loadFile.close();
            save_file_result = true;
        }

        if(save_file_result)
        {
            QMessageBox::information(this, "保存结果", "保存成功~！");
        }
        else
        {
            QMessageBox::information(this, "保存结果", "保存失败QAQ");
        }
    }
}


void SignalGenerator::on_m_file_save_as_action_triggered()
{
    //保存文件
    QString fileName = QFileDialog::getSaveFileName(this, "选择保存地址", "", "信号发生器设置文件(*.json)");
    bool save_file_result = false;
    if (!fileName.isNull())
    {
        QFile loadFile(fileName);
        if (loadFile.open(QIODevice::WriteOnly))
        {
            loadFile.write(GenSettingDataByteArray());
            loadFile.close();
            save_file_result = true;
            m_save_file_path = fileName;
        }
    }
    if(save_file_result)
    {
        QMessageBox::information(this, "保存结果", "保存成功~！");
    }
    else
    {
        QMessageBox::information(this, "保存结果", "保存失败QAQ");
    }
}


void SignalGenerator::on_m_pushbutton_send_frame_clicked()
{
    on_m_timer_gendata_timeout();
}


void SignalGenerator::on_m_pushbutton_mark_clicked()
{
    qDebug() << "mark";
}

void SignalGenerator::keyPressEvent(QKeyEvent *e)
{
    qDebug() << e->key();
    qDebug() << e->text();
}

