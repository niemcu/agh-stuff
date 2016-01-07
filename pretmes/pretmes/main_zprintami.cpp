#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#define NONE 0
#define HEAT 1
#define CONV 2

class GlobalData {
public:
        int elementsCount; // ilosc elementow skonczonych
        int nodesCount; // ilosc wezlow
        double length; // dlugosc preta
        double surface; // powierzchnia przekroju poprzecznego
        double modifier; // wspolczynnik przewodzenia ciepla

        double alfa; // wspolczynnik wymiany ciepla przez konwekcje
        double heat; // strumien ciepla q
        double too; // temp. otoczenia tnieskonczonsc

        GlobalData() {}
        ~GlobalData() {}

        bool loadFromFile(std::string path);
};

class Node {
    public:
        int bc; // NONE, HEAT lub CONV
};

class Element1D {
public:
    Node nop1;
    Node nop2;

    double length;
    double surface;
    double modifier;

    std::vector < std::vector<double> > stiffnessMatrix;
    std::vector < std::vector<double> > loadVector;

    Element1D(double l, double s, double k) : length(l), surface(s), modifier(k) {
        stiffnessMatrix.resize(2, std::vector<double> (2, 0));
        loadVector.resize(2, std::vector<double> (1, 0));
    }
    Element1D() : length(0), surface(0), modifier(0) {}
    ~Element1D() {}

    void setNodes(Node n1, Node n2);
};

class FEMGrid {
    std::vector<Element1D> elements;
    std::vector<Node> nodes;
    GlobalData data;
    double elemLength;
public:
    FEMGrid(GlobalData gd) {
        data = gd;
        nodes.resize(data.nodesCount);
        elements.resize(data.elementsCount);
        // zalozenia: kazdy element ma taka sama dlugosc, w kazdym el. takie samo k oraz S
        elemLength = data.length / data.elementsCount;
    }
    ~FEMGrid() {}

    void insertNodes();
    void setElements();
    void setBoundaryConditions();

    void setLocalStiffnessMatricesAndLoadVectors();

    void printGrid();
    std::vector<Element1D> getElements();
};

class SystemOfEquations {
public:
    std::vector < std::vector<double> > globalMatrix;
    int rowsCount;

    SystemOfEquations(std::vector<Element1D> elements, int count) {

        rowsCount = count + 1;

        globalMatrix.resize(rowsCount, std::vector<double> (rowsCount + 1, 0));

        for (int i = 0; i < count; i++) {
            Element1D e = elements[i];

            globalMatrix[i][i]     += e.stiffnessMatrix[0][0];
            globalMatrix[i][i+1]   += e.stiffnessMatrix[0][1];
            globalMatrix[i+1][i]   += e.stiffnessMatrix[1][0];
            globalMatrix[i+1][i+1] += e.stiffnessMatrix[1][1];

            globalMatrix[i][count+1]   += e.loadVector[0][0];
            globalMatrix[i+1][count+1] += e.loadVector[1][0];

        }
    }
    ~SystemOfEquations() {}

    void printGlobals();
    std::vector<double> solve();
};

int main()
{
    GlobalData data;
    data.loadFromFile("/home/niemcu/agh-stuff/pretmes/pretmes/wejscie.txt");

    FEMGrid grid(data);
    grid.insertNodes();
    grid.setBoundaryConditions();
    grid.setElements();
    grid.setLocalStiffnessMatricesAndLoadVectors();

    SystemOfEquations sys(grid.getElements(), data.elementsCount);

    std::vector<double> result;
    
    sys.printGlobals();

    result = sys.solve();

    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i] << "__";
    }

    return 0;
}

void Element1D::setNodes(Node n1, Node n2) {
    this->nop1 = n1;
    this->nop2 = n2;
}

bool GlobalData::loadFromFile(std::string path) {
    std::fstream f(path.c_str(), std::ios_base::in);

    if (f.good()) {
        std::cout << "plik otworzon";
    } else {
        std::cout << "jest chujnia jakas";
    }

    f >> this->elementsCount >> this->length >> this->surface >> this->modifier >> this->alfa >> this->heat >> this->too;

    f.close();

    this->nodesCount = this->elementsCount + 1;

    return 0;
}

std::vector<Element1D> FEMGrid::getElements() {
    return this->elements;
}

