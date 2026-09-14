#pragma once
#include <string>
#include <vector>
#include <optional>
#include "Student.h"

class StudentRepository {
public:
    explicit StudentRepository(std::string filePath);

    bool load();
    bool save() const;

    int insert(const std::string &name, int age, const std::string &course);
    bool update(int id, const std::string &name, int age, const std::string &course);
    bool remove(int id);

    const std::vector<Student> &getAll() const;
    std::optional<Student> findById(int id) const;
    std::vector<Student> findByName(const std::string &query) const;

    void sortByName();

    size_t count() const;
    double averageAge() const;

private:
    std::string filePath;
    std::vector<Student> students;
    int nextId = 1;

    int indexOfId(int id) const;

    static std::string escapeCsvField(const std::string &field);
    static std::vector<std::string> parseCsvLine(const std::string &line);
};
