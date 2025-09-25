#include "PasswordManager.h"
#include <windows.h>
#include <iostream>

int main() {
    PasswordManager manager;

    // Root entries
    manager.addRootEntry("https://gmail.com", "gleb123", "pass123");

    // Nested groups
    manager.addGroup("Work/ProjectA");
    manager.addGroup("Work/ProjectB");
    manager.addGroup("Personal/Social");

    // Entries in groups
    manager.addEntryToGroup("Work/ProjectA", "https://github.com", "gleb_work", "workPass");
    manager.addEntryToGroup("Work/ProjectB", "https://jira.com", "gleb_jira", "jiraPass");
    manager.addEntryToGroup("Personal/Social", "https://vk.com", "gleb_vk", "vkPass");

    // Display
    manager.listAllEntries();

    return 0;
}
