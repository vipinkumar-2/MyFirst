#ifndef ATM_USER_H
#define ATM_USER_H

#include <string>

namespace ATM {
    struct User {
        std::string username;
        std::string password;
        std::string type;
        double balance;
    };
}

#endif // ATM_USER_H
