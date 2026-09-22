#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include "Student.h"

class StudentDialog : public QDialog {
    Q_OBJECT
public:
    explicit StudentDialog(QWidget *parent = nullptr);
    StudentDialog(QWidget *parent, const Student &student);

    std::string name()   const;
    int         age()    const;
    std::string course() const;

private:
    QLineEdit *nameEdit;
    QSpinBox  *ageSpin;
    QLineEdit *courseEdit;

    void setupUi(const QString &title);
    bool validate();
};
