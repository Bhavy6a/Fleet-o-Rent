#include "../include/DatabaseManager.h"
#include "../include/Global.h"
#include <iostream>

DatabaseManager::DatabaseManager() {
    int exit = sqlite3_open(Global::DB_NAME.c_str(), &db);
    if (exit) {
        std::cerr << "[CRITICAL]: Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "[SYSTEM]: Database Connection Established." << std::endl;
        initializeSchema();
    }
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(db);
    std::cout << "[SYSTEM]: Database Connection Closed." << std::endl;
}

void DatabaseManager::initializeSchema() {
    std::string carsTable = 
        "CREATE TABLE IF NOT EXISTS Cars ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Brand TEXT NOT NULL, "
        "Model TEXT NOT NULL, "
        "Plate TEXT UNIQUE NOT NULL, "
        "Rate REAL NOT NULL, "
        "Status TEXT DEFAULT 'Available', "
        "Category TEXT NOT NULL);";

    std::string rentsTable = 
        "CREATE TABLE IF NOT EXISTS Rents ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Plate TEXT NOT NULL, "
        "CustomerName TEXT NOT NULL, "
        "Contact TEXT NOT NULL, "
        "StartDate TEXT NOT NULL, "
        "EndDate TEXT NOT NULL, "
        "TotalDays INTEGER, "
        "TotalBill REAL, "
        "FOREIGN KEY(Plate) REFERENCES Cars(Plate));";

    execute(carsTable);
    execute(rentsTable);
}

bool DatabaseManager::execute(std::string sql) {
    char* messageError;
    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "[SQL ERROR]: " << messageError << std::endl;
        sqlite3_free(messageError);
        return false;
    }
    return true;
}

sqlite3* DatabaseManager::getHandle() {
    return db;
}
