#include "GlobalData.h"
#include "FEMGrid.h"
#include <iostream>

using namespace std;

int main() {

    GlobalData data;

    data.loadFromFile("/home/szympeg/Desktop/agh-stuff/mes/wejscie.txt");

    cout << data.elementsCount << endl;
    cout << data.nodesCount << endl;
    cout << data.length << endl;
    cout << data.surface << endl;
    cout << data.modifier << endl;

    cout << "Hello, World!" << endl;

    FEMGrid grid =;

    return 0;
}