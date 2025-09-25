#pragma once
#include <string>

// Represents a single password entry
struct PasswordEntry {
    std::wstring url;       // URL of the service
    std::wstring login;     // Username or login
    std::wstring password;  // Password
    std::wstring note;      // Optional note
    std::wstring color;     // Optional color tag
};
