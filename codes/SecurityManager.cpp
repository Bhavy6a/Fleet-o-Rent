#include <iostream>
#include <string>

class SecurityManager {
private:
    const std::string ADMIN_PIN = "9988"; // In a real app, this would be hashed

public:
    bool authenticate() {
        std::string input;
        std::cout << "\n[SECURITY CHECK]: Enter Admin PIN to proceed: ";
        std::cin >> input;
        
        if (input == ADMIN_PIN) {
            std::cout << "Access Granted.\n";
            return true;
        } else {
            std::cout << "Access Denied. Unauthorized action logged.\n";
            return false;
        }
    }

    // Example of a simple XOR "encryption" for sensitive strings
    std::string obscure(std::string data) {
        char key = 'K';
        for (int i = 0; i < data.size(); i++) data[i] ^= key;
        return data;
    }
};
