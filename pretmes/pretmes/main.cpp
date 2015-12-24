#include <iostream>
#include <vector>
#include <fstream>

#define NONE 0
#define HEAT 1
#define CONV 2

class Node {
    public:
        int nid;
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

        GlobalData();
        ~GlobalData();

        bool loadFromFile(std::string path);
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
        // teraz pytanie: czy mam to dzielic na dwa rowne elementy czy na rozne? jesli rozne to w jaki sposob?
        // i czy to w ogole ma znaczenie?
        // podziele na pol potem zobaczymy TODO
        elemLength = data.length / data.elementsCount;
        // k i s bedzie trzeba wprowadzac do kazdego elementu - na razie zrobie ze stale

        // potem sie uzmienni

    };
    ~FEMGrid() {};

    void insertNodes();
    void setElements();
    void setBoundaryConditions();

    void setLocalStiffnessMatricesAndLoadVectors();

    void printGrid();
    std::vector<Element1D> getElements();
};

void Element1D::setNodes(Node n1, Node n2) {
    this->nop1 = n1;
    this->nop2 = n2;
}

GlobalData::GlobalData() {}
GlobalData::~GlobalData() {}

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
        newNode.nid = i;
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

//void SystemOfEquations::solve() {
//    int n = A.size();

//    for (int i=0; i<n; i++) {
//        // Search for maximum in this column
//        double maxEl = abs(A[i][i]);
//        int maxRow = i;
//        for (int k=i+1; k<n; k++) {
//            if (abs(A[k][i]) > maxEl) {
//                maxEl = abs(A[k][i]);
//                maxRow = k;
//            }
//        }

//        // Swap maximum row with current row (column by column)
//        for (int k=i; k<n+1;k++) {
//            double tmp = A[maxRow][k];
//            A[maxRow][k] = A[i][k];
//            A[i][k] = tmp;
//        }

//        // Make all rows below this one 0 in current column
//        for (int k=i+1; k<n; k++) {
//            double c = -A[k][i]/A[i][i];
//            for (int j=i; j<n+1; j++) {
//                if (i==j) {
//                    A[k][j] = 0;
//                } else {
//                    A[k][j] += c * A[i][j];
//                }
//            }
//        }
//    }

//    // Solve equation Ax=b for an upper triangular matrix A
//    vector<double> x(n);
//    for (int i=n-1; i>=0; i--) {
//        x[i] = A[i][n]/A[i][i];
//        for (int k=i-1;k>=0; k--) {
//            A[k][n] -= A[k][i] * x[i];
//        }
//    }
//    return x;
//}

void FEMGrid::setElements() {
    for (int i = 0; i < data.elementsCount; i++) {
        std::cout << data.elementsCount << "iletokurwajestwgciebie" << this->elemLength << std::endl;
        Element1D el(this->elemLength, data.surface, data.modifier);
        el.setNodes(this->nodes[i], this->nodes[i+1]);
        this->elements[i] = el;
        std::cout << "ytyty" << this->elements[i].length << std::endl;
        std::cout << "element" << i << "->" << this->elements[i].surface << this->elements[i].modifier << "guwno" << std::endl;
    }
}

void FEMGrid::setLocalStiffnessMatricesAndLoadVectors() {
    // macierz H lokalna wyglada tak ze liczymy c
    double C;
    for (int i = 0; i < data.elementsCount; i++) {
        Element1D e = this->elements[i];
        std::cout << "xdelement" << i << "->" << this->elements[i].surface << this->elements[i].length << "hujguwno" << std::endl;
        C = (this->elements[i].modifier * this->elements[i].surface) / this->elements[i].length;
        //std::cout << "element" << << std::endl;
        std::cout << "C:" << C << std::endl;
        this->elements[i].stiffnessMatrix[0][0] = this->elements[i].stiffnessMatrix[1][1] =  C;
        this->elements[i].stiffnessMatrix[0][1] = this->elements[i].stiffnessMatrix[1][0] = -C;

        // warunki brzegowe
        if (this->elements[i].nop1.bc == HEAT) {
            // dodaj strumien do P
            this->elements[i].loadVector[0][0] += data.heat * data.surface;
        }
        std::cout << "bc: " << this->elements[i].nop2.bc << std::endl;
        if (this->elements[i].nop2.bc == CONV) {
            // dodaj konwekcje alfaS tu i do P
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

int main()
{
    // 1. czytamy z pliku GlobalData
    GlobalData data;

    data.loadFromFile("/home/niemcu/agh-stuff/pretmes/pretmes/wejscie.txt");

//    std::cout << data.elementsCount << std::endl;
//    std::cout << data.nodesCount << std::endl;
//    std::cout << data.length << std::endl;
//    std::cout << data.surface << std::endl;
//    std::cout << data.modifier << std::endl;
//    std::cout << data.alfa << std::endl;
//    std::cout << data.heat << std::endl;
//    std::cout << data.too << std::endl;

    // 2. wypelnij wektory nopami (elementy)
    FEMGrid grid(data);

    grid.insertNodes();

    grid.setBoundaryConditions();

    grid.setElements();

    // 3. set boundary conditions
    std::cout << "Hello, World! eeehehe" << std::endl;

    // 4. wszystkie lokalne h i lokalne p
    grid.setLocalStiffnessMatricesAndLoadVectors();
    std::cout << "Hello, World! eeehehe" << std::endl;

    grid.printGrid();
    std::cout << "Hello, World! eeehehe" << std::endl;

    // 5. globalne h i p

    SystemOfEquations sys(grid.getElements(), data.elementsCount);

    // 6. gauss uklad rownan
    sys.printGlobals();
    //sys.solve();

    // 7. wypisz wynik

    std::cout << "Hello, World! eeehehe" << std::endl;

    return 0;
}


