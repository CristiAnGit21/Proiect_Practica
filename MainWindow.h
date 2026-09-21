#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "StudentRepository.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void addStudent();
    void editStudent();
    void deleteStudent();
    void showStatistics();
    void applySearch();
    void clearSearch();
    void onSelectionChanged();

private:
    StudentRepository repo;

    QTableWidget *table;
    QLineEdit    *searchEdit;
    QComboBox    *searchField;
    QPushButton  *editBtn;
    QPushButton  *deleteBtn;
    QLabel       *statusLabel;

    void setupUi();
    void populateTable(const std::vector<Student> &students);
    void refreshTable();
    int  selectedStudentId() const;
};
