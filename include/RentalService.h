#ifndef RENTAL_SERVICE_H
#define RENTAL_SERVICE_H

#include "DatabaseManager.h"
#include "Global.h"
#include <vector>
#include <string>

class RentalService {
private:
    DatabaseManager& db;
    int calculateDateDiff(std::string start, std::string end);

public:
    RentalService(DatabaseManager& dbMgr) : db(dbMgr) {}

    std::vector<Car> fetchCars(std::string filter = "");
    bool rentVehicle(std::string plate, std::string customer, std::string phone, std::string start, std::string end);
    bool returnVehicle(std::string plate);
    bool addNewCar(std::string brand, std::string model, std::string plate, double rate, std::string category);
};

#endif
