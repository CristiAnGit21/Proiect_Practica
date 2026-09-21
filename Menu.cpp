#include "Menu.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <limits>

namespace Menu {

namespace {
    constexpr int COL_ID     = 6;
    constexpr int COL_NAME   = 25;
    constexpr int COL_AGE    = 6;
    constexpr int COL_COURSE = 20;
    constexpr int TABLE_WIDTH = COL_ID + COL_NAME + COL_AGE + COL_COURSE;

    void printTableHeader() {
        std::cout << std::left
                   << std::setw(COL_ID)     << "ID"
                   << std::setw(COL_NAME)   << "Name"
                   << std::setw(COL_AGE)    << "Age"
                   << std::setw(COL_COURSE) << "Course" << "\n";
        std::cout << std::string(TABLE_WIDTH, '-') << "\n";
    }

    void printTableRow(const Student &s) {
        std::cout << std::left
                   << std::setw(COL_ID)     << s.id
                   << std::setw(COL_NAME)   << s.name
                   << std::setw(COL_AGE)    << s.age
                   << std::setw(COL_COURSE) << s.course << "\n";
    }
}

int readInt(const std::string &prompt, int min, int max) {
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
        if (!ok) {
            std::cout << "Invalid number, try again.\n";
            continue;
        }
        if (value < min || value > max) {
            std::cout << "Value must be between " << min << " and " << max << ", try again.\n";
            continue;
        }
        return value;
    }
}

std::string readLine(const std::string &prompt, size_t minLen) {
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
        if (value.length() >= minLen) return value;
        if (minLen == 1)
            std::cout << "Value cannot be empty, try again.\n";
        else
            std::cout << "Must be at least " << minLen << " characters, try again.\n";
    }
}

void printCentered(const std::string &text, int width) {
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding < 0) padding = 0;
    std::cout << std::string(padding, ' ') << text << std::endl;
}

void printStudentTable(const std::vector<Student> &list, size_t pageSize) {
    if (list.empty()) {
        std::cout << "No students to display.\n";
        return;
    }

    size_t total = list.size();
    size_t totalPages = (total + pageSize - 1) / pageSize;

    for (size_t page = 0; page < totalPages; ++page) {
        size_t start = page * pageSize;
        size_t end = std::min(start + pageSize, total);

        printTableHeader();
        for (size_t i = start; i < end; ++i)
            printTableRow(list[i]);

        if (totalPages > 1) {
            std::cout << "-- Page " << (page + 1) << "/" << totalPages;
            if (page + 1 < totalPages) {
                std::cout << " -- Press Enter for next page, q to stop: ";
                std::string input;
                std::getline(std::cin, input);
                if (!input.empty() && (input[0] == 'q' || input[0] == 'Q')) break;
            } else {
                std::cout << " --\n";
            }
        }
    }
}

} // namespace Menu
