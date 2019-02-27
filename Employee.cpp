#include "Employee.h"

Employee::Employee() {}

Employee::Employee(const Employee &other) {
    isBusy=other.isBusy;
}