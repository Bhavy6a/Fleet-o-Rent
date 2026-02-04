void seedDatabase(DatabaseManager& db) {
    std::string seeds[] = {
        "INSERT OR IGNORE INTO Cars (Brand, Model, Plate, Rate, Status, Category) VALUES ('Tesla', 'Model S', 'TSLA-001', 150.0, 'Available', 'Luxury');",
        "INSERT OR IGNORE INTO Cars (Brand, Model, Plate, Rate, Status, Category) VALUES ('Ford', 'Mustang', 'MUS-66', 120.0, 'Available', 'Sports');",
        "INSERT OR IGNORE INTO Cars (Brand, Model, Plate, Rate, Status, Category) VALUES ('Toyota', 'Camry', 'CAM-2024', 60.0, 'Available', 'Sedan');",
        "INSERT OR IGNORE INTO Cars (Brand, Model, Plate, Rate, Status, Category) VALUES ('Range Rover', 'Sport', 'RR-LUX', 200.0, 'Available', 'SUV');"
    };

    std::cout << "[SEEDING]: Populating initial fleet data...\n";
    for (const std::string& sql : seeds) {
        db.execute(sql);
    }
    std::cout << "[SEEDING]: 4 Default vehicles added.\n";
}
