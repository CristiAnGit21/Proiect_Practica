#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QDoubleSpinBox>
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
    void filterByCourse();
    void applySearch();
    void clearSearch();
    void onStudentSelectionChanged();
    void addGrade();
    void removeGrade();

private:
    StudentRepository repo;

    // Student table (left)
    QTableWidget   *table;
    QLineEdit      *searchEdit;
    QComboBox      *searchField;
    QPushButton    *editBtn;
    QPushButton    *deleteBtn;
    QLabel         *statusLabel;

    // Grades panel (right)
    QLabel         *gradesPanelTitle;
    QListWidget    *gradesList;
    QLabel         *averageLabel;
    QDoubleSpinBox *newGradeSpin;
    QPushButton    *addGradeBtn;
    QPushButton    *removeGradeBtn;

    void setupUi();
    void populateTable(const std::vector<Student> &students);
    void refreshTable();
    void refreshGradesPanel(int studentId);
    void clearGradesPanel();
    int  selectedStudentId() const;
};
