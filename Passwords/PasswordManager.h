#pragma once
#include "GroupEntry.h"
#include <vector>
#include <string>

class PasswordManager {
private:
    std::vector<PasswordEntry> rootEntries; // Entries under Root
    std::vector<GroupEntry> groups;         // Top-level groups

    // Recursive helpers for nested groups
    GroupEntry* findGroupRecursive(std::vector<GroupEntry>& groups, const std::wstring& path);
    void listGroupRecursive(const GroupEntry& group, const std::wstring& prefix, bool isLast) const;

public:
    // Root entries
    void addRootEntry(const std::wstring& url,
        const std::wstring& login,
        const std::wstring& password,
        const std::wstring& note = L"",
        const std::wstring& color = L"");

    bool removeRootEntry(const std::wstring& url, const std::wstring& login);
    PasswordEntry* findRootEntry(const std::wstring& url, const std::wstring& login);

    // Groups
    void addGroup(const std::wstring& path); // e.g., L"Work/Project"
    bool removeGroup(const std::wstring& path);
    GroupEntry* findGroup(const std::wstring& path);

    void addEntryToGroup(const std::wstring& path,
        const std::wstring& url,
        const std::wstring& login,
        const std::wstring& password,
        const std::wstring& note = L"",
        const std::wstring& color = L"");

    bool removeEntryFromGroup(const std::wstring& path,
        const std::wstring& url,
        const std::wstring& login);

    PasswordEntry* findEntryInGroup(const std::wstring& path,
        const std::wstring& url,
        const std::wstring& login);

    // Display
    void listAllEntries() const;
};
