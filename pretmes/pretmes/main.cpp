#include <iostream>
#include <vector>
#include <fstream>


class Node {
    public:
        int nid;
        int bc;

        void addBC();
};

class Element1D {
public:
    Node nop1;
    Node nop2;

    int length;
    int surface;
    int modifier;

    std::vector < std::vector<double> > stiffnessMatrix;
    std::vector < std::vector<double> > loadVector;

    Element1D(int l, int s, int k) : length(l), surface(s), modifier(k) {};
    ~Element1D() {};

    void setNodes(Node n1, Node n2);
};

class GlobalData {

    public:
        int elementsCount;
        int nodesCount;
        int length;
        int surface;
        int modifier;

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
        // teraz pytanie: czy mam to dzielic na dwa rowne elementy czy na rozne? jesli rozne to w jaki sposob?
        // i czy to w ogole ma znaczenie?
        // podziele na pol potem zobaczymy TODO
        elemLength = gd.length / gd.elementsCount;
        // k i s bedzie trzeba wprowadzac do kazdego elementu - na razie zrobie ze stale

        // potem sie uzmienni

    };
    ~FEMGrid() {};

    void insertNodes();
    void setElements();
    void setBoundaryConditions();
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

    f >> this->elementsCount >> this->length >> this->surface >> this->modifier;

    f.close();

    this->nodesCount = this->elementsCount + 1;

    return 0;
}

void FEMGrid::insertNodes() {
    // najpierw node'y
    for (int i = 0; i < data.nodesCount; i++) {
        Node newNode;
        newNode.nid = i;
        this->nodes[i] = newNode;
    }
}

void FEMGrid::setBoundaryConditions() {
    for (int i = 0; i < data.nodesCount; i++) {
        if (i == 0) {
            // wstaw bc - strumien ciepla
        } else if (i == data.nodesCount - 1) {
            // wstaw bc - konwekcja
        } else {
            this->nodes[i].bc = 0;
        }
    }
}

void FEMGrid::setElements() {
    for (int i = 0; i < data.elementsCount; i++) {
        Element1D el(this->elemLength, data.surface, data.modifier);
        el.setNodes(this->nodes[i], this->nodes[i+1]);
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

    // 2. wypelnij wektory nopami (elementy)
    FEMGrid grid(data);

    grid.insertNodes();
    grid.setElements();
    // 3. set boundary conditions
    grid.setBoundaryConditions();
    // 4. wszystkie lokalne h i lokalne p
    // 5. globalne h i p
    // 6. gauss uklad rownan
    // 7. wypisz wynik



    std::cout << "Hello, World! hehe" << std::endl;

    return 0;
}


