#include <iostream>
#include <vector>
#include <fstream>

#define HEAT 1
#define CONV 2

class Node {
    public:
        int nid;
        int bc; // 1 - strumien ciepla, 2 - konwekcja

        void addBC();
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
    //double stiffnessMatrix[2][2];
    //double loadVector[2][1];
    Element1D(double l, double s, double k) : length(l), surface(s), modifier(k) {
        stiffnessMatrix.resize(2, std::vector<double> (2, 0));
        loadVector.resize(2, std::vector<double> (1, 0));
    }
    Element1D() {
        length = 0;
        surface = 0;
        modifier = 0;
    }
    ~Element1D() {}

    void setNodes(Node n1, Node n2);
};

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

    void setLocalStiffnessMatrices();
    void setLocalLoadVectors();

    void setGlobalStiffnessMatrix();
    void setGlobalLoadVector();

    void printGrid();
};

void Node::addBC() {

}

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

void FEMGrid::insertNodes() {
    // najpierw node'y
    for (int i = 0; i < data.nodesCount; i++) {
        Node newNode;
        newNode.nid = i;
        this->nodes.push_back(newNode);
    }
}

void FEMGrid::setBoundaryConditions() {
    for (int i = 0; i < data.nodesCount; i++) {
        if (i == 0) {
            // wstaw bc - strumien ciepla
            this->nodes[i].bc = HEAT;
        } else if (i == data.nodesCount - 1) {
            // wstaw bc - konwekcja
            this->nodes[i].bc = CONV;
        } else {
            this->nodes[i].bc = 0;
        }
    }
}

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

void FEMGrid::setLocalStiffnessMatrices() {
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
        }
        std::cout << "bc: " << this->elements[i].nop2.bc << std::endl;
        if (this->elements[i].nop2.bc == CONV) {
            // dodaj konwekcje alfaS tu i do P
            this->elements[i].stiffnessMatrix[1][1] += data.alfa * data.surface;
        }
    }
}

void FEMGrid::setLocalLoadVectors() {

}

void FEMGrid::setGlobalStiffnessMatrix() {

}

void FEMGrid::setGlobalLoadVector() {

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
    }
}

int main()
{
    // 1. czytamy z pliku GlobalData
    GlobalData data;

    data.loadFromFile("/home/szympeg/Desktop/agh-stuff/mes/wejscie.txt");

    std::cout << data.elementsCount << std::endl;
    std::cout << data.nodesCount << std::endl;
    std::cout << data.length << std::endl;
    std::cout << data.surface << std::endl;
    std::cout << data.modifier << std::endl;
    std::cout << data.alfa << std::endl;
    std::cout << data.heat << std::endl;
    std::cout << data.too << std::endl;

    std::cout << "Hello, World! eeehehe" << std::endl;

    // 2. wypelnij wektory nopami (elementy)
    FEMGrid grid(data);
    std::cout << "Hello, World! eeehehe" << std::endl;

    grid.insertNodes();
    std::cout << "Hello, World! eeehehe" << std::endl;
    grid.setBoundaryConditions();

    grid.setElements();
    std::cout << "Hello, World! eeehehe" << std::endl;

    // 3. set boundary conditions
    std::cout << "Hello, World! eeehehe" << std::endl;

    // 4. wszystkie lokalne h i lokalne p
    grid.setLocalStiffnessMatrices();
    std::cout << "Hello, World! eeehehe" << std::endl;

    grid.printGrid();
    std::cout << "Hello, World! eeehehe" << std::endl;

    grid.setLocalLoadVectors();
    // 5. globalne h i p
    // 6. gauss uklad rownan
    // 7. wypisz wynik

    std::cout << "Hello, World! eeehehe" << std::endl;

    return 0;
}


