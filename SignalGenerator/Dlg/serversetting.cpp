#include "serversetting.h"
#include "ui_serversetting.h"

ServerSetting::ServerSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSetting)
{
    ui->setupUi(this);

    //tcp设置
    ui->m_combobox_server_ip->setEditable(true);
    //获取本机ip地址
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for(const QHostAddress &ipaddress : ipAddressesList)
    {
        if(ipaddress.toIPv4Address())
        {
            ui->m_combobox_server_ip->addItem(ipaddress.toString());
        }
    }
    //限制端口号输入
    ui->m_lineedit_server_port->setValidator(new QIntValidator(ui->m_lineedit_server_port));

    // 监听
    connect(&m_tcpserver, &QTcpServer::newConnection, this, [ = ]()
    {
        QTcpSocket *client_socket = m_tcpserver.nextPendingConnection();
        m_tcp_client_socket_list.append(client_socket);

        //Disconnect
        connect(client_socket, &QTcpSocket::disconnected, this, [ = ]()
        {
            m_tcp_client_socket_list.removeOne(client_socket);
            client_socket->deleteLater();
        });

//        //Debug
//        connect(client_socket, &QTcpSocket::readyRead, this, [ = ]()
//        {
//            const QString recv_text = QString::fromUtf8(client_socket->readAll());
//            qDebug() << recv_text;
//        });

    });

    //串口设置
//    ui->m_combobox_com_name->setEditable(true);
//    ui->m_combobox_baud->setEditable(true);
//    ui->m_combobox_com_parity->setEditable(true);
//    ui->m_combobox_databits->setEditable(true);
//    ui->m_combobox_stopbits->setEditable(true);

    //可用串口号检测
    const auto availablePorts_infos = QSerialPortInfo::availablePorts();
    for(const auto &availablePorts_info : availablePorts_infos)
    {
        ui->m_combobox_com_name->addItem(availablePorts_info.portName());
    }

    if(ui->m_combobox_com_name->count() == 0)
    {
        ui->m_pushbutton_com_on->setDisabled(true);
        ui->m_label_com_warning->setText("当前无可用串口");
    }
    else
    {
        ui->m_label_com_warning->hide();
    }

}

ServerSetting::~ServerSetting()
{
    CloseTCPServer();
    CloseSerialPort();
    delete ui;
}

void ServerSetting::CloseTCPServer()
{
    for(auto socket : m_tcp_client_socket_list)
    {
        //断开与客户端连接
        if(socket != nullptr)
        {
            socket->disconnectFromHost();
            if(socket->state() != QAbstractSocket::UnconnectedState)
            {
                socket->abort();
            }
            delete socket;
            socket = nullptr;
        }
    }
    m_tcp_client_socket_list.clear();

    m_tcpserver.close();

    ui->m_pushbutton_tcp_on->setText("关闭");
    ui->m_combobox_server_ip->setDisabled(false);
    ui->m_lineedit_server_port->setDisabled(false);
}

void ServerSetting::OpenTCPServer()
{
    if(!m_tcpserver.isListening())
    {

        QString server_ip = ui->m_combobox_server_ip->currentText();
        quint16 server_port = ui->m_lineedit_server_port->text().toUInt();
        QHostAddress server_address = server_ip == "0.0.0.0" ? QHostAddress::AnyIPv4 : QHostAddress(server_ip);
        m_tcpserver.listen(server_address, server_port);

        ui->m_pushbutton_tcp_on->setText("打开");
        ui->m_combobox_server_ip->setDisabled(true);
        ui->m_lineedit_server_port->setDisabled(true);
    }
}

void ServerSetting::CloseSerialPort()
{
    if(m_serialPort.isOpen())
    {
        m_serialPort.clear();
        m_serialPort.close();

        ui->m_combobox_baud->setDisabled(false);
        ui->m_combobox_com_name->setDisabled(false);
        ui->m_combobox_com_parity->setDisabled(false);
        ui->m_combobox_databits->setDisabled(false);
        ui->m_combobox_stopbits->setDisabled(false);
        ui->m_combobox_flowcontrol->setDisabled(false);

        ui->m_pushbutton_com_on->setText("关闭");
    }
}

