//Reader
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main() {
    ifstream fifo("bufferpipe"); 

    string received;
    getline(fifo, received);
    fifo.close();

    cout << "Received message: " << received << endl;
    return 0;
    }
