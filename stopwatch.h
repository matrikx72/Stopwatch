#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

// Структура для хранения данных круга
struct LapData {
    int number;
    double time;
};

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);

    void start();
    void stop();
    void reset();

    // Теперь метод возвращает структуру, а не испускает сигнал
    LapData lap();

    bool isRunning() const;

signals:
    void timeUpdated(double seconds);
    // Сигнал lapRecorded УДАЛЕН

private slots:
    void onTimerTick();

private:
    QTimer *m_timer;
    QElapsedTimer m_elapsed;
    qint64 m_lastLapTime;
    qint64 m_accumulatedTime;
    int m_lapCount;
    bool m_isRunning;
};

#endif // STOPWATCH_H
