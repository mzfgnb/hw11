#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pieSeries(new QPieSeries())
    , chart(new QChart())
{
    ui->setupUi(this);
    
    // Установка стилей для кнопок
    ui->saveButton->setStyleSheet("background-color: green; color: white;");
    ui->deleteButton->setStyleSheet("background-color: red; color: white;");
    ui->loadButton->setStyleSheet("background-color: blue; color: white;");
    ui->clearAllButton->setStyleSheet("background-color: orange; color: white;");
    ui->buildChartButton->setStyleSheet("background-color: purple; color: white;");
    ui->updateChartButton->setStyleSheet("background-color: teal; color: white;");
    ui->addDataButton->setStyleSheet("background-color: pink; color: black;");
    ui->removeDataButton->setStyleSheet("background-color: brown; color: white;");
    ui->saveDataButton->setStyleSheet("background-color: darkgreen; color: white;");
    
    // Подключение сигналов к слотам
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveNote);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::deleteNote);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::loadNotes);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &MainWindow::clearAllNotes);
    connect(ui->notesList, &QListWidget::itemDoubleClicked, this, &MainWindow::editNote);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &MainWindow::searchNotes);
    
    // Подключение кнопок для работы с диаграммой
    connect(ui->buildChartButton, &QPushButton::clicked, this, &MainWindow::buildChart);
    connect(ui->updateChartButton, &QPushButton::clicked, this, &MainWindow::updateChart);
    connect(ui->addDataButton, &QPushButton::clicked, this, &MainWindow::addData);
    connect(ui->removeDataButton, &QPushButton::clicked, this, &MainWindow::removeData);
    connect(ui->saveDataButton, &QPushButton::clicked, this, &MainWindow::saveDataToFile);
    
    // Настройка таблицы
    ui->dataTable->setColumnCount(2);
    ui->dataTable->setHorizontalHeaderLabels({"Категория", "Значение"});
    ui->dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Загрузка данных и заметок при запуске
    loadNotes();
    loadDataFromFile();
    
    // Установка подсказок
    ui->textEdit->setPlaceholderText("Введите вашу заметку здесь...");
    ui->searchEdit->setPlaceholderText("Поиск заметок...");
    
    // Настройка диаграммы
    setupChart();
    
    // Установка заголовка окна
    setWindowTitle("Менеджер заметок и аналитики данных");
}

MainWindow::~MainWindow()
{
    // Сохранение заметок и данных при закрытии
    saveNotesToFile();
    saveDataToFile();
    delete ui;
}

void MainWindow::saveNote() {
    QString noteText = ui->textEdit->toPlainText().trimmed();
    
    if (!noteText.isEmpty()) {
        // Добавляем timestamp к заметке
        QString timestamp = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
        QString noteWithTime = noteText + " [" + timestamp + "]";
        
        ui->notesList->addItem(noteWithTime);
        ui->textEdit->clear();
        saveNotesToFile();
    }
}

void MainWindow::deleteNote() {
    QListWidgetItem *selectedItem = ui->notesList->currentItem();
    if (selectedItem) {
        delete selectedItem;
        saveNotesToFile();
    }
}

void MainWindow::editNote(QListWidgetItem *item) {
    QString text = item->text();
    // Удаляем timestamp для редактирования
    int bracketPos = text.lastIndexOf('[');
    if (bracketPos > 0) {
        text = text.left(bracketPos).trimmed();
    }
    
    ui->textEdit->setPlainText(text);
    delete item;
}

void MainWindow::loadNotes() {
    QFile file("notes.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->notesList->clear();
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.isEmpty()) {
                ui->notesList->addItem(line);
            }
        }
        file.close();
    }
}

void MainWindow::clearAllNotes() {
    if (QMessageBox::question(this, "Очистка", "Удалить все заметки?") == QMessageBox::Yes) {
        ui->notesList->clear();
        saveNotesToFile();
    }
}

void MainWindow::searchNotes(const QString &searchText) {
    for (int i = 0; i < ui->notesList->count(); ++i) {
        QListWidgetItem *item = ui->notesList->item(i);
        item->setHidden(!item->text().contains(searchText, Qt::CaseInsensitive));
    }
}

void MainWindow::saveNotesToFile() {
    QFile file("notes.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < ui->notesList->count(); ++i) {
            out << ui->notesList->item(i)->text() << "\n";
        }
        file.close();
    }
}

// Методы для работы с данными и диаграммой
void MainWindow::loadDataFromFile() {
    QFile file("data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->dataTable->setRowCount(0);
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(";");
            if (parts.size() == 2) {
                int row = ui->dataTable->rowCount();
                ui->dataTable->insertRow(row);
                ui->dataTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
                ui->dataTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            }
        }
        file.close();
    }
}

void MainWindow::setupChart() {
    chart->addSeries(pieSeries);
    chart->setTitle("Круговая диаграмма данных");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::buildChart() {
    pieSeries->clear();
    
    double total = 0;
    QVector<QPair<QString, double>> data;
    
    // Собираем данные из таблицы
    for (int row = 0; row < ui->dataTable->rowCount(); ++row) {
        QString category = ui->dataTable->item(row, 0)->text();
        double value = ui->dataTable->item(row, 1)->text().toDouble();
        if (value > 0) {
            data.append(qMakePair(category, value));
            total += value;
        }
    }
    
    if (data.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет данных для построения диаграммы!");
        return;
    }
    
    // Добавляем сектора в диаграмму
    QStringList colors = {"#FF6384", "#36A2EB", "#FFCE56", "#4BC0C0", "#9966FF", 
                         "#FF9F40", "#FF6384", "#C9CBCF", "#4BC0C0", "#FFCD56"};
    
    for (int i = 0; i < data.size(); ++i) {
        double percentage = (data[i].second / total) * 100;
        QPieSlice *slice = pieSeries->append(data[i].first + QString(" (%1%)").arg(percentage, 0, 'f', 1), data[i].second);
        slice->setBrush(QColor(colors[i % colors.size()]));
        slice->setLabelVisible(true);
    }
    
    chart->setTitle(QString("Круговая диаграмма (Всего: %1)").arg(total));
}

void MainWindow::updateChart() {
    buildChart();
}

void MainWindow::addData() {
    bool ok;
    QString category = QInputDialog::getText(this, "Добавить данные", 
                                           "Введите категорию:", QLineEdit::Normal, "", &ok);
    if (ok && !category.isEmpty()) {
        double value = QInputDialog::getDouble(this, "Добавить данные", 
                                             "Введите значение:", 0, 0, 1000000, 2, &ok);
        if (ok) {
            int row = ui->dataTable->rowCount();
            ui->dataTable->insertRow(row);
            ui->dataTable->setItem(row, 0, new QTableWidgetItem(category));
            ui->dataTable->setItem(row, 1, new QTableWidgetItem(QString::number(value)));
            saveDataToFile();
        }
    }
}

void MainWindow::removeData() {
    int currentRow = ui->dataTable->currentRow();
    if (currentRow >= 0) {
        ui->dataTable->removeRow(currentRow);
        saveDataToFile();
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите строку для удаления!");
    }
}

void MainWindow::saveDataToFile() {
    QFile file("data.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < ui->dataTable->rowCount(); ++row) {
            QString category = ui->dataTable->item(row, 0)->text();
            QString value = ui->dataTable->item(row, 1)->text();
            out << category << ";" << value << "\n";
        }
        file.close();
    }
}
