#include "StudentDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>

StudentDialog::StudentDialog(QWidget *parent) : QDialog(parent) { setupUi("Add Student"); }

StudentDialog::StudentDialog(QWidget *parent, const Student &student) : QDialog(parent) {
    setupUi("Edit Student");
    nameEdit->setText(QString::fromStdString(student.name));
    ageSpin->setValue(student.age);
    courseEdit->setText(QString::fromStdString(student.course));
}

void StudentDialog::setupUi(const QString &title) {
    setWindowTitle(title);
    setMinimumWidth(300);

    nameEdit   = new QLineEdit;
    ageSpin    = new QSpinBox;
    courseEdit = new QLineEdit;

    ageSpin->setRange(16, 100);
    ageSpin->setValue(18);

    auto *form = new QFormLayout;
    form->addRow("Name:",   nameEdit);
    form->addRow("Age:",    ageSpin);
    form->addRow("Course:", courseEdit);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]{ if (validate()) accept(); });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
}

bool StudentDialog::validate() {
    if (nameEdit->text().trimmed().length() < 2) {
        QMessageBox::warning(this, "Validation", "Name must be at least 2 characters.");
        nameEdit->setFocus(); return false;
    }
    if (courseEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Course cannot be empty.");
        courseEdit->setFocus(); return false;
    }
    return true;
}

std::string StudentDialog::name()   const { return nameEdit->text().trimmed().toStdString(); }
int         StudentDialog::age()    const { return ageSpin->value(); }
std::string StudentDialog::course() const { return courseEdit->text().trimmed().toStdString(); }
