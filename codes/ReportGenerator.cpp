#include "../include/RentalService.h"
#include <iostream>
#include <iomanip>
#include <map>

void generateFinancialReport(DatabaseManager& db) {
    std::cout << "\n" << std::string(40, '=') << "\n";
    std::cout << "     EXECUTIVE FINANCIAL SUMMARY\n";
    std::cout << std::string(40, '=') << "\n";

    sqlite3_stmt* stmt;
    std::string sql = "SELECT SUM(TotalBill), AVG(TotalBill), COUNT(*) FROM Rents;";
    
    if (sqlite3_prepare_v2(db.getHandle(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            double totalRev = sqlite3_column_double(stmt, 0);
            double avgBill = sqlite3_column_double(stmt, 1);
            int totalRents = sqlite3_column_int(stmt, 2);

            std::cout << std::left << std::setw(25) << "Total Revenue:" << "$" << totalRev << "\n";
            std::cout << std::left << std::setw(25) << "Average Transaction:" << "$" << avgBill << "\n";
            std::cout << std::left << std::setw(25) << "Total Bookings:" << totalRents << "\n";
        }
    }
    sqlite3_finalize(stmt);

    // Fleet Utilization Logic
    std::cout << "\n[Fleet Utilization by Category]\n";
    std::string catSql = "SELECT Category, COUNT(*) FROM Cars GROUP BY Category;";
    if (sqlite3_prepare_v2(db.getHandle(), catSql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string cat = (const char*)sqlite3_column_text(stmt, 0);
            int count = sqlite3_column_int(stmt, 1);
            std::cout << " > " << std::setw(10) << cat << ": " << count << " vehicles\n";
        }
    }
    sqlite3_finalize(stmt);
}
