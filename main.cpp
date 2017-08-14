#include "Server.h"

using namespace std;

int main(int argc, char** argv) {
    Server s(nullptr, stoi(argv[1]));
    s.start();
}
