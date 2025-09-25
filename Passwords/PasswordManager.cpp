#include "PasswordManager.h"
#include <iostream>
#include <sstream>
#include <windows.h>

// --- Helper: set console color ---
void setConsoleColor(const std::wstring& color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (color == L"red") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }
    else if (color == L"green") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
    else if (color == L"blue") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
    else if (color == L"yellow") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
    else {
        // default color
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

// --- Root entries ---
void PasswordManager::addRootEntry(const std::wstring& url,
    const std::wstring& login,
    const std::wstring& password,
    const std::wstring& note,
    const std::wstring& color) {
    rootEntries.push_back({ url, login, password, note, color });
}

bool PasswordManager::removeRootEntry(const std::wstring& url, const std::wstring& login) {
    for (auto it = rootEntries.begin(); it != rootEntries.end(); ++it) {
        if (it->url == url && it->login == login) {
            rootEntries.erase(it);
            return true;
        }
    }
    return false;
}

PasswordEntry* PasswordManager::findRootEntry(const std::wstring& url, const std::wstring& login) {
    for (auto& entry : rootEntries) {
        if (entry.url == url && entry.login == login) return &entry;
    }
    return nullptr;
}

// --- Helper to split path ---
static std::vector<std::wstring> splitPath(const std::wstring& path) {
    std::vector<std::wstring> parts;
    std::wstringstream ss(path);
    std::wstring item;
    while (std::getline(ss, item, L'/')) {
        if (!item.empty()) parts.push_back(item);
    }
    return parts;
}

// --- Recursive find group ---
GroupEntry* PasswordManager::findGroupRecursive(std::vector<GroupEntry>& groupsVec, const std::wstring& path) {
    auto parts = splitPath(path);
    std::vector<GroupEntry>* current = &groupsVec;
    GroupEntry* group = nullptr;

    for (auto& part : parts) {
        group = nullptr;
        for (auto& g : *current) {
            if (g.groupName == part) {
                group = &g;
                current = &g.subGroups;
                break;
            }
        }
        if (!group) return nullptr;
    }
    return group;
}

GroupEntry* PasswordManager::findGroup(const std::wstring& path) {
    return findGroupRecursive(groups, path);
}

// --- Add group with nested support ---
void PasswordManager::addGroup(const std::wstring& path) {
    auto parts = splitPath(path);
    std::vector<GroupEntry>* current = &groups;
    GroupEntry* group = nullptr;

    for (auto& part : parts) {
        group = nullptr;
        for (auto& g : *current) {
            if (g.groupName == part) {
                group = &g;
                break;
            }
        }
        if (!group) { // create new if not exist
            current->push_back({ part, {}, {} });
            group = &current->back();
        }
        current = &group->subGroups;
    }
}

bool PasswordManager::removeGroup(const std::wstring& path) {
    auto parts = splitPath(path);
    if (parts.empty()) return false;

    std::vector<GroupEntry>* current = &groups;
    for (size_t i = 0; i < parts.size() - 1; ++i) {
        GroupEntry* g = nullptr;
        for (auto& grp : *current) {
            if (grp.groupName == parts[i]) {
                g = &grp;
                current = &g->subGroups;
                break;
            }
        }
        if (!g) return false;
    }

    for (auto it = current->begin(); it != current->end(); ++it) {
        if (it->groupName == parts.back()) {
            current->erase(it);
            return true;
        }
    }
    return false;
}

// --- Add entry to group ---
void PasswordManager::addEntryToGroup(const std::wstring& path,
    const std::wstring& url,
    const std::wstring& login,
    const std::wstring& password,
    const std::wstring& note,
    const std::wstring& color) {
    GroupEntry* group = findGroup(path);
    if (group) {
        group->entries.push_back({ url, login, password, note, color });
    }
}

bool PasswordManager::removeEntryFromGroup(const std::wstring& path,
    const std::wstring& url,
    const std::wstring& login) {
    GroupEntry* group = findGroup(path);
    if (group) {
        for (auto it = group->entries.begin(); it != group->entries.end(); ++it) {
            if (it->url == url && it->login == login) {
                group->entries.erase(it);
                return true;
            }
        }
    }
    return false;
}

PasswordEntry* PasswordManager::findEntryInGroup(const std::wstring& path,
    const std::wstring& url,
    const std::wstring& login) {
    GroupEntry* group = findGroup(path);
    if (group) {
        for (auto& entry : group->entries) {
            if (entry.url == url && entry.login == login) return &entry;
        }
    }
    return nullptr;
}

// --- Recursive display with colors and notes ---
void PasswordManager::listGroupRecursive(const GroupEntry& group, const std::wstring& prefix, bool isLast) const {
    // Выводим название группы
    std::wcout << prefix << (isLast ? L"└─ " : L"├─ ") << group.groupName << L"/" << std::endl;

    // Новый префикс для содержимого группы
    std::wstring childPrefix = prefix + (isLast ? L"    " : L"│   ");

    // Вывод записей
    for (size_t i = 0; i < group.entries.size(); ++i) {
        bool lastEntry = (i == group.entries.size() - 1) && group.subGroups.empty();
        std::wcout << childPrefix << (lastEntry ? L"└─ " : L"├─ ");

        setConsoleColor(group.entries[i].color);
        std::wcout << group.entries[i].url << L" (" << group.entries[i].login << L")";
        setConsoleColor(L"default");

        if (!group.entries[i].note.empty()) {
            std::wcout << L" - " << group.entries[i].note;
        }
        std::wcout << std::endl;
    }

    // Вывод подгрупп
    for (size_t i = 0; i < group.subGroups.size(); ++i) {
        bool lastSubGroup = (i == group.subGroups.size() - 1);
        listGroupRecursive(group.subGroups[i], childPrefix, lastSubGroup);
    }
}

// --- Display all entries ---
void PasswordManager::listAllEntries() const {
    std::wcout << L"Root/" << std::endl;

    // Root entries
    for (size_t i = 0; i < rootEntries.size(); ++i) {
        bool lastRootEntry = (i == rootEntries.size() - 1) && groups.empty();
        std::wcout << (lastRootEntry ? L"└─ " : L"├─ ");

        setConsoleColor(rootEntries[i].color);
        std::wcout << rootEntries[i].url << L" (" << rootEntries[i].login << L")";
        setConsoleColor(L"default");

        if (!rootEntries[i].note.empty()) {
            std::wcout << L" - " << rootEntries[i].note;
        }
        std::wcout << std::endl;
    }

    // Top-level groups
    for (size_t i = 0; i < groups.size(); ++i) {
        bool lastGroup = (i == groups.size() - 1);
        listGroupRecursive(groups[i], L"", lastGroup);
    }
}
