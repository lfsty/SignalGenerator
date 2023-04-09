
#ifndef COMMUNICATE_H
#define COMMUNICATE_H


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

class Communicate : public QObject
{
    Q_OBJECT
public:
    explicit Communicate(QObject* parent = nullptr);
    ~Communicate();
private:
    //  通信接口
    QTcpServer m_tcpserver;
    QList<QTcpSocket*> m_tcp_client_socket_list;
    void TcpSend(const QByteArray& senddata);
public:
    void MoveToThread(QThread* thread);
public slots:
    void CloseTCPServer();
    void OpenTCPServer(const QString& ip_address, const quint16& port);
    void OnGenData(const QList<float>& framedata);
signals:
    void sig_TCPServer_status_changed(bool enable);
    void sig_TCP_frame_send();
};

#endif // COMMUNICATE_H
