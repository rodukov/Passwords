#include "PasswordManager.h"
#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

int main() {
    // Настройка консоли на вывод UTF-16 (wstring)
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    PasswordManager manager;

    // --- Root entries ---
    manager.addRootEntry(L"https://gmail.com", L"gleb123", L"pass123", L"main email", L"yellow");
    manager.addRootEntry(L"https://outlook.com", L"gleb_outlook", L"outlookPass", L"work mail", L"blue");

    // --- Nested groups ---
    manager.addGroup(L"Work/ProjectA");
    manager.addGroup(L"Work/ProjectB");
    manager.addGroup(L"Personal/Social");

    // --- Entries in groups ---
    manager.addEntryToGroup(L"Work/ProjectA", L"https://github.com", L"gleb_work", L"workPass", L"repo access", L"green");
    manager.addEntryToGroup(L"Work/ProjectB", L"https://jira.com", L"gleb_jira", L"jiraPass", L"task tracker", L"red");
    manager.addEntryToGroup(L"Personal/Social", L"https://vk.com", L"gleb_vk", L"vkPass", L"social network", L"blue");

    // --- Display all entries ---
    manager.listAllEntries();

    return 0;
}
