#include "StudentRepository.h"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
    constexpr size_t EXPECTED_CSV_FIELDS = 4;

    std::string escapeCsvField(const std::string &field) {
        bool needsQuotes = field.find_first_of(",\"\n") != std::string::npos;
        if (!needsQuotes) return field;

        std::string escaped = "\"";
        for (char c : field) {
            if (c == '"') escaped += "\"\"";
            else escaped += c;
        }
        escaped += "\"";
        return escaped;
    }

    std::vector<std::string> parseCsvLine(const std::string &line) {
        std::vector<std::string> fields;
        std::string current;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (inQuotes) {
                if (c == '"') {
                    if (i + 1 < line.size() && line[i + 1] == '"') {
                        current += '"';
                        ++i;
                    } else {
                        inQuotes = false;
                    }
                } else {
                    current += c;
                }
            } else {
                if (c == '"') {
                    inQuotes = true;
                } else if (c == ',') {
                    fields.push_back(current);
                    current.clear();
                } else {
                    current += c;
                }
            }
        }
        fields.push_back(current);
        return fields;
    }

    std::string toLower(const std::string &s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }
}

StudentRepository::StudentRepository(std::string filePath) : filePath(std::move(filePath)) {}

bool StudentRepository::load() {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    students.clear();
    nextId = 1;

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        if (firstLine) {
            firstLine = false;
            if (line.find("id,") == 0) continue;
        }

        std::vector<std::string> fields = parseCsvLine(line);
        if (fields.size() < EXPECTED_CSV_FIELDS) continue;

        Student s;
        try {
            s.id = std::stoi(fields[0]);
            s.name = fields[1];
            s.age = std::stoi(fields[2]);
            s.course = fields[3];
        } catch (const std::exception &e) {
            std::cerr << "Warning: skipping malformed CSV line: " << e.what() << "\n";
            continue;
        }

        students.push_back(s);
        if (s.id >= nextId) nextId = s.id + 1;
    }

    return true;
}

bool StudentRepository::save() const {
    std::ofstream file(filePath, std::ios::trunc);
    if (!file.is_open()) return false;

    file << "id,name,age,course\n";
    for (const auto &s : students) {
        file << s.id << ","
             << escapeCsvField(s.name) << ","
             << s.age << ","
             << escapeCsvField(s.course) << "\n";
    }
    return true;
}

int StudentRepository::indexOfId(int id) const {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].id == id) return static_cast<int>(i);
    }
    return -1;
}

int StudentRepository::insert(const std::string &name, int age, const std::string &course) {
    Student s{nextId, name, age, course};
    students.push_back(s);
    ++nextId;
    save();
    return s.id;
}

bool StudentRepository::update(int id, const std::string &name, int age, const std::string &course) {
    int idx = indexOfId(id);
    if (idx == -1) return false;

    students[idx].name = name;
    students[idx].age = age;
    students[idx].course = course;
    save();
    return true;
}

bool StudentRepository::remove(int id) {
    int idx = indexOfId(id);
    if (idx == -1) return false;

    students.erase(students.begin() + idx);
    save();
    return true;
}

const std::vector<Student> &StudentRepository::getAll() const {
    return students;
}

std::optional<Student> StudentRepository::findById(int id) const {
    int idx = indexOfId(id);
    if (idx == -1) return std::nullopt;
    return students[idx];
}

std::vector<Student> StudentRepository::findByName(const std::string &query) const {
    std::string lowerQuery = toLower(query);
    std::vector<Student> results;
    for (const auto &s : students) {
        if (toLower(s.name).find(lowerQuery) != std::string::npos)
            results.push_back(s);
    }
    return results;
}

std::vector<Student> StudentRepository::findByCourse(const std::string &query) const {
    std::string lowerQuery = toLower(query);
    std::vector<Student> results;
    for (const auto &s : students) {
        if (toLower(s.course).find(lowerQuery) != std::string::npos)
            results.push_back(s);
    }
    return results;
}

void StudentRepository::sortBy(SortField field) {
    switch (field) {
        case SortField::Name:
            std::sort(students.begin(), students.end(),
                      [](const Student &a, const Student &b) { return a.name < b.name; });
            break;
        case SortField::Age:
            std::sort(students.begin(), students.end(),
                      [](const Student &a, const Student &b) { return a.age < b.age; });
            break;
        case SortField::Id:
            std::sort(students.begin(), students.end(),
                      [](const Student &a, const Student &b) { return a.id < b.id; });
            break;
    }
    save();
}

size_t StudentRepository::count() const {
    return students.size();
}

double StudentRepository::averageAge() const {
    if (students.empty()) return 0.0;
    int64_t sum = 0;
    for (const auto &s : students) sum += s.age;
    return static_cast<double>(sum) / static_cast<double>(students.size());
}
