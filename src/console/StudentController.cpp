#include "StudentController.h"
#include "Menu.h"
#include <iostream>
#include <iomanip>

namespace {
    constexpr int MIN_AGE  = 16;
    constexpr int MAX_AGE  = 100;
    constexpr size_t MIN_NAME_LEN = 2;
}

StudentController::StudentController(StudentRepository &repo) : repo(repo) {}

void StudentController::run() {
    MenuOption choice = MenuOption::Insert;
    do {
        Menu::printCentered("-------- Student Management System Menu --------", 100);
        Menu::printCentered("1.  Insert Student record", 100);
        Menu::printCentered("2.  View All Student records", 100);
        Menu::printCentered("3.  Search Student record by ID", 100);
        Menu::printCentered("4.  Search Student record by Name", 100);
        Menu::printCentered("5.  Search Student record by Course", 100);
        Menu::printCentered("6.  Delete Student record", 100);
        Menu::printCentered("7.  Update Student record", 100);
        Menu::printCentered("8.  Sort Students", 100);
        Menu::printCentered("9.  Display Statistics", 100);
        Menu::printCentered("10. Exit", 100);
        Menu::printCentered("--------------------------------------------", 100);

        int userChoice = Menu::readInt("Enter your choice: ", 1, 10);
        choice = static_cast<MenuOption>(userChoice);

        switch (choice) {
            case MenuOption::Insert:            insertStudent();          break;
            case MenuOption::ViewAll:           viewAllStudents();        break;
            case MenuOption::SearchByID:        searchStudentByID();      break;
            case MenuOption::SearchByName:      searchStudentByName();    break;
            case MenuOption::SearchByCourse:    searchStudentByCourse();  break;
            case MenuOption::Delete:            deleteStudent();          break;
            case MenuOption::Update:            updateStudent();          break;
            case MenuOption::SortBy:            sortStudents();           break;
            case MenuOption::DisplayStatistics: displayStatistics();      break;
            case MenuOption::Exit: Menu::printCentered("Exiting the program.", 50); break;
        }
        std::cout << "\n";
    } while (choice != MenuOption::Exit);
}

void StudentController::insertStudent() {
    std::string name   = Menu::readLine("Name: ", MIN_NAME_LEN);
    int age            = Menu::readInt("Age: ", MIN_AGE, MAX_AGE);
    std::string course = Menu::readLine("Course: ");

    int id = repo.insert(name, age, course);
    std::cout << "Student added with ID " << id << ".\n";
}

void StudentController::viewAllStudents() const {
    Menu::printStudentTable(repo.getAll());
}

void StudentController::searchStudentByID() const {
    int id = Menu::readInt("Student ID: ");
    auto result = repo.findById(id);
    if (!result) {
        std::cout << "No student found with ID " << id << ".\n";
        return;
    }
    Menu::printStudentTable({*result});
}

void StudentController::searchStudentByName() const {
    std::string query = Menu::readLine("Name (or part of it): ");
    Menu::printStudentTable(repo.findByName(query));
}

void StudentController::searchStudentByCourse() const {
    std::string query = Menu::readLine("Course (or part of it): ");
    Menu::printStudentTable(repo.findByCourse(query));
}

void StudentController::deleteStudent() {
    int id = Menu::readInt("Student ID to delete: ");
    if (repo.remove(id)) {
        std::cout << "Student " << id << " deleted.\n";
    } else {
        std::cout << "No student found with ID " << id << ".\n";
    }
}

void StudentController::updateStudent() {
    int id = Menu::readInt("Student ID to update: ");
    if (!repo.findById(id)) {
        std::cout << "No student found with ID " << id << ".\n";
        return;
    }
    std::string name   = Menu::readLine("New name: ", MIN_NAME_LEN);
    int age            = Menu::readInt("New age: ", MIN_AGE, MAX_AGE);
    std::string course = Menu::readLine("New course: ");

    repo.update(id, name, age, course);
    std::cout << "Student " << id << " updated.\n";
}

void StudentController::sortStudents() {
    std::cout << "Sort by:\n"
              << "  1. Name\n"
              << "  2. Age\n"
              << "  3. ID\n";
    int field = Menu::readInt("Choice: ", 1, 3);
    repo.sortBy(static_cast<SortField>(field));
    std::cout << "Students sorted.\n";
    Menu::printStudentTable(repo.getAll());
}

void StudentController::displayStatistics() const {
    std::cout << "Total students: " << repo.count() << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average age: " << repo.averageAge() << "\n";
}
