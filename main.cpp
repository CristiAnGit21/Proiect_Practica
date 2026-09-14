#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include "Student.h"
#include "StudentRepository.h"

enum class MenuOption {
    Insert = 1,
    ViewAll = 2,
    SearchByID = 3,
    SearchByName = 4,
    Delete = 5,
    Update = 6,
    SortByName = 7,
    DisplayStatistics = 8,
    Exit = 9
};

void printCentered(const std::string &text, int width) {
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding < 0) padding = 0;
    std::cout << std::string(padding, ' ') << text << std::endl;
}

int readInt(const std::string &prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        bool ok = static_cast<bool>(std::cin >> value);
        if (std::cin.eof()) {
            std::cerr << "\nEOF reached. Exiting.\n";
            std::exit(0);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (ok) return value;
        std::cout << "Invalid number, try again.\n";
    }
}

std::string readLine(const std::string &prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, value)) {
            if (std::cin.eof()) {
                std::cerr << "\nEOF reached. Exiting.\n";
                std::exit(0);
            }
            std::cin.clear();
            continue;
        }
        if (!value.empty()) return value;
        std::cout << "Value cannot be empty, try again.\n";
    }
}

constexpr int COL_ID     = 6;
constexpr int COL_NAME   = 25;
constexpr int COL_AGE    = 6;
constexpr int COL_COURSE = 20;
constexpr int TABLE_WIDTH = COL_ID + COL_NAME + COL_AGE + COL_COURSE;

void printStudentTable(const std::vector<Student> &list) {
    if (list.empty()) {
        std::cout << "No students to display.\n";
        return;
    }
    std::cout << std::left
               << std::setw(COL_ID)     << "ID"
               << std::setw(COL_NAME)   << "Name"
               << std::setw(COL_AGE)    << "Age"
               << std::setw(COL_COURSE) << "Course" << "\n";
    std::cout << std::string(TABLE_WIDTH, '-') << "\n";
    for (const auto &s : list) {
        std::cout << std::left
                   << std::setw(COL_ID)     << s.id
                   << std::setw(COL_NAME)   << s.name
                   << std::setw(COL_AGE)    << s.age
                   << std::setw(COL_COURSE) << s.course << "\n";
    }
}

void insertStudent(StudentRepository &repo) {
    std::string name = readLine("Name: ");
    int age = readInt("Age: ");
    std::string course = readLine("Course: ");

    int id = repo.insert(name, age, course);
    std::cout << "Student added with ID " << id << ".\n";
}

void viewAllStudents(const StudentRepository &repo) {
    printStudentTable(repo.getAll());
}

void searchStudentByID(const StudentRepository &repo) {
    int id = readInt("Student ID: ");
    auto result = repo.findById(id);
    if (!result) {
        std::cout << "No student found with ID " << id << ".\n";
        return;
    }
    printStudentTable({*result});
}

void searchStudentByName(const StudentRepository &repo) {
    std::string query = readLine("Name (or part of it): ");
    auto results = repo.findByName(query);
    printStudentTable(results);
}

void deleteStudent(StudentRepository &repo) {
    int id = readInt("Student ID to delete: ");
    if (repo.remove(id)) {
        std::cout << "Student " << id << " deleted.\n";
    } else {
        std::cout << "No student found with ID " << id << ".\n";
    }
}

void updateStudent(StudentRepository &repo) {
    int id = readInt("Student ID to update: ");
    if (!repo.findById(id)) {
        std::cout << "No student found with ID " << id << ".\n";
        return;
    }
    std::string name = readLine("New name: ");
    int age = readInt("New age: ");
    std::string course = readLine("New course: ");

    repo.update(id, name, age, course);
    std::cout << "Student " << id << " updated.\n";
}

void sortStudentsByName(StudentRepository &repo) {
    repo.sortByName();
    std::cout << "Students sorted by name.\n";
    printStudentTable(repo.getAll());
}

void displayStatistics(const StudentRepository &repo) {
    std::cout << "Total students: " << repo.count() << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average age: " << repo.averageAge() << "\n";
}

int main() {
    StudentRepository repo("students.csv");
    repo.load();

    MenuOption choice = MenuOption::Insert;
    do {
        printCentered("-------- Student Management System Menu --------", 100);
        printCentered("1. Insert Student record", 100);
        printCentered("2. View All Student records", 100);
        printCentered("3. Search Student record by ID", 100);
        printCentered("4. Search Student record by Name", 100);
        printCentered("5. Delete Student record", 100);
        printCentered("6. Update Student record", 100);
        printCentered("7. Sort Students by Name", 100);
        printCentered("8. Display Statistics", 100);
        printCentered("9. Exit", 100);
        printCentered("--------------------------------------------", 100);

        int userChoice = readInt("Enter your choice: ");
        choice = static_cast<MenuOption>(userChoice);

        switch (choice) {
            case MenuOption::Insert: insertStudent(repo); break;
            case MenuOption::ViewAll: viewAllStudents(repo); break;
            case MenuOption::SearchByID: searchStudentByID(repo); break;
            case MenuOption::SearchByName: searchStudentByName(repo); break;
            case MenuOption::Delete: deleteStudent(repo); break;
            case MenuOption::Update: updateStudent(repo); break;
            case MenuOption::SortByName: sortStudentsByName(repo); break;
            case MenuOption::DisplayStatistics: displayStatistics(repo); break;
            case MenuOption::Exit: printCentered("Exiting the program.", 50); break;
            default: printCentered("Invalid choice. Please try again.", 50);
        }
        std::cout << "\n";
    } while (choice != MenuOption::Exit);

    return 0;
}
