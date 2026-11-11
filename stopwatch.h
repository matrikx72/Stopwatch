#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);

    void start();
    void stop();
    void reset();
    void lap();
    bool isRunning() const;

signals:
    void timeUpdated(double seconds);
    void lapRecorded(int lapNumber, double lapTime);

private slots:
    void onTimerTick();

private:
    QTimer *m_timer;
    QElapsedTimer m_elapsed;
    qint64 m_lastLapTime;
    qint64 m_accumulatedTime; // <-- НОВОЕ: время, накопленное до паузы (в мс)
    int m_lapCount;
    bool m_isRunning;
};

#endif // STOPWATCH_H
