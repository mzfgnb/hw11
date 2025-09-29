#include <QMainWindow>
#include <QChartView>
#include <QPieSeries>
#include <QChart>
#include <QListWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void saveNote();
    void deleteNote();
    void editNote(QListWidgetItem *item);
    void loadNotes();
    void clearAllNotes();
    void searchNotes(const QString &searchText);
    void buildChart();
    void updateChart();
    void addData();
    void removeData();
    void saveDataToFile();

private:
    void setupUI();
    void setupChart();
    void loadDataFromFile();
    void saveNotesToFile();

private:
    // UI элементы
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    
    // Левая панель - заметки
    QListWidget *notesList;
    QTextEdit *textEdit;
    QLineEdit *searchEdit;
    QPushButton *saveButton;
    QPushButton *deleteButton;
    QPushButton *loadButton;
    QPushButton *clearAllButton;
    
    // Правая панель - данные и диаграмма
    QTableWidget *dataTable;
    QChartView *chartView;
    QPushButton *buildChartButton;
    QPushButton *updateChartButton;
    QPushButton *addDataButton;
    QPushButton *removeDataButton;
    QPushButton *saveDataButton;
    
    // Данные диаграммы
    QPieSeries *pieSeries;
    QChart *chart;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(new QWidget(this))
    , mainLayout(new QVBoxLayout(centralWidget))
    , topLayout(new QHBoxLayout())
    , leftLayout(new QVBoxLayout())
    , rightLayout(new QVBoxLayout())
    , notesList(new QListWidget())
    , textEdit(new QTextEdit())
    , searchEdit(new QLineEdit())
    , saveButton(new QPushButton("Сохранить заметку"))
    , deleteButton(new QPushButton("Удалить заметку"))
    , loadButton(new QPushButton("Загрузить заметки"))
    , clearAllButton(new QPushButton("Очистить все"))
    , dataTable(new QTableWidget())
    , chartView(new QChartView())
    , buildChartButton(new QPushButton("Построить диаграмму"))
    , updateChartButton(new QPushButton("Обновить диаграмму"))
    , addDataButton(new QPushButton("Добавить данные"))
    , removeDataButton(new QPushButton("Удалить данные"))
    , saveDataButton(new QPushButton("Сохранить данные"))
    , pieSeries(new QPieSeries())
    , chart(new QChart())
{
    setupUI();
    setupChart();
    
    // Загрузка данных при запуске
    loadNotes();
    loadDataFromFile();
    
    setWindowTitle("Менеджер заметок и аналитики данных - Qt 6");
    setMinimumSize(1200, 800);
}

MainWindow::~MainWindow()
{
    saveNotesToFile();
    saveDataToFile();
}

