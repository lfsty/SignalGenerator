#include "channelsetting.h"
#include "ui_channelsetting.h"
#include <QIcon>
#include <QPixmap>
ChannelSetting::ChannelSetting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ChannelSetting),
    m_dlg_sigsetting(this)
{
    ui->setupUi(this);

    {
        // chart初始化
        if(m_chart == nullptr)
        {
            m_chart = new QChart();
            // 将Chart添加到ChartView
            ui->m_chartview->setChart(m_chart);
            ui->m_chartview->setRenderHint(QPainter::Antialiasing);
            m_chart->legend()->setVisible(false);
        }
        if(m_line_series == nullptr)
        {
            m_line_series = new QLineSeries(m_chart);
            m_chart->addSeries(m_line_series);
        }
        if(m_axisY == nullptr)
        {
            m_axisY = new QValueAxis(m_chart);
            m_chart->addAxis(m_axisY, Qt::AlignLeft);
            m_line_series->attachAxis(m_axisY);
        }
        if(m_axisX == nullptr)
        {
            m_axisX = new QValueAxis(m_chart);
            m_axisX->setRange(0, qreal(EXAMPLE_DATA_NUM) / 1000);
            m_chart->addAxis(m_axisX, Qt::AlignBottom);
            m_line_series->attachAxis(m_axisX);
        }
    }
    {
        // sigsetting初始化
        m_dlg_sigsetting.setModal(true);
    }
    {
        // tablewidget初始化
        ui->m_tableWidget_sig->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        ui->m_tableWidget_sig->setColumnWidth(1, 20);
        ui->m_tableWidget_sig->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

ChannelSetting::~ChannelSetting()
{
    delete ui;

    if(m_chart != nullptr)
    {
        m_chart->deleteLater();
    }

    if(m_line_series != nullptr)
    {
        m_line_series->deleteLater();
    }
    if(m_axisY != nullptr)
    {
        m_axisY->deleteLater();
    }
    if(m_axisX != nullptr)
    {
        m_axisX->deleteLater();
    }
}

void ChannelSetting::OnSettingChannel(Channel* channel)
{
    assert(channel != nullptr);
    connect(channel, &Channel::sig_example_data_changed, this, &ChannelSetting::UpdateChartData);
    m_current_channel = channel;

    this->show();
    UpdateSigsDesp();
    UpdateChartData(channel->GetExampleData());

    ui->m_lineedit_ch_name->setText(channel->GetChData().m_ch_name);

    this->exec();

    emit sig_channel_data_changed();

    channel->GetChData().m_ch_name = ui->m_lineedit_ch_name->text();

    disconnect(channel, &Channel::sig_example_data_changed, this, &ChannelSetting::UpdateChartData);

}

void ChannelSetting::UpdateChartData(const QList<QPointF>& example_data)
{
    //初始化导联绘图
    if(m_line_series != nullptr)
    {
        m_line_series->clear();
        m_line_series->replace(example_data);

        qreal _max, _min;
        _max = _min = example_data[0].y();
        for(QPointF p : example_data)
        {
            qreal _data = p.y();
            if(_data > _max)
            {
                _max = _data;
            }
            if(_data < _min)
            {
                _min = _data;
            }
        }

        float axix_delta = 1;
        if(_max != _min)
        {
            axix_delta = (_max - _min) * 0.125;
        }
        m_axisY->setMax(_max + axix_delta);
        m_axisY->setMin(_min - axix_delta);
    }

    // 更新导联描述信息
    UpdateSigsDesp();
}

void ChannelSetting::UpdateSigsDesp()
{
    ui->m_tableWidget_sig->setRowCount(0);
    ui->m_tableWidget_sig->clearContents();

    QList<SigDef*> _sig_list = m_current_channel->GetSigList();
    for(SigDef* _sig : _sig_list)
    {
        int _current_line = ui->m_tableWidget_sig->rowCount();
        ui->m_tableWidget_sig->setRowCount(_current_line + 1);

        ui->m_tableWidget_sig->setItem(_current_line, 0, new QTableWidgetItem(_sig->GetDescription()));
        ui->m_tableWidget_sig->setItem(_current_line, 1, new QTableWidgetItem(QIcon(QPixmap(":/image/bin.png")), ""));
    }
}

void ChannelSetting::on_m_pushButton_add_sig_clicked()
{
    SigDef* _new_sig = m_dlg_sigsetting.OnSettingSig();
    _new_sig->moveToThread(m_current_channel->thread());
    m_current_channel->AddSig(_new_sig);
}


void ChannelSetting::on_m_tableWidget_sig_itemDoubleClicked(QTableWidgetItem* item)
{
    if(ui->m_tableWidget_sig->indexFromItem(item).column() != 0)
    {
        return;
    }
    int _sig_index = ui->m_tableWidget_sig->indexFromItem(item).row();
    SigDef* _sig_choosed = m_current_channel->GetSigList().at(_sig_index);
    if(_sig_choosed->GetSigType() == SigType::Real)
    {
        return;
    }

    SigDef* _new_sig = m_dlg_sigsetting.OnSettingSig(_sig_choosed);
    _new_sig->moveToThread(m_current_channel->thread());
    m_current_channel->SetSig(_sig_index, _new_sig);
}


void ChannelSetting::on_m_tableWidget_sig_itemClicked(QTableWidgetItem* item)
{
    if(ui->m_tableWidget_sig->indexFromItem(item).column() != 1)
    {
        return;
    }
    int _sig_index = ui->m_tableWidget_sig->indexFromItem(item).row();
    m_current_channel->DelSig(_sig_index);
}

