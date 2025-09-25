#pragma once
#include "PasswordEntry.h"
#include <string>
#include <vector>

// Represents a group of password entries, supports nested groups
struct GroupEntry {
    std::string groupName;
    std::vector<PasswordEntry> entries;
    std::vector<GroupEntry> subGroups;
};