void FEMGrid::insertNodes() {
    for (int i = 0; i < data.nodesCount; i++) {
        Node newNode;
        this->nodes.push_back(newNode);
    }
}

void FEMGrid::setBoundaryConditions() {
    for (int i = 0; i < data.nodesCount; i++) {
        if (i == 0) {
            this->nodes[i].bc = HEAT;
        } else if (i == data.nodesCount - 1) {
            this->nodes[i].bc = CONV;
        } else {
            this->nodes[i].bc = NONE;
        }
    }
}

std::vector<double> SystemOfEquations::solve() {

    for (int i = 0; i < rowsCount; i++) {
        // Search for maximum in this column
        double maxEl = std::abs(globalMatrix[i][i]);
        int maxRow = i;
        for (int k = i + 1; k < rowsCount; k++) {
            if (std::abs(globalMatrix[k][i]) > maxEl) {
                maxEl = std::abs(globalMatrix[k][i]);
                maxRow = k;
            }
        }

        // Swap maximum row with current row (column by column)
        for (int k = i; k < rowsCount + 1; k++) {
            double tmp = globalMatrix[maxRow][k];
            globalMatrix[maxRow][k] = globalMatrix[i][k];
            globalMatrix[i][k] = tmp;
        }

        // Make all rows below this one 0 in current column
        for (int k = i + 1; k < rowsCount; k++) {
            double c = -globalMatrix[k][i] / globalMatrix[i][i];
            for (int j = i; j < rowsCount + 1; j++) {
                if (i == j) {
                    globalMatrix[k][j] = 0;
                } else {
                    globalMatrix[k][j] += c * globalMatrix[i][j];
                }
            }
        }
    }

    // Solve equation Ax=b for an upper triangular matrix A
    std::vector<double> x(rowsCount);
    for (int i = rowsCount - 1; i >= 0; i--) {
        x[i] = globalMatrix[i][rowsCount] / globalMatrix[i][i];
        for (int k = i-1; k >= 0; k--) {
            globalMatrix[k][rowsCount] -= globalMatrix[k][i] * x[i];
        }
    }
    return x;
}

void FEMGrid::setElements() {
    for (int i = 0; i < data.elementsCount; i++) {
        Element1D el(this->elemLength, data.surface, data.modifier);
        el.setNodes(this->nodes[i], this->nodes[i+1]);
        this->elements[i] = el;
    }
}

void FEMGrid::setLocalStiffnessMatricesAndLoadVectors() {
    double C;
    for (int i = 0; i < data.elementsCount; i++) {
        Element1D e = this->elements[i];
        C = (this->elements[i].modifier * this->elements[i].surface) / this->elements[i].length;
        this->elements[i].stiffnessMatrix[0][0] = this->elements[i].stiffnessMatrix[1][1] =  C;
        this->elements[i].stiffnessMatrix[0][1] = this->elements[i].stiffnessMatrix[1][0] = -C;

        if (this->elements[i].nop1.bc == HEAT) {
            this->elements[i].loadVector[0][0] += data.heat * data.surface;
        }

        if (this->elements[i].nop2.bc == CONV) {
            this->elements[i].stiffnessMatrix[1][1] += data.alfa * data.surface;
            this->elements[i].loadVector[1][0] -= data.alfa * data.surface * data.too;
        }
    }
}













void FEMGrid::printGrid() {
    for (int i = 0; i < data.elementsCount; i++) {
        std::cout << "Macierz dla elementu " << i+1 << std::endl;
        Element1D e = this->elements[i];
        for (int j = 0; j < 2; j++) {
            for (int z = 0; z < 2; z++) {
                std::cout << e.stiffnessMatrix[j][z];
            }
            std::cout << std::endl;
        }
        std::cout << "LoadVector dla elementu " << i+1 << std::endl;
        for (int j = 0; j < 2; j++) {
            for (int z = 0; z < 1; z++) {
                std::cout << e.loadVector[j][z];
            }
            std::cout << std::endl;
        }
    }
}


void SystemOfEquations::printGlobals() {
    std::cout << "Wszystko: " << std::endl;
    for (int i = 0; i < rowsCount; i++) {
        std::cout << "[ ";
        for (int j = 0; j < rowsCount + 1; j++) {
                std::cout << globalMatrix[i][j] << " ";
        }
        std::cout << " ] ";
        std::cout << std::endl;
    }
}
