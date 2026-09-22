#include "MainWindow.h"
#include "StudentDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QToolButton>
#include <QStatusBar>
#include <QInputDialog>
#include <QSplitter>
#include <QFrame>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), repo("students.csv")
{
    repo.load();
    setupUi();
    refreshTable();
}

void MainWindow::setupUi() {
    setWindowTitle("Student Management System");
    resize(950, 560);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Search bar (full width)
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

    // Splitter: left = student table, right = grades panel
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    // --- Left: student table ---
    QWidget *leftWidget = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    table = new QTableWidget;
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Name", "Age", "Course", "Average Grade"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table->verticalHeader()->setVisible(false);
    leftLayout->addWidget(table);

    // Action buttons (left side)
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
    sortMenu->addAction("By Name",   [this]{ repo.sortBy(SortField::Name); refreshTable(); });
    sortMenu->addAction("By Age",    [this]{ repo.sortBy(SortField::Age);  refreshTable(); });
    sortMenu->addAction("By ID",     [this]{ repo.sortBy(SortField::Id);   refreshTable(); });
    sortMenu->addSeparator();
    sortMenu->addAction("By Course (filter)", this, &MainWindow::filterByCourse);
    sortBtn->setMenu(sortMenu);

    QPushButton *statsBtn = new QPushButton("Statistics");

    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(sortBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(statsBtn);
    leftLayout->addLayout(btnLayout);

    splitter->addWidget(leftWidget);

    // --- Right: grades panel ---
    QGroupBox *gradesGroup = new QGroupBox("Grades");
    QVBoxLayout *gradesLayout = new QVBoxLayout(gradesGroup);

    gradesPanelTitle = new QLabel("Select a student");
    gradesPanelTitle->setAlignment(Qt::AlignCenter);
    gradesPanelTitle->setStyleSheet("font-weight: bold;");
    gradesLayout->addWidget(gradesPanelTitle);

    gradesList = new QListWidget;
    gradesLayout->addWidget(gradesList);

    averageLabel = new QLabel("Average: —");
    averageLabel->setAlignment(Qt::AlignCenter);
    averageLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #2255aa;");
    gradesLayout->addWidget(averageLabel);

    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    gradesLayout->addWidget(line);

    QHBoxLayout *addGradeLayout = new QHBoxLayout;
    newGradeSpin = new QDoubleSpinBox;
    newGradeSpin->setRange(1.0, 10.0);
    newGradeSpin->setSingleStep(0.25);
    newGradeSpin->setDecimals(2);
    newGradeSpin->setValue(5.0);
    addGradeBtn = new QPushButton("Add Grade");
    addGradeBtn->setEnabled(false);
    addGradeLayout->addWidget(newGradeSpin);
    addGradeLayout->addWidget(addGradeBtn);
    gradesLayout->addLayout(addGradeLayout);

    removeGradeBtn = new QPushButton("Remove Selected Grade");
    removeGradeBtn->setEnabled(false);
    gradesLayout->addWidget(removeGradeBtn);

    gradesGroup->setMinimumWidth(220);
    gradesGroup->setMaximumWidth(280);
    splitter->addWidget(gradesGroup);

    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    mainLayout->addWidget(splitter);

    // Status bar
    statusLabel = new QLabel;
    statusBar()->addWidget(statusLabel);

    // Connections — student table
    connect(addBtn,    &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(editBtn,   &QPushButton::clicked, this, &MainWindow::editStudent);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteStudent);
    connect(statsBtn,  &QPushButton::clicked, this, &MainWindow::showStatistics);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::applySearch);
    connect(clearBtn,  &QPushButton::clicked, this, &MainWindow::clearSearch);
    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::applySearch);
    connect(table, &QTableWidget::itemDoubleClicked, this, &MainWindow::editStudent);
    connect(table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onStudentSelectionChanged);

    // Connections — grades panel
    connect(addGradeBtn,    &QPushButton::clicked, this, &MainWindow::addGrade);
    connect(removeGradeBtn, &QPushButton::clicked, this, &MainWindow::removeGrade);
    connect(gradesList, &QListWidget::currentRowChanged, this, [this](int row){
        removeGradeBtn->setEnabled(row >= 0);
    });
}

