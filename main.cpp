#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

const int MAX_STUDENTS = 100;
const int MAX_NAME_LENGTH = 50;
const int MAX_COURSE_LENGTH = 50;

struct Student {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    char course[MAX_COURSE_LENGTH];
};

Student students[MAX_STUDENTS];
int studentCount = 0;

void printCentered(const string &text, int width) {
    int padding = (width - text.length()) / 2;
    for (int i = 0; i < padding; ++i) {
        cout << " ";
    }
    cout << text << endl;
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
    int choice;
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
        printCentered("Enter your choice: ", 100);
        cin >> choice;

        switch (choice) {
            case 1: insertStudent(); break;
            case 2: viewAllStudents(); break;
            case 3: searchStudentByID(); break;
            case 4: searchStudentByName(); break;
            case 5: deleteStudent(); break;
            case 6: updateStudent(); break;
            case 7: sortStudentsByName(); break;
            case 8: displayStatistics(); break;
            case 9: printCentered("Exiting the program.", 50); break;
            default: printCentered("Invalid choice. Please try again.", 50);
        }
    } while (choice != 9);

    return 0;
}