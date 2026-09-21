#pragma once
#include <limits>
#include <string>
#include <vector>
#include "Student.h"

namespace Menu {
    int readInt(const std::string &prompt,
                int min = std::numeric_limits<int>::min(),
                int max = std::numeric_limits<int>::max());

    std::string readLine(const std::string &prompt, size_t minLen = 1);

    void printCentered(const std::string &text, int width);
    void printStudentTable(const std::vector<Student> &list, size_t pageSize = 10);
}
