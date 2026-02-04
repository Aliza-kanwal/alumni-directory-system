#include "alumni.h"

// Default constructor
Alumni::Alumni() = default;

// Map constructor
Alumni::Alumni(const QMap<QString, QVariant>& data) :
    name(data["name"].toString()),
    department(data["department"].toString()),
    graduationYear(data["graduation_year"].toString()),
    email(data["email"].toString()),
    linkedIn(data["linkedin_profile"].toString()),
    field(data["main_skills"].toString()),
    degree(data["qualifications"].toString())
{}

// Parameterized constructor
Alumni::Alumni(const QString &name, const QString &department,
               const QString &graduationYear, const QString &email,
               const QString &linkedIn, const QString &field,
               const QString &degree) :
    name(name), department(department),
    graduationYear(graduationYear), email(email),
    linkedIn(linkedIn), field(field), degree(degree)
{}

// Getter implementations
QString Alumni::getName() const { return name; }
QString Alumni::getDepartment() const { return department; }
QString Alumni::getGraduationYear() const { return graduationYear; }
QString Alumni::getEmail() const { return email; }
QString Alumni::getLinkedIn() const { return linkedIn; }
QString Alumni::getField() const { return field; }
QString Alumni::getDegree() const { return degree; }

// Setter implementations
void Alumni::setName(const QString &name) { this->name = name; }
void Alumni::setDepartment(const QString &dept) { department = dept; }
void Alumni::setGraduationYear(const QString &year) { graduationYear = year; }
void Alumni::setEmail(const QString &email) { this->email = email; }
void Alumni::setLinkedIn(const QString &linkedIn) { this->linkedIn = linkedIn; }
void Alumni::setField(const QString &field) { this->field = field; }
void Alumni::setDegree(const QString &degree) { this->degree = degree; }

// Conversion method
QMap<QString, QVariant> Alumni::toMap() const
{
        QMap<QString, QVariant> map;
        map["name"] = name;
        map["graduation_year"] = graduationYear;
        map["linkedin_profile"] = linkedIn;
        map["main_skills"] = field;  // Assuming 'field' corresponds to 'main_skills'
        map["qualifications"] = degree;
        return map;
    }
