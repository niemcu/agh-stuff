#include "GlobalData.h"
#include <fstream>
#include <string>

using namespace std;

GlobalData::GlobalData() {};
GlobalData::~GlobalData() {};

bool GlobalData::loadFromFile(string path) {
    ifstream f;
    f.open(path);

    f >> this->elementsCount >>
            this->nodesCount >>
            this->length >>
            this->surface >>
            this->k;

    

    f.close();
}
