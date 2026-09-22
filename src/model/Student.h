#pragma once
#include <string>
#include <vector>

struct Student {
    int id;
    std::string name;
    int age;
    std::string course;
    std::vector<double> grades;

    double averageGrade() const {
        if (grades.empty()) return 0.0;
        double sum = 0.0;
        for (double g : grades) sum += g;
        return sum / static_cast<double>(grades.size());
    }
};
