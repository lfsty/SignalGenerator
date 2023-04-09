
#ifndef TIMERGEN_H
#define TIMERGEN_H


#include <QObject>
#include <QTimer>
#define MAN_SENDDATA_INTERVAL_MS 50

class TimerGen : public QObject
{
    Q_OBJECT
public:
    explicit TimerGen(QObject* parent = nullptr);
    void MoveToThread(QThread* thread);
    void ManGenTimer();
    void Destroy();
private:
    QTimer m_timer_gen;
    quint64 m_current_time_ms = 0;

    //帧数统计
    QTimer m_timer_frame_count;
    quint64 m_frame_count_s = 0;
public slots:
    void SetTimerInterval(const int& interval_ms);
    void SetTimerEnable(const bool& enable);
    void FrameSended();
signals:
    void sig_genTimer_ms(const quint64& timer_ms);
    void sig_frame_count_s(const int frame_count_s);
};

#endif // TIMERGEN_H
