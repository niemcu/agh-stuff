//
// Created by szympeg on 11/17/15.
//

#ifndef MES_FEMGRID_H
#define MES_FEMGRID_H


#include <vector>
#include "Element1D.h"
#include "Node.h"
#include "GlobalData.h"

class FEMGrid {
    std::vector<Element1D> elements;
    std::vector<Node> nodes;

    FEMGrid(GlobalData gd) {
        // teraz pytanie: czy mam to dzielic na dwa rowne elementy czy na rozne? jesli rozne to w jaki sposob?
        // i czy to w ogole ma znaczenie?
        // podziele na pol potem zobaczymy TODO
        double elemLength = gd.length / gd.elementsCount;
        // k i s bedzie trzeba wprowadzac do kazdego elementu - na razie zrobie ze stale
    };
    ~FEMGrid() {};

    void printGrid();
};


#endif //MES_FEMGRID_H
