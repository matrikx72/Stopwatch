#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    connect(m_timer, &QTimer::timeout, this, &Stopwatch::onTimerTick);

    m_isRunning = false;
    m_lapCount = 0;
    m_lastLapTime = 0;
    m_accumulatedTime = 0;
}

void Stopwatch::start()
{
    if (!m_isRunning) {
        m_elapsed.start();
        m_timer->start();
        m_isRunning = true;
    }
}

void Stopwatch::stop()
{
    if (m_isRunning) {
        m_timer->stop();
        m_accumulatedTime += m_elapsed.elapsed();
        m_isRunning = false;
    }
}

void Stopwatch::reset()
{
    // Если секундомер работает, перезапускаем таймер с нуля, но НЕ останавливаем
    if (m_isRunning) {
        m_elapsed.start();
    }

    m_accumulatedTime = 0;
    m_lapCount = 0;
    m_lastLapTime = 0;

    emit timeUpdated(0.0);
}

LapData Stopwatch::lap()
{
    LapData result = {0, 0.0};

    if (!m_isRunning) return result;

    qint64 totalCurrentTime = m_accumulatedTime + m_elapsed.elapsed();
    double lapTime = (totalCurrentTime - m_lastLapTime) / 1000.0;

    m_lastLapTime = totalCurrentTime;
    m_lapCount++;

    result.number = m_lapCount;
    result.time = lapTime;

    return result;
}

bool Stopwatch::isRunning() const
{
    return m_isRunning;
}

void Stopwatch::onTimerTick()
{
    double totalSeconds = (m_accumulatedTime + m_elapsed.elapsed()) / 1000.0;
    emit timeUpdated(totalSeconds);
}
