#include <iostream>
#include "workflow.h"

int main() {
    std::cout << "ATM Workflow Simulator\n";
    std::cout << "======================\n\n";
    ATM::Workflow::RunATM();
    return 0;
}
