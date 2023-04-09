#ifndef SERVERSETTING_H
#define SERVERSETTING_H

#include <QDialog>
#include <QHostAddress>
#include <QString>
#include <QNetworkInterface>
namespace Ui
{
    class ServerSetting;
}

class ServerSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ServerSetting(QWidget* parent = nullptr);
    ~ServerSetting();

private:
    Ui::ServerSetting* ui;
public slots:
    void OpenTCPServer();
    void TCPServerStatusChanged(bool enable);

signals:
    void sig_open_TCPServer(const QString& ip_address, const quint16& port);
};

#endif // SERVERSETTING_H
