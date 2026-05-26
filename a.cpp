#include <bits/stdc++.h>
using namespace std;

bool isPrime(int x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (int i = 3; i <= static_cast<int>(sqrt(x)); i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

int main() {
    long long n;
    if (!(cin >> n) || n <= 0) {
        return 0;
    }

    long long count = 0;
    int value = 2;
    while (count < n) {
        if (isPrime(value)) {
            cout << value;
            count++;
            if (count < n) cout << " ";
        }
        value++;
    }
    cout << '\n';
    return 0;
}
