#include "StudentRepository.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

StudentRepository::StudentRepository(std::string filePath) : filePath(std::move(filePath)) {}

std::string StudentRepository::escapeCsvField(const std::string &field) {
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

std::vector<std::string> StudentRepository::parseCsvLine(const std::string &line) {
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
            if (line.rfind("id,", 0) == 0) continue;
        }

        std::vector<std::string> fields = parseCsvLine(line);
        if (fields.size() < 4) continue;

        Student s;
        try {
            s.id = std::stoi(fields[0]);
            s.name = fields[1];
            s.age = std::stoi(fields[2]);
            s.course = fields[3];
        } catch (...) {
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
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
                    [](unsigned char c) { return std::tolower(c); });

    std::vector<Student> results;
    for (const auto &s : students) {
        std::string lowerName = s.name;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                        [](unsigned char c) { return std::tolower(c); });
        if (lowerName.find(lowerQuery) != std::string::npos) {
            results.push_back(s);
        }
    }
    return results;
}

void StudentRepository::sortByName() {
    std::sort(students.begin(), students.end(),
              [](const Student &a, const Student &b) { return a.name < b.name; });
    save();
}

size_t StudentRepository::count() const {
    return students.size();
}

double StudentRepository::averageAge() const {
    if (students.empty()) return 0.0;
    long sum = 0;
    for (const auto &s : students) sum += s.age;
    return static_cast<double>(sum) / static_cast<double>(students.size());
}
