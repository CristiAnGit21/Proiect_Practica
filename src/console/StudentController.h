#pragma once
#include "StudentRepository.h"

enum class MenuOption {
    Insert = 1,
    ViewAll = 2,
    SearchByID = 3,
    SearchByName = 4,
    SearchByCourse = 5,
    Delete = 6,
    Update = 7,
    SortBy = 8,
    DisplayStatistics = 9,
    Exit = 10
};

class StudentController {
public:
    explicit StudentController(StudentRepository &repo);
    void run();

private:
    StudentRepository &repo;

    void insertStudent();
    void viewAllStudents() const;
    void searchStudentByID() const;
    void searchStudentByName() const;
    void searchStudentByCourse() const;
    void deleteStudent();
    void updateStudent();
    void sortStudents();
    void displayStatistics() const;
};
