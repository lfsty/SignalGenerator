#include "serversetting.h"
#include "ui_serversetting.h"

ServerSetting::ServerSetting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ServerSetting)
{
    ui->setupUi(this);

    {
        //tcp设置
        ui->m_combobox_server_ip->setEditable(true);
        //获取本机ip地址
        const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for(const QHostAddress& ipaddress : ipAddressesList)
        {
            if(ipaddress.toIPv4Address())
            {
                ui->m_combobox_server_ip->addItem(ipaddress.toString());
            }
        }
    }
    {
        //串口暂不使用
        ui->tabWidget->removeTab(1);
    }
}

ServerSetting::~ServerSetting()
{
    delete ui;
}

void ServerSetting::OpenTCPServer()
{
    QString _ip_address = ui->m_combobox_server_ip->currentText();
    quint16 _port = ui->m_spinBox_port->value();
    emit sig_open_TCPServer(_ip_address, _port);
}

void ServerSetting::TCPServerStatusChanged(bool enable)
{
    if(enable)
    {
        ui->m_combobox_server_ip->setEnabled(false);
        ui->m_spinBox_port->setEnabled(false);
    }
    else
    {
        ui->m_combobox_server_ip->setEnabled(true);
        ui->m_spinBox_port->setEnabled(true);
    }
}

