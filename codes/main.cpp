#include "../include/DatabaseManager.h"
#include "../include/RentalService.h"
#include "../include/UserInterface.h"

int main() {
    // Instantiate Core Components
    DatabaseManager dbManager;
    RentalService rentalService(dbManager);
    UserInterface ui(rentalService);

    // Boot Application
    ui.mainLoop();

    return 0;
}
