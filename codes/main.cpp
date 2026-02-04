#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sqlite3.h>
#include <algorithm>
#include <limits>
#include <ctime>
#include <sstream>
#include <memory>
#include <regex>

using namespace std;

// ==========================================
// 1. CONSTANTS AND UTILS
// ==========================================
namespace Global {
    const string DB_NAME = "fleet_management.db";
    const int COLUMN_WIDTH = 15;
}

class Logger {
public:
    static void log(string msg) { cout << "[LOG]: " << msg << endl; }
    static void error(string msg) { cerr << "[ERROR]: " << msg << endl; }
};

class Validator {
public:
    static bool isDate(const string& date) {
        regex pattern(R"(\d{4}-\d{2}-\d{2})");
        return regex_match(date, pattern);
    }
    static bool isPhone(const string& phone) {
        regex pattern(R"(\d{10,12})");
        return regex_match(phone, pattern);
    }
};

// ==========================================
// 2. DATA MODELS
// ==========================================
struct Car {
    int id;
    string brand;
    string model;
    string plate;
    double dailyRate;
    string status; // Available, Rented, Maintenance
    string category; // Sedan, SUV, Luxury
};

struct Renter {
    string name;
    string phone;
    string license;
};

struct Transaction {
    int id;
    string plate;
    string renterName;
    string startDate;
    string endDate;
    int totalDays;
    double totalBill;
};

// ==========================================
// 3. DATABASE ENGINE (SQLite Wrapper)
// ==========================================
class DatabaseManager {
private:
    sqlite3* db;
    char* errMsg = nullptr;

public:
    DatabaseManager() {
        if (sqlite3_open(Global::DB_NAME.c_str(), &db)) {
            Logger::error("Failed to open DB.");
        } else {
            createTables();
        }
    }

    ~DatabaseManager() {
        sqlite3_close(db);
    }

    void createTables() {
        string carTable = "CREATE TABLE IF NOT EXISTS Cars ("
                          "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "Brand TEXT, Model TEXT, Plate TEXT UNIQUE, "
                          "Rate REAL, Status TEXT, Category TEXT);";

        string rentTable = "CREATE TABLE IF NOT EXISTS Rents ("
                           "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "Plate TEXT, RenterName TEXT, Phone TEXT, "
                           "StartDate TEXT, EndDate TEXT, TotalDays INTEGER, TotalBill REAL);";

        execute(carTable);
        execute(rentTable);
    }

    bool execute(string sql) {
        int res = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (res != SQLITE_OK) {
            Logger::error(errMsg);
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }

    sqlite3* handle() { return db; }
};

// ==========================================
// 4. CORE SERVICE LAYER
// ==========================================
class RentalService {
private:
    DatabaseManager& dbManager;

    static int carLoadCallback(void* data, int argc, char** argv, char** colNames) {
        auto* list = static_cast<vector<Car>*>(data);
        Car c;
        c.id = stoi(argv[0]);
        c.brand = argv[1];
        c.model = argv[2];
        c.plate = argv[3];
        c.dailyRate = stod(argv[4]);
        c.status = argv[5];
        c.category = argv[6];
        list->push_back(c);
        return 0;
    }

    int calculateDays(string start, string end) {
        struct tm tm1 = {0}, tm2 = {0};
        stringstream ss1(start), ss2(end);
        char dash;
        ss1 >> tm1.tm_year >> dash >> tm1.tm_mon >> dash >> tm1.tm_mday;
        ss2 >> tm2.tm_year >> dash >> tm2.tm_mon >> dash >> tm2.tm_mday;
        tm1.tm_year -= 1900; tm1.tm_mon -= 1;
        tm2.tm_year -= 1900; tm2.tm_mon -= 1;
        time_t t1 = mktime(&tm1);
        time_t t2 = mktime(&tm2);
        return difftime(t2, t1) / (60 * 60 * 24);
    }

public:
    RentalService(DatabaseManager& mgr) : dbManager(mgr) {}

    void registerCar() {
        string b, m, p, cat;
        double r;
        cout << "\n--- New Fleet Entry ---\n";
        cout << "Brand: "; getline(cin, b);
        cout << "Model: "; getline(cin, m);
        cout << "Plate: "; getline(cin, p);
        cout << "Category (Sedan/SUV/Luxury): "; getline(cin, cat);
        cout << "Daily Rate: "; cin >> r; cin.ignore();

        string sql = "INSERT INTO Cars (Brand, Model, Plate, Rate, Status, Category) VALUES "
                     "('" + b + "', '" + m + "', '" + p + "', " + to_string(r) + ", 'Available', '" + cat + "');";
        
        if (dbManager.execute(sql)) cout << "Successfully registered car.\n";
    }

