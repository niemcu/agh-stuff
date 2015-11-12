#include "GlobalData.h"
#include <iostream>

using namespace std;

int main() {

    GlobalData data;

    data.loadFromFile("wejscie");

    cout << data.elementsCount << endl;
    cout << data.nodesCount << endl;
    cout << data.length << endl;
    cout << data.surface << endl;
    cout << data.k << endl;

    cout << "Hello, World!" << endl;
    return 0;
}