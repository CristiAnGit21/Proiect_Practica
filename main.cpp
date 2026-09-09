#include <iostream>
#include <string>

const int MAX_STUDENTS = 100;

struct Student {
    int id;
    std::string name;
    int age;
    std::string course;
};

Student students[MAX_STUDENTS];
int studentCount = 0;

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
    int padding = (width - text.length()) / 2;
    for (int i = 0; i < padding; ++i) {
        std::cout << " ";
    }
    std::cout << text << std::endl;
}

void insertStudent() { printCentered("Feature in progress...", 100); }
void viewAllStudents() { printCentered("Feature in progress...", 100); }
void searchStudentByID() { printCentered("Feature in progress...", 100); }
void searchStudentByName() { printCentered("Feature in progress...", 100); }
void deleteStudent() { printCentered("Feature in progress...", 100); }
void updateStudent() { printCentered("Feature in progress...", 100); }
void sortStudentsByName() { printCentered("Feature in progress...", 100); }
void displayStatistics() { printCentered("Feature in progress...", 100); }

int main() {
    int UserChoice;
    do{
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
        printCentered("Enter your choice: ", 100);

    std:: cin >> UserChoice;
    MenuOption choice = static_cast<MenuOption>(UserChoice);

    switch (choice) {
        case MenuOption::Insert: insertStudent(); break;
        case MenuOption::ViewAll: viewAllStudents(); break;
        case MenuOption::SearchByID: searchStudentByID(); break;
        case MenuOption::SearchByName: searchStudentByName(); break;
        case MenuOption::Delete: deleteStudent(); break;
        case MenuOption::Update: updateStudent(); break;
        case MenuOption::SortByName: sortStudentsByName(); break;
        case MenuOption::DisplayStatistics: displayStatistics(); break;
        case MenuOption::Exit: printCentered("Exiting the program.", 50); break;
        default: printCentered("Invalid choice. Please try again.", 50);
    }
} while (static_cast<MenuOption>(UserChoice) != MenuOption::Exit);

    return 0;
}