    vector<Car> getAllCars(string filter = "") {
        vector<Car> cars;
        string sql = "SELECT * FROM Cars " + filter + ";";
        sqlite3_exec(dbManager.handle(), sql.c_str(), carLoadCallback, &cars, nullptr);
        return cars;
    }

    void processRental() {
        string plate, name, phone, sDate, eDate;
        cout << "\n--- Initiate Rental Transaction ---\n";
        cout << "Car Plate: "; getline(cin, plate);
        
        auto fleet = getAllCars("WHERE Plate = '" + plate + "' AND Status = 'Available'");
        if (fleet.empty()) {
            cout << "Car unavailable or not found.\n";
            return;
        }

        cout << "Customer Name: "; getline(cin, name);
        cout << "Contact Number: "; getline(cin, phone);
        while (true) {
            cout << "Start Date (YYYY-MM-DD): "; getline(cin, sDate);
            cout << "End Date (YYYY-MM-DD): "; getline(cin, eDate);
            if (Validator::isDate(sDate) && Validator::isDate(eDate)) break;
            cout << "Invalid format. Retry.\n";
        }

        int days = calculateDays(sDate, eDate);
        if (days <= 0) days = 1;
        double total = days * fleet[0].dailyRate;

        string sqlRent = "INSERT INTO Rents (Plate, RenterName, Phone, StartDate, EndDate, TotalDays, TotalBill) VALUES "
                         "('" + plate + "', '" + name + "', '" + phone + "', '" + sDate + "', '" + eDate + "', " + to_string(days) + ", " + to_string(total) + ");";
        string sqlUpdate = "UPDATE Cars SET Status = 'Rented' WHERE Plate = '" + plate + "';";

        dbManager.execute("BEGIN TRANSACTION;");
        if (dbManager.execute(sqlRent) && dbManager.execute(sqlUpdate)) {
            dbManager.execute("COMMIT;");
            cout << "Transaction Complete. Total Bill: $" << total << endl;
        } else {
            dbManager.execute("ROLLBACK;");
        }
    }

    void returnCar() {
        string plate;
        cout << "Enter Plate for Return: "; getline(cin, plate);
        string sql = "UPDATE Cars SET Status = 'Available' WHERE Plate = '" + plate + "';";
        if (dbManager.execute(sql)) cout << "Car status restored.\n";
    }
};

// ==========================================
// 5. VIEW / UI LAYER
// ==========================================
class UserInterface {
private:
    RentalService& service;

public:
    UserInterface(RentalService& s) : service(s) {}

    void drawHeader() {
        cout << "\n\t" << string(50, '=') << endl;
        cout << "\t      UNIVERSAL CAR RENTAL SYSTEM v2.0" << endl;
        cout << "\t" << string(50, '=') << endl;
    }

    void displayFleet() {
        auto cars = service.getAllCars();
        cout << "\n" << left << setw(5) << "ID" << setw(15) << "Brand" << setw(15) << "Plate" << setw(15) << "Category" << setw(10) << "Price" << "Status" << endl;
        cout << string(75, '-') << endl;
        for (const auto& c : cars) {
            cout << left << setw(5) << c.id << setw(15) << c.brand << setw(15) << c.plate << setw(15) << c.category << "$" << setw(9) << c.dailyRate << c.status << endl;
        }
    }

    void run() {
        int choice;
        while (true) {
            drawHeader();
            cout << "1. Display Fleet\n2. Add New Vehicle\n3. Process Rental\n4. Return Vehicle\n5. Search Inventory\n0. Exit\nSelection: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore();

            if (choice == 0) break;
            switch (choice) {
                case 1: displayFleet(); break;
                case 2: service.registerCar(); break;
                case 3: service.processRental(); break;
                case 4: service.returnCar(); break;
                case 5: {
                    string s; cout << "Keyword: "; getline(cin, s);
                    auto results = service.getAllCars("WHERE Brand LIKE '%" + s + "%' OR Plate LIKE '%" + s + "%'");
                    // Logic to display results...
                    break;
                }
            }
            cout << "\nPress Enter to return to menu...";
            cin.get();
        }
    }
};

// ==========================================
// 6. MAIN CONTROLLER
// ==========================================
int main() {
    DatabaseManager db;
    RentalService service(db);
    UserInterface ui(service);

    ui.run();

    return 0;
}
