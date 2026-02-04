#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "RentalService.h"
#include <vector>

class UserInterface {
private:
    RentalService& service;
    void clearBuffer();
    void handleChoice(int choice);
    
    // Sub-menus
    void menuAddCar();
    void menuProcessRental();
    void menuSearch();

public:
    UserInterface(RentalService& s) : service(s) {}

    void drawTable(const std::vector<Car>& cars);
    void mainLoop();
};

#endif
