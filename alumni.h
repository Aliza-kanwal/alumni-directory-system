#ifndef ALUMNI_H
#define ALUMNI_H

#include <QString>
#include <QMap>
#include <QVariant>  // Required for QVariant

class Alumni
{
public:
    Alumni();  // Default constructor
    Alumni(const QMap<QString, QVariant>& data);  // Map constructor
    Alumni(const QString &name, const QString &department,
           const QString &graduationYear, const QString &email,
           const QString &linkedIn, const QString &field,
           const QString &degree);  // Parameterized constructor

    // Getter methods
    QString getName() const;
    QString getDepartment() const;
    QString getGraduationYear() const;
    QString getEmail() const;
    QString getLinkedIn() const;  // Fixed typo in method name
    QString getField() const;     // Fixed casing
    QString getDegree() const;

    // Setter methods
    void setName(const QString &name);
    void setDepartment(const QString &department);
    void setGraduationYear(const QString &graduationYear);
    void setEmail(const QString &email);
    void setLinkedIn(const QString &linkedIn);  // Fixed typo
    void setField(const QString &field);        // Fixed casing
    void setDegree(const QString &degree);

    // Conversion method
    QMap<QString, QVariant> toMap() const;

private:
    QString name;
    QString department;
    QString graduationYear;
    QString email;
    QString linkedIn;  // Fixed member variable name
    QString field;
    QString degree;
};

#endif // ALUMNI_H
