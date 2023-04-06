#include "channelsetting.h"
#include "ui_channelsetting.h"

ChannelSetting::ChannelSetting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ChannelSetting)
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
            // 设置图表主题色
            //            ui->graphicsView->chart()->setTheme(QChart::ChartTheme(0));
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
            m_axisX->setRange(0, qreal(CHART_POINT) / 1000);
            m_chart->addAxis(m_axisX, Qt::AlignBottom);
            m_line_series->attachAxis(m_axisX);
        }
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
    this->show();
    {
        //初始化导联绘图
        if(m_line_series != nullptr)
        {
            m_line_series->clear();
            qreal _max, _min;
            for(qint64 tm_ms = 0; tm_ms < CHART_POINT; tm_ms++)
            {
                qreal _x = qreal(tm_ms) / 1000;
                qreal _data = channel->GenData(_x);

                if(tm_ms == 0)
                {
                    _max = _min = _data;
                }
                else
                {
                    if(_data > _max)
                    {
                        _max = _data;
                    }
                    if(_data < _min)
                    {
                        _min = _data;
                    }
                }

                m_line_series->append(_x, _data);
            }

            float axix_delta = 1;
            if(_max != _min)
            {
                axix_delta = (_max - _min) * 0.1;
            }
            m_axisY->setMax(_max + axix_delta);
            m_axisY->setMin(_min - axix_delta);
        }
    }

    ui->m_lineedit_ch_name->setText(channel->GetChData().m_ch_name);

    this->exec();

    emit sig_channel_data_changed();

    channel->GetChData().m_ch_name = ui->m_lineedit_ch_name->text();


//    qDebug() << channel_widget->GetChannelData().m_ch_name;

}

