#include "StudentRepository.h"
#include "StudentController.h"

int main() {
    StudentRepository repo("students.csv");
    repo.load();
    StudentController controller(repo);
    controller.run();
    return 0;
}
