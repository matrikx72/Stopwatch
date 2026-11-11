#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QAbstractItemView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Скрываем номера строк слева (чтобы не было дубля 1, 2, 3...)
    ui->lapTableWidget->verticalHeader()->setVisible(false);

    // 2. Растягиваем колонки на всю ширину таблицы
    // (Для этого нужен #include <QHeaderView> в начале файла)
    ui->lapTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 3. Включаем сетку и делаем её видимой (серой)
    ui->lapTableWidget->setShowGrid(true);
    ui->lapTableWidget->setStyleSheet( "QTableWidget {"
                                       "   gridline-color: #808080;"
                                       "   border: 1px solid #808080;"
                                       "QHeaderView::section {"
                                       "   border: 1px solid #808080;"
                                       "   border-bottom: 2px solid #404040;"
                                       "   background-color: #f0f0f0;"
                                       "   font-weight: bold;"
                                       "   padding: 4px;"
                                       "}");

    // 4. Запрещаем редактирование ячеек
    ui->lapTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Создаем экземпляр секундомера
    m_stopwatch = new Stopwatch(this);

    // Подключаем сигналы от секундомера к слотам интерфейса (требование критериев)
    connect(m_stopwatch, &Stopwatch::timeUpdated, this, &MainWindow::updateTime);
    connect(m_stopwatch, &Stopwatch::lapRecorded, this, &MainWindow::recordLap);

    // Подключаем кнопки
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->lapButton, &QPushButton::clicked, this, &MainWindow::onLapClicked);

    // Изначально кнопка "Круг" неактивна
    ui->lapButton->setEnabled(false);

    // Начальный текст лейбла
    ui->timeLabel->setText("0.0 сек");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartStopClicked()
{
    if (m_stopwatch->isRunning()) {
        // Если работает - останавливаем
        m_stopwatch->stop();
        ui->startStopButton->setText("Старт");
        ui->lapButton->setEnabled(false); // Деактивируем круг
    } else {
        // Если стоит - запускаем
        m_stopwatch->start();
        ui->startStopButton->setText("Стоп");
        ui->lapButton->setEnabled(true);  // Активируем круг
    }
}

void MainWindow::onClearClicked()
{
    m_stopwatch->reset();
    ui->lapTableWidget->setRowCount(0);
    ui->startStopButton->setText("Старт"); // Возвращаем текст кнопки
    ui->lapButton->setEnabled(false);
}

void MainWindow::onLapClicked()
{
    m_stopwatch->lap();
}

void MainWindow::updateTime(double seconds)
{
    // Обновляем лейбл с точностью до 0.1 сек
    ui->timeLabel->setText(QString::number(seconds, 'f', 1) + " сек");
}

void MainWindow::recordLap(int lapNumber, double lapTime)
{
    // Получаем текущее количество строк в таблице
    int row = ui->lapTableWidget->rowCount();

    // Вставляем новую пустую строку в конец
    ui->lapTableWidget->insertRow(row);

    // Создаем элементы для ячеек (номер круга и время)
    QTableWidgetItem *itemLap = new QTableWidgetItem(QString::number(lapNumber));
    QTableWidgetItem *itemTime = new QTableWidgetItem(QString::number(lapTime, 'f', 1) + " сек");

    // Выравниваем текст по центру для красоты
    itemLap->setTextAlignment(Qt::AlignCenter);
    itemTime->setTextAlignment(Qt::AlignCenter);

    // Устанавливаем элементы в таблицу
    ui->lapTableWidget->setItem(row, 0, itemLap);   // Колонка 0 (Круг)
    ui->lapTableWidget->setItem(row, 1, itemTime);  // Колонка 1 (Время)

    // Автоматически прокручиваем вниз, чтобы был виден последний круг
    ui->lapTableWidget->scrollToBottom();
}
