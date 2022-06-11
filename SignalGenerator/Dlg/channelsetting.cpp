#include "channelsetting.h"
#include "ui_channelsetting.h"

ChannelSetting::ChannelSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChannelSetting)
{
    ui->setupUi(this);
    ui->m_tableWidget_sig->setColumnWidth(0, 50);
    ui->m_tableWidget_sig->horizontalHeader()->setStretchLastSection(true);

    init_chart();
    //更新绘图
    on_update_data_chart();

}

ChannelSetting::ChannelSetting(QWidget *parent, ChannelSetting *from_copy_widget):
    QDialog(parent),
    ui(new Ui::ChannelSetting)
{
    ui->setupUi(this);
    ui->m_tableWidget_sig->setColumnWidth(0, 50);
    ui->m_tableWidget_sig->horizontalHeader()->setStretchLastSection(true);

    ui->m_lineedit_ch_name->setText(from_copy_widget->GetChName());
    m_list_sig = from_copy_widget->GetSigListCopy();

    //跟新table_widget
    for(auto sig : m_list_sig)
    {
        int current_line = ui->m_tableWidget_sig->rowCount();
        ui->m_tableWidget_sig->setRowCount(current_line + 1);

        ui->m_tableWidget_sig->setItem(current_line, 0, new QTableWidgetItem(sig->GetTypeStr()));
        ui->m_tableWidget_sig->setItem(current_line, 1, new QTableWidgetItem(sig->GetDescription()));
    }

    init_chart();

    //更新绘图
    on_update_data_chart();

}

ChannelSetting::~ChannelSetting()
{
    delete ui;
    foreach(auto sig, m_list_sig)
    {
        m_list_sig.removeOne(sig);
        delete sig;
        sig = nullptr;
    }

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

QString ChannelSetting::GetChName()
{
    return ui->m_lineedit_ch_name->text();
}

void ChannelSetting::SetChName(QString name)
{
    ui->m_lineedit_ch_name->setText(name);
}

float ChannelSetting::GenData(quint64 tm_ms)
{
    float data = 0;
    for(auto sig : m_list_sig)
    {
        data += sig->GenData(tm_ms);
    }
    return data;
}

QList<SimSig::SigDef *> ChannelSetting::GetSigListCopy()
{
    QList<SimSig::SigDef *> SigList;
    for(auto sig : m_list_sig)
    {
        SigList.push_back(sig->GetCopy());
    }
    return SigList;
}

void ChannelSetting::init_chart()
{

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
        m_axisX->setRange(0, CHART_POINT);
        m_chart->addAxis(m_axisX, Qt::AlignBottom);
        m_line_series->attachAxis(m_axisX);
    }

}

void ChannelSetting::on_m_pushbutton_addsig_clicked()
{
    SigSetting w(this);
    w.setModal(true);
    w.show();
    if(w.exec())
    {
        SimSig::SigDef *sig = w.GetSig();
        if(sig != nullptr)
        {
            m_list_sig.push_back(sig);

            int current_line = ui->m_tableWidget_sig->rowCount();
            ui->m_tableWidget_sig->setRowCount(current_line + 1);

            ui->m_tableWidget_sig->setItem(current_line, 0, new QTableWidgetItem(sig->GetTypeStr()));
            ui->m_tableWidget_sig->setItem(current_line, 1, new QTableWidgetItem(sig->GetDescription()));
            on_update_data_chart();
        }
    }
}

void ChannelSetting::on_update_data_chart()
{
    if(m_line_series != nullptr)
    {
        m_line_series->clear();
        float max = 0, min = 0;
        for(qint64 tm_ms = 0; tm_ms < CHART_POINT; tm_ms++)
        {
            float data = GenData(tm_ms);
            if(data > max)
            {
                max = data;
            }
            if(data < min)
            {
                min = data;
            }
            m_line_series->append(tm_ms, data);
        }
        float axix_delta = (max - min) * 0.1;
        m_axisY->setMax(max + axix_delta);
        m_axisY->setMin(min - axix_delta);
    }
}
