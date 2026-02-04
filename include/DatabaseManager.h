#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>

class DatabaseManager {
private:
    sqlite3* db;
    void initializeSchema();

public:
    DatabaseManager();
    ~DatabaseManager();

    bool execute(std::string sql);
    sqlite3* getHandle();
};

#endif
