#ifndef ATM_DATA_MANAGER_H
#define ATM_DATA_MANAGER_H

#include <string>
#include <vector>
#include "user.h"

namespace ATM {
namespace Files {
    extern const std::string kUserFilePath;

    bool EnsureUserFile();
    std::vector<User> LoadUsers();
    bool SaveUsers(const std::vector<User>& users);
    bool UpdateUser(const User& user);
}
}

#endif // ATM_DATA_MANAGER_H
