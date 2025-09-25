#pragma once
#include <string>

// Represents a single password entry
struct PasswordEntry {
    std::string url;       // URL of the service
    std::string login;     // Username or login
    std::string password;  // Password
    std::string note;      // Optional note
    std::string color;     // Optional color tag
};
