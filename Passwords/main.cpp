#include "PasswordManager.h"
#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    PasswordManager manager;

    // Root entries
    manager.addRootEntry(L"https://gmail.com", L"gleb123", L"pass123");

    // Nested groups
    manager.addGroup(L"Work/ProjectA");
    manager.addGroup(L"Work/ProjectB");
    manager.addGroup(L"Personal/");
    manager.addGroup(L"Personal/Social");

    // Entries in groups
    manager.addEntryToGroup(L"Work/ProjectA", L"https://github.com", L"gleb_work", L"workPass");
    manager.addEntryToGroup(L"Work/", L"https://jira.com", L"gleb_jira", L"jiraPass");
    manager.addEntryToGroup(L"Personal/Social", L"https://vk.com", L"gleb_vk", L"vkPass");
    manager.addEntryToGroup(L"Personal/", L"https://vk.com", L"gleb_vk", L"vkPass");
    manager.addEntryToGroup(L"Personal/", L"https://vk.com", L"gleb_vk", L"vkPass");
    manager.addEntryToGroup(L"Personal/Social", L"https://vk.com", L"gleb_vk", L"vkPass");

    // Display all entries
    manager.listAllEntries();

    return 0;
}
