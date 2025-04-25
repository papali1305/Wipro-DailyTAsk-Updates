#include<iostream>
using namespace std; // to avoid naming collision

int main() {
    int num;
    cout << "Enter the number: ";
    cin >> num;
    
    if(num % 2 == 0)
        cout << num << " is even";
    else
        cout << num << " is odd";

    return 0;
}