void MainWindow::setupUI()
{
    // Настройка левой панели (заметки)
    QLabel *notesLabel = new QLabel("Заметки:");
    leftLayout->addWidget(notesLabel);
    
    searchEdit->setPlaceholderText("Поиск заметок...");
    leftLayout->addWidget(searchEdit);
    
    leftLayout->addWidget(notesList);
    
    QLabel *editLabel = new QLabel("Редактор заметок:");
    leftLayout->addWidget(editLabel);
    
    textEdit->setPlaceholderText("Введите вашу заметку здесь...");
    leftLayout->addWidget(textEdit);
    
    QHBoxLayout *notesButtonsLayout = new QHBoxLayout();
    notesButtonsLayout->addWidget(saveButton);
    notesButtonsLayout->addWidget(deleteButton);
    leftLayout->addLayout(notesButtonsLayout);
    
    QHBoxLayout *notesControlsLayout = new QHBoxLayout();
    notesControlsLayout->addWidget(loadButton);
    notesControlsLayout->addWidget(clearAllButton);
    leftLayout->addLayout(notesControlsLayout);
    
    // Настройка правой панели (данные и диаграмма)
    QLabel *dataLabel = new QLabel("Данные для диаграммы:");
    rightLayout->addWidget(dataLabel);
    
    dataTable->setColumnCount(2);
    dataTable->setHorizontalHeaderLabels({"Категория", "Значение"});
    dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rightLayout->addWidget(dataTable);
    
    QHBoxLayout *dataButtonsLayout = new QHBoxLayout();
    dataButtonsLayout->addWidget(addDataButton);
    dataButtonsLayout->addWidget(removeDataButton);
    dataButtonsLayout->addWidget(saveDataButton);
    rightLayout->addLayout(dataButtonsLayout);
    
    QLabel *chartLabel = new QLabel("Круговая диаграмма:");
    rightLayout->addWidget(chartLabel);
    
    rightLayout->addWidget(chartView);
    
    QHBoxLayout *chartButtonsLayout = new QHBoxLayout();
    chartButtonsLayout->addWidget(buildChartButton);
    chartButtonsLayout->addWidget(updateChartButton);
    rightLayout->addLayout(chartButtonsLayout);
    
    // Компоновка главного окна
    topLayout->addLayout(leftLayout, 1);
    topLayout->addLayout(rightLayout, 1);
    mainLayout->addLayout(topLayout);
    
    setCentralWidget(centralWidget);
    
    // Стили кнопок
    saveButton->setStyleSheet("QPushButton { background-color: green; color: white; border: none; padding: 8px; border-radius: 4px; }");
    deleteButton->setStyleSheet("QPushButton { background-color: red; color: white; border: none; padding: 8px; border-radius: 4px; }");
    loadButton->setStyleSheet("QPushButton { background-color: blue; color: white; border: none; padding: 8px; border-radius: 4px; }");
    clearAllButton->setStyleSheet("QPushButton { background-color: orange; color: white; border: none; padding: 8px; border-radius: 4px; }");
    buildChartButton->setStyleSheet("QPushButton { background-color: purple; color: white; border: none; padding: 8px; border-radius: 4px; }");
    updateChartButton->setStyleSheet("QPushButton { background-color: teal; color: white; border: none; padding: 8px; border-radius: 4px; }");
    addDataButton->setStyleSheet("QPushButton { background-color: pink; color: black; border: none; padding: 8px; border-radius: 4px; }");
    removeDataButton->setStyleSheet("QPushButton { background-color: brown; color: white; border: none; padding: 8px; border-radius: 4px; }");
    saveDataButton->setStyleSheet("QPushButton { background-color: darkgreen; color: white; border: none; padding: 8px; border-radius: 4px; }");
    
    // Подключение сигналов
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveNote);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteNote);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadNotes);
    connect(clearAllButton, &QPushButton::clicked, this, &MainWindow::clearAllNotes);
    connect(notesList, &QListWidget::itemDoubleClicked, this, &MainWindow::editNote);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::searchNotes);
    connect(buildChartButton, &QPushButton::clicked, this, &MainWindow::buildChart);
    connect(updateChartButton, &QPushButton::clicked, this, &MainWindow::updateChart);
    connect(addDataButton, &QPushButton::clicked, this, &MainWindow::addData);
    connect(removeDataButton, &QPushButton::clicked, this, &MainWindow::removeData);
    connect(saveDataButton, &QPushButton::clicked, this, &MainWindow::saveDataToFile);
}

