#include "data_manager.h"
#include <fstream>
#include <iomanip>
#include <sstream>

namespace ATM {
namespace Files {

const std::string kUserFilePath = "atm/users.txt";

bool EnsureUserFile() {
    std::ifstream input(kUserFilePath);
    if (input.good()) {
        return true;
    }

    std::ofstream output(kUserFilePath);
    if (!output.is_open()) {
        return false;
    }

    output << "admin admin admin 1000.00\n";
    output << "alice 1234 common 250.00\n";
    return output.good();
}

std::vector<User> LoadUsers() {
    std::vector<User> users;
    std::ifstream input(kUserFilePath);
    if (!input.is_open()) {
        return users;
    }

    std::string line;
    while (std::getline(input, line)) {
        std::istringstream parser(line);
        User user;
        if (!(parser >> user.username >> user.password >> user.type >> user.balance)) {
            continue;
        }
        users.push_back(user);
    }
    return users;
}

bool SaveUsers(const std::vector<User>& users) {
    std::ofstream output(kUserFilePath);
    if (!output.is_open()) {
        return false;
    }

    output << std::fixed << std::setprecision(2);
    for (const auto& user : users) {
        output << user.username << ' ' << user.password << ' ' << user.type << ' ' << user.balance << '\n';
    }
    return output.good();
}

bool UpdateUser(const User& user) {
    std::vector<User> users = LoadUsers();
    bool found = false;
    for (auto& existing : users) {
        if (existing.username == user.username) {
            existing = user;
            found = true;
            break;
        }
    }
    if (!found) {
        return false;
    }
    return SaveUsers(users);
}

} // namespace Files
} // namespace ATM
