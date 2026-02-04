#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

class Validator {
public:
    static bool isValidPlate(const std::string& plate);
    static bool isValidDate(const std::string& date);
    static bool isValidPrice(double price);
    static bool isValidPhone(const std::string& phone);
};

#endif
