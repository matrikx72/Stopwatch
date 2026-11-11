#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h" // Подключаем наш класс

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartStopClicked();
    void onClearClicked();
    void onLapClicked();
    void updateTime(double seconds);
    void recordLap(int lapNumber, double lapTime);

private:
    Ui::MainWindow *ui;
    Stopwatch *m_stopwatch; // Наш секундомер
};

#endif // MAINWINDOW_H
