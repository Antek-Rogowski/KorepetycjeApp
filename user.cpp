#include "user.h"

// --- Implementacja Klasy Bazowej User ---
User::User(const QString& id, const QString& name)
    : id(id), name(name), availability(id) {}

QString User::getId() const {
    return id;
}

QString User::getName() const {
    return name;
}

UserAvailability& User::getAvailability() {
    return availability;
}

const UserAvailability& User::getAvailability() const {
    return availability;
}


// --- Implementacja Klasy Student ---
Student::Student(const QString& id, const QString& name, const QString& gradeLevel)
    : User(id, name), gradeLevel(gradeLevel) {}

QString Student::getGradeLevel() const {
    return gradeLevel;
}

QString Student::getRole() const {
    return "Uczeń";
}


// --- Implementacja Klasy Tutor ---
Tutor::Tutor(const QString& id, const QString& name, const QString& specialization)
    : User(id, name), specialization(specialization) {}

QString Tutor::getSpecialization() const {
    return specialization;
}

QString Tutor::getRole() const {
    return "Korepetytor";
}