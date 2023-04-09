#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui
{
    class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelWidget(QWidget* parent = nullptr);
    ~ChannelWidget();
public:
    void SetChName(QString ch_name);

private slots:
    void on_m_pushButton_delete_clicked();

    void on_m_pushButton_copy_clicked();

    void on_m_pushButton_setting_clicked();

private:
    Ui::ChannelWidget* ui;

signals:
    void sig_copy(ChannelWidget*);
    void sig_open_settingDlg(ChannelWidget*);
    void sig_del_this(ChannelWidget*);

};

#endif // CHANNELWIDGET_H
