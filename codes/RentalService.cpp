#include "../include/RentalService.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>

int RentalService::calculateDateDiff(std::string start, std::string end) {
    struct tm tm_start = {0}, tm_end = {0};
    std::istringstream ss_s(start), ss_e(end);
    char sep;
    
    ss_s >> tm_start.tm_year >> sep >> tm_start.tm_mon >> sep >> tm_start.tm_mday;
    ss_e >> tm_end.tm_year >> sep >> tm_end.tm_mon >> sep >> tm_end.tm_mday;

    tm_start.tm_year -= 1900; tm_start.tm_mon -= 1;
    tm_end.tm_year -= 1900; tm_end.tm_mon -= 1;

    time_t time_s = mktime(&tm_start);
    time_t time_e = mktime(&tm_end);

    double seconds = difftime(time_e, time_s);
    int days = seconds / (60 * 60 * 24);
    return (days <= 0) ? 1 : days;
}

static int carCallback(void* data, int argc, char** argv, char** azColName) {
    auto* carList = static_cast<std::vector<Car>*>(data);
    Car c;
    c.id = std::stoi(argv[0]);
    c.brand = argv[1];
    c.model = argv[2];
    c.plate = argv[3];
    c.dailyRate = std::stod(argv[4]);
    c.status = argv[5];
    c.category = argv[6];
    carList->push_back(c);
    return 0;
}

std::vector<Car> RentalService::fetchCars(std::string filter) {
    std::vector<Car> carList;
    std::string query = "SELECT * FROM Cars " + filter + ";";
    sqlite3_exec(db.getHandle(), query.c_str(), carCallback, &carList, NULL);
    return carList;
}

bool RentalService::rentVehicle(std::string plate, std::string customer, std::string phone, std::string start, std::string end) {
    auto available = fetchCars("WHERE Plate = '" + plate + "' AND Status = 'Available'");
    if (available.empty()) return false;

    int days = calculateDateDiff(start, end);
    double total = days * available[0].dailyRate;

    db.execute("BEGIN TRANSACTION;");
    
    std::string sqlRent = "INSERT INTO Rents (Plate, CustomerName, Contact, StartDate, EndDate, TotalDays, TotalBill) VALUES ('"
                          + plate + "', '" + customer + "', '" + phone + "', '" + start + "', '" + end + "', " 
                          + std::to_string(days) + ", " + std::to_string(total) + ");";
    
    std::string sqlUpdate = "UPDATE Cars SET Status = 'Rented' WHERE Plate = '" + plate + "';";

    if (db.execute(sqlRent) && db.execute(sqlUpdate)) {
        db.execute("COMMIT;");
        std::cout << "\n[BILLING]: Transaction Success. Total: $" << std::fixed << std::setprecision(2) << total << std::endl;
        return true;
    } else {
        db.execute("ROLLBACK;");
        return false;
    }
}