void MainWindow::populateTable(const std::vector<Student> &students) {
    table->setRowCount(static_cast<int>(students.size()));
    for (int i = 0; i < static_cast<int>(students.size()); ++i) {
        const auto &s = students[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::number(s.id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(s.name)));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(s.age)));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(s.course)));
        QString avg = s.grades.empty() ? "—" : QString::number(s.averageGrade(), 'f', 2);
        table->setItem(i, 4, new QTableWidgetItem(avg));
    }
    statusLabel->setText(QString("Total: %1 students").arg(students.size()));
}

void MainWindow::refreshTable() {
    int selectedId = selectedStudentId();
    populateTable(repo.getAll());
    // Restore selection if possible
    if (selectedId != -1) {
        for (int i = 0; i < table->rowCount(); ++i) {
            if (table->item(i, 0)->text().toInt() == selectedId) {
                table->selectRow(i);
                break;
            }
        }
    }
}

int MainWindow::selectedStudentId() const {
    const QList<QTableWidgetItem *> sel = table->selectedItems();
    if (sel.isEmpty()) return -1;
    return table->item(sel.first()->row(), 0)->text().toInt();
}

void MainWindow::onStudentSelectionChanged() {
    int id = selectedStudentId();
    bool has = (id != -1);
    editBtn->setEnabled(has);
    deleteBtn->setEnabled(has);
    addGradeBtn->setEnabled(has);
    if (has) refreshGradesPanel(id);
    else clearGradesPanel();
}

void MainWindow::refreshGradesPanel(int studentId) {
    auto student = repo.findById(studentId);
    if (!student) { clearGradesPanel(); return; }

    gradesPanelTitle->setText(QString::fromStdString(student->name));
    gradesList->clear();
    for (size_t i = 0; i < student->grades.size(); ++i)
        gradesList->addItem(QString("%1.   %2").arg(i + 1).arg(student->grades[i], 0, 'f', 2));

    if (student->grades.empty())
        averageLabel->setText("Average: —");
    else
        averageLabel->setText(QString("Average: %1").arg(student->averageGrade(), 0, 'f', 2));

    removeGradeBtn->setEnabled(false);
}

void MainWindow::clearGradesPanel() {
    gradesPanelTitle->setText("Select a student");
    gradesList->clear();
    averageLabel->setText("Average: —");
    addGradeBtn->setEnabled(false);
    removeGradeBtn->setEnabled(false);
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
        refreshGradesPanel(id);
    }
}

void MainWindow::deleteStudent() {
    int id = selectedStudentId();
    if (id == -1) return;
    auto student = repo.findById(id);
    if (!student) return;

    auto reply = QMessageBox::question(this, "Confirm Delete",
        QString("Delete student \"%1\"?").arg(QString::fromStdString(student->name)));
    if (reply == QMessageBox::Yes) {
        repo.remove(id);
        clearGradesPanel();
        refreshTable();
    }
}

void MainWindow::addGrade() {
    int id = selectedStudentId();
    if (id == -1) return;
    repo.addGrade(id, newGradeSpin->value());
    refreshGradesPanel(id);
    refreshTable();
}

void MainWindow::removeGrade() {
    int id = selectedStudentId();
    if (id == -1) return;
    int gradeIndex = gradesList->currentRow();
    if (gradeIndex < 0) return;
    repo.removeGrade(id, gradeIndex);
    refreshGradesPanel(id);
    refreshTable();
}

void MainWindow::filterByCourse() {
    QStringList courses;
    for (const auto &s : repo.getAll()) {
        QString c = QString::fromStdString(s.course);
        if (!courses.contains(c)) courses.append(c);
    }
    courses.sort();
    bool ok;
    QString selected = QInputDialog::getItem(this, "Filter by Course", "Select course:", courses, 0, false, &ok);
    if (ok && !selected.isEmpty())
        populateTable(repo.findByCourse(selected.toStdString()));
}

void MainWindow::showStatistics() {
    double avgGrade = repo.averageGrade();
    QString gradeStr = (avgGrade > 0.0)
        ? QString::number(avgGrade, 'f', 2)
        : QString("—");
    QString msg = QString("Total students: %1\nAverage age: %2\nAverage grade (all): %3")
        .arg(repo.count())
        .arg(repo.averageAge(), 0, 'f', 2)
        .arg(gradeStr);
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
