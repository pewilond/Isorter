#ifndef INTERFACE_H
#define INTERFACE_H
#include "string"

void run_interface();

int CompareInt(int a, int b);

int getValidatedInt(const std::string &prompt);

int getValidatedPositiveInt(const std::string &prompt);

int getValidatedIntInRange(const std::string &prompt, int minValue, int maxValue);

#endif // INTERFACE_H
