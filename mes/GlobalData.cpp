#include "GlobalData.h"
#include <fstream>
#include <string>
#include <iostream>

GlobalData::GlobalData() {};
GlobalData::~GlobalData() {};

bool GlobalData::loadFromFile(std::string path) {
    std::fstream f(path, std::ios_base::in);

    if (f.good()) {
        std::cout << "plik otworzon";
    } else {
        std::cout << "jest chujnia jakas";
    }

    f >> this->elementsCount >> this->nodesCount >> this->length >> this->surface >> this->modifier;

    f.close();

    return 0;
}
