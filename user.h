#ifndef USER_H
#define USER_H

#include <QString>
#include "useravailability.h"

// Klasa abstrakcyjna (Abstrakcja)
class User {
private:
    QString id;
    QString name;
    UserAvailability availability; // Hermetyzacja - obiekt dostępności wewnątrz użytkownika

public:
    User(const QString& id, const QString& name);
    virtual ~User() = default; // Wirtualny destruktor jest kluczowy przy polimorfizmie!

    // Gettery (Hermetyzacja)
    QString getId() const;
    QString getName() const;

    // Zwracamy referencję, aby móc modyfikować kalendarz danego użytkownika
    UserAvailability& getAvailability();
    const UserAvailability& getAvailability() const;

    // Metoda czysto wirtualna - każda klasa pochodna MUSI ją zaimplementować
    virtual QString getRole() const = 0;
};

// Klasa pochodna: Uczeń (Dziedziczenie)
class Student : public User {
private:
    QString gradeLevel; // np. "Liceum", "Studia"

public:
    Student(const QString& id, const QString& name, const QString& gradeLevel);

    QString getGradeLevel() const;
    QString getRole() const override; // Nadpisanie metody wirtualnej (Polimorfizm)
};

// Klasa pochodna: Korepetytor (Dziedziczenie)
class Tutor : public User {
private:
    QString specialization; // np. "Matematyka", "Programowanie C++"

public:
    Tutor(const QString& id, const QString& name, const QString& specialization);

    QString getSpecialization() const;
    QString getRole() const override; // Nadpisanie metody wirtualnej (Polimorfizm)
};

#endif // USER_H