#include "../include/UserInterface.h"
#include <iostream>
#include <iomanip>
#include <limits>

void UserInterface::clearBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UserInterface::drawTable(const std::vector<Car>& cars) {
    std::cout << "\n" << std::setfill('=') << std::setw(90) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left 
              << std::setw(8)  << "ID" 
              << std::setw(15) << "Brand" 
              << std::setw(12) << "Model" 
              << std::setw(15) << "Plate" 
              << std::setw(12) << "Category" 
              << std::setw(12) << "Rate/Day" 
              << "Status" << std::endl;
    std::cout << std::setfill('-') << std::setw(90) << "" << std::setfill(' ') << std::endl;

    for (const auto& c : cars) {
        std::cout << std::left 
                  << std::setw(8)  << c.id 
                  << std::setw(15) << c.brand 
                  << std::setw(12) << c.model 
                  << std::setw(15) << c.plate 
                  << std::setw(12) << c.category 
                  << "$" << std::setw(11) << std::fixed << std::setprecision(2) << c.dailyRate 
                  << c.status << std::endl;
    }
    std::cout << std::setfill('=') << std::setw(90) << "" << std::endl << std::endl;
}

void UserInterface::mainLoop() {
    int choice;
    while (true) {
        std::cout << "--- ELITE FLEET MANAGEMENT SYSTEM ---\n";
        std::cout << "1. View All Cars\n2. Register New Car\n3. Process Rental\n4. Return Vehicle\n5. Search Inventory\n0. Exit\nSelection: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearBuffer();
            continue;
        }
        clearBuffer();

        if (choice == 0) break;
        handleChoice(choice);
    }
}
