
#include "timergen.h"

TimerGen::TimerGen(QObject* parent)
    : QObject{parent}
{
    m_timer_gen.setTimerType(Qt::TimerType::PreciseTimer);
    connect(&m_timer_gen, &QTimer::timeout, this, [ = ]()
    {
        m_current_time_ms += m_timer_gen.interval();
        emit sig_genTimer_ms(m_current_time_ms);
    });

    m_timer_frame_count.setInterval(1000);
    m_timer_frame_count.start();
    connect(&m_timer_frame_count, &QTimer::timeout, this, [ = ]()
    {
        emit sig_frame_count_s(m_frame_count_s);
        m_frame_count_s = 0;
    });
}

void TimerGen::MoveToThread(QThread* thread)
{
    this->moveToThread(thread);
    m_timer_gen.moveToThread(thread);
    m_timer_frame_count.moveToThread(thread);
}

void TimerGen::ManGenTimer()
{
    static quint64 _timer_ms = 0;
    emit sig_genTimer_ms(_timer_ms);
    _timer_ms += MAN_SENDDATA_INTERVAL_MS;
}

void TimerGen::Destroy()
{
    m_timer_frame_count.stop();
}

void TimerGen::SetTimerInterval(const int& interval_ms)
{
    m_timer_gen.setInterval(interval_ms);
}

void TimerGen::SetTimerEnable(const bool& enable)
{
    if(enable && m_timer_gen.interval() != 0)
    {
        m_timer_gen.start();
    }
    else
    {
        m_timer_gen.stop();
    }
}

void TimerGen::FrameSended()
{
    m_frame_count_s++;
}