void ServerSetting::OpenSerialPort()
{
    if(!m_serialPort.isOpen())
    {
        switch(ui->m_combobox_baud->currentText().toInt())
        {
            case 1200:
                m_serialPort.setBaudRate(QSerialPort::Baud1200);
                break;
            case 2400:
                m_serialPort.setBaudRate(QSerialPort::Baud2400);
                break;
            case 4800:
                m_serialPort.setBaudRate(QSerialPort::Baud4800);
                break;
            case 9600:
                m_serialPort.setBaudRate(QSerialPort::Baud9600);
                break;
            case 19200:
                m_serialPort.setBaudRate(QSerialPort::Baud19200);
                break;
            case 38400:
                m_serialPort.setBaudRate(QSerialPort::Baud38400);
                break;
            case 57600:
                m_serialPort.setBaudRate(QSerialPort::Baud57600);
                break;
            case 1115200:
                m_serialPort.setBaudRate(QSerialPort::Baud115200);
                break;
            default:
                m_serialPort.setBaudRate(QSerialPort::Baud115200);
                break;
        }

        switch(ui->m_combobox_databits->currentText().toInt())
        {
            case 5:
                m_serialPort.setDataBits(QSerialPort::Data5);
                break;
            case 6:
                m_serialPort.setDataBits(QSerialPort::Data6);
                break;
            case 7:
                m_serialPort.setDataBits(QSerialPort::Data7);
                break;
            case 8:
                m_serialPort.setDataBits(QSerialPort::Data8);
                break;
            default:
                m_serialPort.setDataBits(QSerialPort::Data8);
                break;
        }

        QString parity = ui->m_combobox_com_parity->currentText();
        if(parity == "No")
        {
            m_serialPort.setParity(QSerialPort::NoParity);
        }
        else if(parity == "Even")
        {
            m_serialPort.setParity(QSerialPort::EvenParity);
        }
        else if(parity == "Odd")
        {
            m_serialPort.setParity(QSerialPort::OddParity);
        }
        else if(parity == "Mark")
        {
            m_serialPort.setParity(QSerialPort::MarkParity);
        }
        else if(parity == "Space")
        {
            m_serialPort.setParity(QSerialPort::SpaceParity);
        }
        else
        {
            m_serialPort.setParity(QSerialPort::NoParity);
        }

        switch(ui->m_combobox_stopbits->currentText().toInt())
        {
            case 1:
                m_serialPort.setStopBits(QSerialPort::OneStop);
                break;
            case 2:
                m_serialPort.setStopBits(QSerialPort::TwoStop);
                break;
            default:
                m_serialPort.setStopBits(QSerialPort::OneStop);
                break;
        }


        QString flowcontrol = ui->m_combobox_flowcontrol->currentText();
        if(flowcontrol == "No")
        {
            m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
        }
        else if(flowcontrol == "Hardware")
        {
            m_serialPort.setFlowControl(QSerialPort::HardwareControl);
        }
        else if(flowcontrol == "Software")
        {
            m_serialPort.setFlowControl(QSerialPort::SoftwareControl);
        }
        else
        {
            m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
        }

        m_serialPort.setPortName(ui->m_combobox_com_name->currentText());

        m_serialPort.open(QIODevice::ReadWrite);

        ui->m_combobox_baud->setDisabled(true);
        ui->m_combobox_com_name->setDisabled(true);
        ui->m_combobox_com_parity->setDisabled(true);
        ui->m_combobox_databits->setDisabled(true);
        ui->m_combobox_stopbits->setDisabled(true);
        ui->m_combobox_flowcontrol->setDisabled(true);

        ui->m_pushbutton_com_on->setText("打开");
    }
}

void ServerSetting::TcpSend(QByteArray senddata)
{
    for(QTcpSocket *socket : m_tcp_client_socket_list)
    {
        if(socket != nullptr)
        {
            socket->write(senddata);
        }
    }
}

void ServerSetting::SerialPortSend(QByteArray senddata)
{
    if(m_serialPort.isOpen())
    {
        m_serialPort.write(senddata);
    }
}

bool ServerSetting::IsServerOn()
{
    return m_tcp_client_socket_list.size() > 0 || m_serialPort.isOpen();
}

void ServerSetting::on_m_pushbutton_tcp_on_toggled(bool checked)
{
    if(checked)
    {
        OpenTCPServer();
    }
    else
    {
        CloseTCPServer();
    }
    emit ServerStateChanged();
}


void ServerSetting::on_m_pushbutton_com_on_toggled(bool checked)
{
    if(checked)
    {
        OpenSerialPort();
    }
    else
    {
        CloseSerialPort();
    }
    emit ServerStateChanged();
}

