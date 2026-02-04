#include "../include/Validator.h"
#include <regex>

bool Validator::isValidPlate(const std::string& plate) {
    // Basic regex for standard license plates
    std::regex pattern("^[A-Z0-9-]{3,10}$");
    return std::regex_match(plate, pattern);
}

bool Validator::isValidDate(const std::string& date) {
    // Format YYYY-MM-DD
    std::regex pattern(R"(\d{4}-\d{2}-\d{2})");
    return std::regex_match(date, pattern);
}

bool Validator::isValidPrice(double price) {
    return price > 0 && price < 10000;
}
