#pragma once
#include "GroupEntry.h"
#include <vector>
#include <string>

class PasswordManager {
private:
    std::vector<PasswordEntry> rootEntries; // Entries under Root
    std::vector<GroupEntry> groups;         // Top-level groups

    // Recursive helpers for nested groups
    GroupEntry* findGroupRecursive(std::vector<GroupEntry>& groups, const std::string& path);
    void listGroupRecursive(const GroupEntry& group, const std::string& prefix) const;

public:
    // Root entries
    void addRootEntry(const std::string& url,
        const std::string& login,
        const std::string& password,
        const std::string& note = "",
        const std::string& color = "");

    bool removeRootEntry(const std::string& url, const std::string& login);
    PasswordEntry* findRootEntry(const std::string& url, const std::string& login);

    // Groups
    void addGroup(const std::string& path); // e.g., "Work/Project"
    bool removeGroup(const std::string& path);
    GroupEntry* findGroup(const std::string& path);

    void addEntryToGroup(const std::string& path,
        const std::string& url,
        const std::string& login,
        const std::string& password,
        const std::string& note = "",
        const std::string& color = "");

    bool removeEntryFromGroup(const std::string& path,
        const std::string& url,
        const std::string& login);

    PasswordEntry* findEntryInGroup(const std::string& path,
        const std::string& url,
        const std::string& login);

    // Display
    void listAllEntries() const;
};
