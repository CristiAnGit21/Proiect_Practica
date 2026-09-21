#pragma once
#include <string>
#include <vector>
#include <optional>
#include "Student.h"

enum class SortField { Name = 1, Age = 2, Id = 3 };

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
    std::vector<Student> findByCourse(const std::string &query) const;

    void sortBy(SortField field);

    size_t count() const;
    double averageAge() const;

private:
    std::string filePath;
    std::vector<Student> students;
    int nextId = 1;

    int indexOfId(int id) const;
};
