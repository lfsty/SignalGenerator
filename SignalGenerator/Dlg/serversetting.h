#ifndef SERVERSETTING_H
#define SERVERSETTING_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QString>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
namespace Ui
{
    class ServerSetting;
}

class ServerSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ServerSetting(QWidget *parent = nullptr);
    ~ServerSetting();

private slots:
    void on_m_pushbutton_tcp_on_toggled(bool checked);

    void on_m_pushbutton_com_on_toggled(bool checked);

private:
    Ui::ServerSetting *ui;
    //  通信接口
    QTcpServer m_tcpserver;
    QList<QTcpSocket *> m_tcp_client_socket_list;
    QSerialPort m_serialPort;
private:
    QByteArray m_com_protocol;
private:
    void CloseTCPServer();
    void OpenTCPServer();
    void CloseSerialPort();
    void OpenSerialPort();
public:
    bool TcpSend(QByteArray senddata);
    bool SerialPortSend(QByteArray senddata);
    bool IsServerOn();
private:
    void showEvent(QShowEvent *event);

signals:
    void ServerStateChanged();
};

#endif // SERVERSETTING_H
