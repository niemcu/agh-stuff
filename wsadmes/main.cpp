#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#define NONE 0
#define HEAT 1
#define CONV 2

// co to są funktory i traity sie dowiedz

//class InputData {
//public:
//    //int elementsCount; // ilosc elementow skonczonych
//    //int nodesCount; // ilosc wezlow
//};

class GlobalData {
public:

        int elements_count;
        int nodes_count;
        int integration_points_count;

        double radius_min;
        double radius_max;
        double convection_modifier;
        double temp_start;
        double temp_air;
        double specific_heat;
        double conduction_modifier;
        double density;
        double time_max;

        GlobalData() {}
        ~GlobalData() {}

        bool loadFromFile(std::string path);
};

class Node {
    public:
        double coord;

        int bc; // NONE, HEAT lub CONV
        double temp;
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
    GlobalData gd;
    double elemLength;
public:
    FEMGrid(GlobalData data) {
        gd = data;
        nodes.resize(gd.nodes_count);
        elements.resize(gd.elements_count);
        // zalozenia: kazdy element ma taka sama dlugosc, w kazdym el. takie samo k oraz S
        elemLength = data.length / data.elementsCount;

        double a_modifier = gd.convection_modifier / (gd.specific_heat * gd.density);

        // wagi ptk calkowania sa 1

        double int_point1, int_point2;

        int_point1 = int_point2 = 0.5773502692;

        double N1_1, N1_2, N2_1, N2_2;

        N1_1 = 0.5 * (1 - int_point1);
        N1_2 = 0.5 * (1 - int_point2);

        N2_1 = 0.5 * (1 + int_point1);
        N2_2 = 0.5 * (1 + int_point2);

        double elem_length = (gd.radius_max - gd.radius_min) / gd.elements_count;

        double time_step = ( elem_length * elem_length ) / ( 0.5 * a_modifier);

        // jakies zamieszanie z taumax

        double x = 0;

        // petla po wezlach ktora wpisze wspolrzedne oraz temp. poczatkowe do wezlow
        for (int i = 0; i < gd.nodes_count; i++) {
            nodes[i].temp = gd.temp_start;
            nodes[i].coord = x;
            x += elem_length;
        }

        // petla po czasie
        for (;;) {
            // zerowanie macierzy lokalnych i globalnych
            resetMatrices();

            // petla po elementach
            for (j = 0; j < gd.elements_count; j++) {
                double r1, r2;
                r1 = nodes[j].coord;
                r2 = nodes[j+1].coord;

                //tempTau jest iles rowne

                // petla po punktach calkowania
                for (int k = 0; k < gd.integration_points_count; k++) {
                    double int_coord = ;

                    // obliczanie macierzy lokalnej
                    C = (this->elements[i].modifier * this->elements[i].surface) / this->elements[i].length;

                    this->elements[i].stiffnessMatrix[0][0] +=


                            this->elements[i].stiffnessMatrix[1][1] =  C;
                    this->elements[i].stiffnessMatrix[0][1] = this->elements[i].stiffnessMatrix[1][0] = -C;
                }
            }
        }
    }
    ~FEMGrid() {}

    void resetMatrices();

    void insertNodes();
    void setElements();
    void setBoundaryConditions();

    void setLocalStiffnessMatricesAndLoadVectors();

    void printGrid();
    std::vector<Element1D> getElements();
};

void FEMGrid::resetMatrices() {
    // lokalne poki co
    for (int i = 0; i < gd.elements_count; i++) {
        this->elements[i].stiffnessMatrix[0][0]
                = this->elements[i].stiffnessMatrix[1][0]
                = this->elements[i].stiffnessMatrix[0][1]
                = this->elements[i].stiffnessMatrix[1][1]
                = 0.0;
    }
}

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

    grid.calculate();

//    grid.setBoundaryConditions();
//    grid.setElements();
//    grid.setLocalStiffnessMatricesAndLoadVectors();

//    SystemOfEquations sys(grid.getElements(), data.elementsCount);

//    std::vector<double> result;

//    sys.printGlobals();

//    result = sys.solve();

//    for (int i = 0; i < result.size(); i++) {
//        std::cout << result[i] << "__";
//    }

    return 0;
}

void Element1D::setNodes(Node n1, Node n2) {
    this->nop1 = n1;
    this->nop2 = n2;
}

bool GlobalData::loadFromFile(std::string path) {
//    std::fstream f(path.c_str(), std::ios_base::in);

//    if (f.good()) {
//        std::cout << "plik otworzon";
//    } else {
//        std::cout << "jest chujnia jakas";
//    }

//    f >> this->elementsCount >> this->length >> this->surface >> this->modifier >> this->alfa >> this->heat >> this->too;

//    f.close();

    this->elements_count = 5;
    this->radius_min = 0.0;
    this->radius_max = 0.05;
    this->convection_modifier = 300;
    this->temp_start = 100;
    this->temp_air = 1200;
    this->specific_heat = 700;
    this->density = 7800;
    this->conduction_modifier = 25;
    this->time_max = 1000; // seconds

    this->nodes_count = this->elements_count + 1;

    this->integration_points_count = 2;


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
