#include "PasswordManager.h"
#include <iostream>
#include <sstream>

// --- Root entries ---
void PasswordManager::addRootEntry(const std::string& url,
                                   const std::string& login,
                                   const std::string& password,
                                   const std::string& note,
                                   const std::string& color) {
    rootEntries.push_back({url, login, password, note, color});
}

bool PasswordManager::removeRootEntry(const std::string& url, const std::string& login) {
    for (auto it = rootEntries.begin(); it != rootEntries.end(); ++it) {
        if (it->url == url && it->login == login) {
            rootEntries.erase(it);
            return true;
        }
    }
    return false;
}

PasswordEntry* PasswordManager::findRootEntry(const std::string& url, const std::string& login) {
    for (auto& entry : rootEntries) {
        if (entry.url == url && entry.login == login) return &entry;
    }
    return nullptr;
}

// --- Helper to split path ---
static std::vector<std::string> splitPath(const std::string& path) {
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string item;
    while (std::getline(ss, item, '/')) {
        if (!item.empty()) parts.push_back(item);
    }
    return parts;
}

// --- Recursive find group ---
GroupEntry* PasswordManager::findGroupRecursive(std::vector<GroupEntry>& groups, const std::string& path) {
    auto parts = splitPath(path);
    std::vector<GroupEntry>* current = &groups;
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

GroupEntry* PasswordManager::findGroup(const std::string& path) {
    return findGroupRecursive(groups, path);
}

// --- Add group with nested support ---
void PasswordManager::addGroup(const std::string& path) {
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
            current->push_back({part, {}, {}});
            group = &current->back();
        }
        current = &group->subGroups;
    }
}

bool PasswordManager::removeGroup(const std::string& path) {
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
void PasswordManager::addEntryToGroup(const std::string& path,
                                      const std::string& url,
                                      const std::string& login,
                                      const std::string& password,
                                      const std::string& note,
                                      const std::string& color) {
    GroupEntry* group = findGroup(path);
    if (group) {
        group->entries.push_back({url, login, password, note, color});
    }
}

bool PasswordManager::removeEntryFromGroup(const std::string& path,
                                           const std::string& url,
                                           const std::string& login) {
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

PasswordEntry* PasswordManager::findEntryInGroup(const std::string& path,
                                                 const std::string& url,
                                                 const std::string& login) {
    GroupEntry* group = findGroup(path);
    if (group) {
        for (auto& entry : group->entries) {
            if (entry.url == url && entry.login == login) return &entry;
        }
    }
    return nullptr;
}

// --- Recursive display ---
void PasswordManager::listGroupRecursive(const GroupEntry& group, const std::string& prefix) const {
    std::cout << prefix << group.groupName << "/" << std::endl;

    for (size_t i = 0; i < group.entries.size(); ++i) {
        std::cout << prefix << "├─ " << group.entries[i].url << " (" << group.entries[i].login << ")" << std::endl;
    }

    for (size_t i = 0; i < group.subGroups.size(); ++i) {
        listGroupRecursive(group.subGroups[i], prefix + "│  ");
    }
}

// --- Display all ---
void PasswordManager::listAllEntries() const {
    std::cout << "Root/" << std::endl;
    for (const auto& entry : rootEntries) {
        std::cout << "├─ " << entry.url << " (" << entry.login << ")" << std::endl;
    }

    for (const auto& group : groups) {
        listGroupRecursive(group, "│  ");
    }
}
