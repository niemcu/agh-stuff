//
// Created by szympeg on 11/17/15.
//

#ifndef MES_FEMGRID_H
#define MES_FEMGRID_H


#include <vector>
#include "Element1D.h"
#include "Node.h"

class FEMGrid {
    std::vector<Element1D> elements;
    std::vector<Node> nodes;

    FEMGrid() {};
    ~FEMGrid() {};
    
    void printGrid();
};


#endif //MES_FEMGRID_H
