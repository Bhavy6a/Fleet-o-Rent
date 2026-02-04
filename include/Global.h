#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>

namespace Global {
    const std::string DB_NAME = "fleet_management.db";
}

struct Car {
    int id;
    std::string brand;
    std::string model;
    std::string plate;
    double dailyRate;
    std::string status;
    std::string category;
};

struct RentRecord {
    int id;
    std::string plate;
    std::string customerName;
    std::string contact;
    std::string startDate;
    std::string endDate;
    int totalDays;
    double totalBill;
};

#endif
