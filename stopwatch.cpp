#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    connect(m_timer, &QTimer::timeout, this, &Stopwatch::onTimerTick);

    m_isRunning = false;
    m_lapCount = 0;
    m_lastLapTime = 0;
    m_accumulatedTime = 0; // Инициализируем
}

void Stopwatch::start()
{
    if (!m_isRunning) {
        m_elapsed.start(); // Запускаем отсчет текущего отрезка
        m_timer->start();
        m_isRunning = true;
    }
}

void Stopwatch::stop()
{
    if (m_isRunning) {
        m_timer->stop();
        // Сохраняем время, которое прошло с момента старта
        m_accumulatedTime += m_elapsed.elapsed();
        m_isRunning = false;
    }
}

void Stopwatch::reset()
{
    stop(); // Останавливаем, если шло
    m_accumulatedTime = 0; // Обнуляем накопленное
    m_lapCount = 0;
    m_lastLapTime = 0;
    emit timeUpdated(0.0);
}

void Stopwatch::lap()
{
    if (!m_isRunning) return;

    // Общее прошедшее время = накопленное + текущее
    qint64 totalCurrentTime = m_accumulatedTime + m_elapsed.elapsed();

    // Время круга = общее время - время предыдущего круга
    double lapTime = (totalCurrentTime - m_lastLapTime) / 1000.0;

    m_lastLapTime = totalCurrentTime;
    m_lapCount++;

    emit lapRecorded(m_lapCount, lapTime);
}

bool Stopwatch::isRunning() const
{
    return m_isRunning;
}

void Stopwatch::onTimerTick()
{
    // Отправляем общее время: накопленное + текущее
    double totalSeconds = (m_accumulatedTime + m_elapsed.elapsed()) / 1000.0;
    emit timeUpdated(totalSeconds);
}