void MainWindow::setupChart()
{
    chart->addSeries(pieSeries);
    chart->setTitle("Круговая диаграмма данных");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::saveNote()
{
    QString noteText = textEdit->toPlainText().trimmed();
    
    if (!noteText.isEmpty()) {
        QString timestamp = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
        QString noteWithTime = noteText + " [" + timestamp + "]";
        notesList->addItem(noteWithTime);
        textEdit->clear();
        saveNotesToFile();
        QMessageBox::information(this, "Успех", "Заметка сохранена!");
    } else {
        QMessageBox::warning(this, "Ошибка", "Заметка не может быть пустой!");
    }
}

void MainWindow::deleteNote()
{
    QListWidgetItem *selectedItem = notesList->currentItem();
    if (selectedItem) {
        if (QMessageBox::question(this, "Удаление", "Удалить выбранную заметку?") == QMessageBox::Yes) {
            delete selectedItem;
            saveNotesToFile();
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите заметку для удаления!");
    }
}

void MainWindow::editNote(QListWidgetItem *item)
{
    QString text = item->text();
    int bracketPos = text.lastIndexOf('[');
    if (bracketPos > 0) {
        text = text.left(bracketPos).trimmed();
    }
    textEdit->setPlainText(text);
    delete item;
}

void MainWindow::loadNotes()
{
    QFile file("notes.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        notesList->clear();
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.isEmpty()) {
                notesList->addItem(line);
            }
        }
        file.close();
    }
}

void MainWindow::clearAllNotes()
{
    if (QMessageBox::question(this, "Очистка", "Удалить все заметки?") == QMessageBox::Yes) {
        notesList->clear();
        saveNotesToFile();
    }
}

void MainWindow::searchNotes(const QString &searchText)
{
    for (int i = 0; i < notesList->count(); ++i) {
        QListWidgetItem *item = notesList->item(i);
        item->setHidden(!item->text().contains(searchText, Qt::CaseInsensitive));
    }
}

void MainWindow::saveNotesToFile()
{
    QFile file("notes.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < notesList->count(); ++i) {
            out << notesList->item(i)->text() << "\n";
        }
        file.close();
    }
}

void MainWindow::loadDataFromFile()
{
    QFile file("data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        dataTable->setRowCount(0);
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(";");
            if (parts.size() == 2) {
                int row = dataTable->rowCount();
                dataTable->insertRow(row);
                dataTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
                dataTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            }
        }
        file.close();
    }
}

void MainWindow::buildChart()
{
    pieSeries->clear();
    double total = 0;
    QVector<QPair<QString, double>> data;
    
    for (int row = 0; row < dataTable->rowCount(); ++row) {
        QTableWidgetItem *categoryItem = dataTable->item(row, 0);
        QTableWidgetItem *valueItem = dataTable->item(row, 1);
        
        if (categoryItem && valueItem) {
            QString category = categoryItem->text();
            bool ok;
            double value = valueItem->text().toDouble(&ok);
            if (ok && value > 0) {
                data.append(qMakePair(category, value));
                total += value;
            }
        }
    }
    
    if (data.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет данных для построения диаграммы!");
        return;
    }
    
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

void MainWindow::updateChart()
{
    buildChart();
}

void MainWindow::addData()
{
    bool ok;
    QString category = QInputDialog::getText(this, "Добавить данные", "Введите категорию:", QLineEdit::Normal, "", &ok);
    if (ok && !category.isEmpty()) {
        double value = QInputDialog::getDouble(this, "Добавить данные", "Введите значение:", 0, 0, 1000000, 2, &ok);
        if (ok) {
            int row = dataTable->rowCount();
            dataTable->insertRow(row);
            dataTable->setItem(row, 0, new QTableWidgetItem(category));
            dataTable->setItem(row, 1, new QTableWidgetItem(QString::number(value)));
            saveDataToFile();
        }
    }
}

void MainWindow::removeData()
{
    int currentRow = dataTable->currentRow();
    if (currentRow >= 0) {
        dataTable->removeRow(currentRow);
        saveDataToFile();
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите строку для удаления!");
    }
}

void MainWindow::saveDataToFile()
{
    QFile file("data.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < dataTable->rowCount(); ++row) {
            QTableWidgetItem *categoryItem = dataTable->item(row, 0);
            QTableWidgetItem *valueItem = dataTable->item(row, 1);
            
            if (categoryItem && valueItem) {
                QString category = categoryItem->text();
                QString value = valueItem->text();
                out << category << ";" << value << "\n";
            }
        }
        file.close();
    }
}

// main.cpp
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

#include "main.moc"
