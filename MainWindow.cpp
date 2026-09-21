#include "MainWindow.h"
#include "StudentDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QToolButton>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), repo("students.csv")
{
    repo.load();
    setupUi();
    refreshTable();
}

void MainWindow::setupUi() {
    setWindowTitle("Student Management System");
    resize(750, 520);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Search bar
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchEdit  = new QLineEdit;
    searchEdit->setPlaceholderText("Search...");
    searchField = new QComboBox;
    searchField->addItem("By Name");
    searchField->addItem("By Course");
    QPushButton *searchBtn = new QPushButton("Search");
    QPushButton *clearBtn  = new QPushButton("Clear");
    searchLayout->addWidget(searchEdit, 1);
    searchLayout->addWidget(searchField);
    searchLayout->addWidget(searchBtn);
    searchLayout->addWidget(clearBtn);
    mainLayout->addLayout(searchLayout);

    // Table
    table = new QTableWidget;
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Name", "Age", "Course"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    mainLayout->addWidget(table);

    // Action buttons
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *addBtn = new QPushButton("Add");
    editBtn             = new QPushButton("Edit");
    deleteBtn           = new QPushButton("Delete");
    editBtn->setEnabled(false);
    deleteBtn->setEnabled(false);

    QToolButton *sortBtn = new QToolButton;
    sortBtn->setText("Sort ▾");
    sortBtn->setPopupMode(QToolButton::InstantPopup);
    QMenu *sortMenu = new QMenu(this);
    sortMenu->addAction("By Name",   [this]{ repo.sortBy(SortField::Name);  refreshTable(); });
    sortMenu->addAction("By Age",    [this]{ repo.sortBy(SortField::Age);   refreshTable(); });
    sortMenu->addAction("By ID",     [this]{ repo.sortBy(SortField::Id);    refreshTable(); });
    sortBtn->setMenu(sortMenu);

    QPushButton *statsBtn = new QPushButton("Statistics");

    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(sortBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(statsBtn);
    mainLayout->addLayout(btnLayout);

    // Status bar
    statusLabel = new QLabel;
    statusBar()->addWidget(statusLabel);

    // Connections
    connect(addBtn,    &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(editBtn,   &QPushButton::clicked, this, &MainWindow::editStudent);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteStudent);
    connect(statsBtn,  &QPushButton::clicked, this, &MainWindow::showStatistics);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::applySearch);
    connect(clearBtn,  &QPushButton::clicked, this, &MainWindow::clearSearch);
    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::applySearch);
    connect(table, &QTableWidget::itemDoubleClicked, this, &MainWindow::editStudent);
    connect(table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
}

void MainWindow::populateTable(const std::vector<Student> &students) {
    table->setRowCount(static_cast<int>(students.size()));
    for (int i = 0; i < static_cast<int>(students.size()); ++i) {
        const auto &s = students[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::number(s.id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(s.name)));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(s.age)));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(s.course)));
    }
    statusLabel->setText(QString("Total: %1 students").arg(students.size()));
}

void MainWindow::refreshTable() {
    populateTable(repo.getAll());
}

int MainWindow::selectedStudentId() const {
    const QList<QTableWidgetItem *> selected = table->selectedItems();
    if (selected.isEmpty()) return -1;
    return table->item(selected.first()->row(), 0)->text().toInt();
}

void MainWindow::onSelectionChanged() {
    bool has = !table->selectedItems().isEmpty();
    editBtn->setEnabled(has);
    deleteBtn->setEnabled(has);
}

void MainWindow::addStudent() {
    StudentDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        repo.insert(dlg.name(), dlg.age(), dlg.course());
        refreshTable();
    }
}

void MainWindow::editStudent() {
    int id = selectedStudentId();
    if (id == -1) return;
    auto student = repo.findById(id);
    if (!student) return;

    StudentDialog dlg(this, *student);
    if (dlg.exec() == QDialog::Accepted) {
        repo.update(id, dlg.name(), dlg.age(), dlg.course());
        refreshTable();
    }
}

void MainWindow::deleteStudent() {
    int id = selectedStudentId();
    if (id == -1) return;
    auto student = repo.findById(id);
    if (!student) return;

    auto reply = QMessageBox::question(
        this, "Confirm Delete",
        QString("Delete student \"%1\"?").arg(QString::fromStdString(student->name)));
    if (reply == QMessageBox::Yes) {
        repo.remove(id);
        refreshTable();
    }
}

void MainWindow::showStatistics() {
    QString msg = QString("Total students: %1\nAverage age: %2")
        .arg(repo.count())
        .arg(repo.averageAge(), 0, 'f', 2);
    QMessageBox::information(this, "Statistics", msg);
}

void MainWindow::applySearch() {
    QString query = searchEdit->text().trimmed();
    if (query.isEmpty()) { refreshTable(); return; }

    std::vector<Student> results = (searchField->currentIndex() == 0)
        ? repo.findByName(query.toStdString())
        : repo.findByCourse(query.toStdString());
    populateTable(results);
}

void MainWindow::clearSearch() {
    searchEdit->clear();
    refreshTable();
}
