#include "workflow.h"
#include "data_manager.h"
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>

namespace ATM {
namespace Workflow {
namespace {

std::string ReadLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

bool ReadDouble(const std::string& prompt, double& value) {
    std::string text = ReadLine(prompt);
    try {
        value = std::stod(text);
        if (value < 0.0) {
            std::cout << "Value must be non-negative.\n";
            return false;
        }
        return true;
    } catch (...) {
        std::cout << "Invalid number format.\n";
        return false;
    }
}

int FindUserIndex(const std::vector<User>& users, const std::string& username) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].username == username) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void PrintUserList(const std::vector<User>& users) {
    std::cout << "\nStored Users:\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "Username       Type     Balance\n";
    std::cout << "-----------------------------------------\n";
    for (const auto& user : users) {
        std::cout << user.username << "\t" << user.type << "\t" << user.balance << "\n";
    }
    std::cout << "-----------------------------------------\n";
}

void ShowCommonMenu() {
    std::cout << "\nCommon User Menu:\n";
    std::cout << "1. Balance inquiry\n";
    std::cout << "2. Deposit money\n";
    std::cout << "3. Withdraw money\n";
    std::cout << "4. Change password\n";
    std::cout << "5. Logout\n";
}

void ShowAdminMenu() {
    std::cout << "\nAdmin Menu:\n";
    std::cout << "1. View all users\n";
    std::cout << "2. Add common user\n";
    std::cout << "3. Delete common user\n";
    std::cout << "4. Logout\n";
}

void RunCommonUser(std::vector<User>& users, int index) {
    User& active = users[index];
    while (true) {
        ShowCommonMenu();
        std::string choice = ReadLine("Enter choice: ");
        if (choice == "1") {
            std::cout << "Current balance: " << active.balance << "\n";
        } else if (choice == "2") {
            double amount = 0.0;
            if (!ReadDouble("Deposit amount: ", amount)) {
                continue;
            }
            active.balance += amount;
            if (Files::UpdateUser(active)) {
                std::cout << "Deposit complete. New balance: " << active.balance << "\n";
            } else {
                std::cout << "Unable to update balance.\n";
            }
        } else if (choice == "3") {
            double amount = 0.0;
            if (!ReadDouble("Withdraw amount: ", amount)) {
                continue;
            }
            if (amount > active.balance) {
                std::cout << "Insufficient funds.\n";
                continue;
            }
            active.balance -= amount;
            if (Files::UpdateUser(active)) {
                std::cout << "Withdrawal complete. New balance: " << active.balance << "\n";
            } else {
                std::cout << "Unable to update balance.\n";
            }
        } else if (choice == "4") {
            std::string newPassword = ReadLine("New password: ");
            if (newPassword.empty()) {
                std::cout << "Password cannot be empty.\n";
                continue;
            }
            active.password = newPassword;
            if (Files::UpdateUser(active)) {
                std::cout << "Password updated.\n";
            } else {
                std::cout << "Unable to update password.\n";
            }
        } else if (choice == "5") {
            std::cout << "Logging out...\n";
            break;
        } else {
            std::cout << "Invalid option.\n";
        }
    }
}

void RunAdminUser(std::vector<User>& users) {
    while (true) {
        ShowAdminMenu();
        std::string choice = ReadLine("Enter choice: ");
        if (choice == "1") {
            PrintUserList(users);
        } else if (choice == "2") {
            std::string username = ReadLine("New common username: ");
            if (username.empty()) {
                std::cout << "Username cannot be empty.\n";
                continue;
            }
            if (FindUserIndex(users, username) >= 0) {
                std::cout << "User already exists.\n";
                continue;
            }
            std::string password = ReadLine("New password: ");
            if (password.empty()) {
                std::cout << "Password cannot be empty.\n";
                continue;
            }
            double balance = 0.0;
            if (!ReadDouble("Initial balance: ", balance)) {
                continue;
            }
            User newUser;
            newUser.username = username;
            newUser.password = password;
            newUser.type = "common";
            newUser.balance = balance;
            users.push_back(newUser);
            if (Files::SaveUsers(users)) {
                std::cout << "Common user added.\n";
            } else {
                std::cout << "Unable to save user.\n";
            }
        } else if (choice == "3") {
            std::string username = ReadLine("Common username to delete: ");
            int index = FindUserIndex(users, username);
            if (index < 0) {
                std::cout << "User not found.\n";
                continue;
            }
            if (users[index].type != "common") {
                std::cout << "Only common users can be deleted from admin menu.\n";
                continue;
            }
            users.erase(users.begin() + index);
            if (Files::SaveUsers(users)) {
                std::cout << "User deleted.\n";
            } else {
                std::cout << "Unable to delete user.\n";
            }
        } else if (choice == "4") {
            std::cout << "Logging out...\n";
            break;
        } else {
            std::cout << "Invalid option.\n";
        }
    }
}

} // namespace

void RunATM() {
    if (!Files::EnsureUserFile()) {
        std::cerr << "Unable to create or open the user data file.\n";
        return;
    }

    std::vector<User> users = Files::LoadUsers();
    if (users.empty()) {
        std::cerr << "No user records available.\n";
        return;
    }

    std::string userType = ReadLine("Select user type (common/admin): ");
    std::transform(userType.begin(), userType.end(), userType.begin(), ::tolower);

    if (userType != "common" && userType != "admin") {
        std::cout << "User type must be 'common' or 'admin'.\n";
        return;
    }

    std::string username = ReadLine("Username: ");
    std::string password = ReadLine("Password: ");

    int userIndex = FindUserIndex(users, username);
    if (userIndex < 0) {
        std::cout << "Login failed: user not found.\n";
        return;
    }

    User& currentUser = users[userIndex];
    if (currentUser.password != password || currentUser.type != userType) {
        std::cout << "Login failed: invalid credentials or user type.\n";
        return;
    }

    std::cout << "Welcome, " << currentUser.username << "!\n";
    if (userType == "admin") {
        RunAdminUser(users);
    } else {
        RunCommonUser(users, userIndex);
    }
}

} // namespace Workflow
} // namespace ATM
