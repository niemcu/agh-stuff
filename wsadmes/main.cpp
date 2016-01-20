#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

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
        double alfa;
        double temp_start;
        double temp_air;
        double c;
        double lambda;
        double ro;
        double time_max;

        GlobalData() {}
        ~GlobalData() {}

        bool loadFromFile(std::string path);
};

class Node {
    public:
        double coord;

        //int bc; // NONE, HEAT lub CONV
        double temp;
};

class Element1D {
public:
    Node nop1;
    Node nop2;

    std::vector < std::vector<double> > mat;
    std::vector < std::vector<double> > vec;

    Element1D() {
        mat.resize(2, std::vector<double> (2, 0));
        vec.resize(2, std::vector<double> (1, 0));
    }
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
    }
    ~FEMGrid() {}

    void calculate();
    void resetMatrices();
    void printGrid();
    std::vector<Element1D> getElements();
};

void FEMGrid::calculate() {

    double a_modifier = gd.lambda / (gd.c * gd.ro);

    // wagi ptk calkowania sa 1

    double int_point1, int_point2;

    int_point1 = -0.5773502692; //ty kurwa gnoju zgnijesz w piekle za to
    int_point2 = int_point1 * (-1);

    double N1[2], N2[2];

    N1[0] = 0.5 * (1 - int_point1);
    N1[1] = 0.5 * (1 - int_point2);

    N2[0] = 0.5 * (1 + int_point1);
    N2[1] = 0.5 * (1 + int_point2);

    double dr = (gd.radius_max - gd.radius_min) / gd.elements_count;

    double time_step = ( dr * dr ) / ( 0.5 * a_modifier);
    //std::cout << "krewa" << std::endl;

    //tauMax = tau1 + tau2 wtf
    double n_time = (gd.time_max / time_step); // + 1 ????
    time_step = gd.time_max / n_time;

    time_step = 50;

    // jakies zamieszanie z taumax

    double x = 0;

    // petla po wezlach ktora wpisze wspolrzedne oraz temp. poczatkowe do wezlow
    for (int i = 0; i < gd.nodes_count; i++) {
        nodes[i].temp = gd.temp_start;
        nodes[i].coord = x;
        x += dr;
    }
    //std::cout << "krewa2" << std::endl;

//        d_tmax = 0;
//        tau = 0;



    double temp_tau[2];

    // petla po czasie
    for (int i = 0; i < n_time; i++) { // albo o time_step ???>

        // zerowanko macierzy globalnej kURWA

        //std::cout << "czas iter" << i << std::endl;
        // petla po elementach
        for (int j = 0; j < gd.elements_count; j++) {
            //std::cout << "elem" << std::endl;
            double r1, r2;
            r1 = nodes[j].coord;
            r2 = nodes[j+1].coord;

            temp_tau[0] = nodes[j].temp;
            temp_tau[1] = nodes[j+1].temp;

            double dr = r2 - r1;

            double alfa = 0;
            if (j == gd.elements_count - 1) {
                alfa = gd.alfa;
            }
            // zerowanie macierzy lokalnych
            //std::cout << "reset" << std::endl;
            resetMatrices();
            //std::cout << "po resecie" << std::endl;

            // petla po punktach calkowania
            for (int k = 0; k < gd.integration_points_count; k++) {
                double rp = (N1[k] * r1) + (N2[k] * r2);
                double tp_tau = N1[k] * temp_tau[0] + N2[k] * temp_tau[1];

//                std::cout << << "asde/n" <gd.lambda << std::endl
//                          << rp << std::endl
//                          << dr << std::endl
//                          << gd.c << std::endl
//                          << gd.ro<< std::endl
//                          << N1[k] << std::endl
//                          << time_step;
                //std::cout << "nienawidze cie" << ((gd.lambda * rp * 1) / (dr)) << std::endl;
                // obliczanie macierzy lokalnej
                this->elements[j].mat[0][0]
                        += ((gd.lambda * rp * 1) / (dr))
                         + gd.c * gd.ro * dr * rp * 1 * std::pow(N1[k], 2) / time_step;

                this->elements[j].mat[0][1]
                        += ((gd.lambda * rp * 1) / (dr))*(-1)
                         + (gd.c * gd.ro * dr * rp * 1 * N1[k] * N2[k]) / (time_step);

                this->elements[j].mat[1][0] = this->elements[j].mat[0][1];

                this->elements[j].mat[1][1]
                        += ((gd.lambda * rp * 1) / (dr))
                         + (gd.c * gd.ro * dr * rp * 1 * std::pow(N2[k], 2)) / (time_step)
                         + (2 * alfa * gd.radius_max); // ostatni czynnik jest zerem jak nie ma konwekcji

                // lokalny wektor.

                this->elements[j].vec[0][0] += (gd.c * gd.ro * dr * tp_tau * rp * 1 * N1[k]) / (time_step);

                this->elements[j].vec[1][0]
                        += (gd.c * gd.ro * dr * tp_tau * rp * 1 * N2[k]) / (time_step)
                         + (2 * alfa * gd.radius_max * gd.temp_air);
            } // koniec petli po punktach calkowania
            //if (i == 1) printGrid();

            // konstrukcja macierzy globalnej
            // niech kazdy element znajdzie swoje miejsce w macierzy globalnej

            SystemOfEquations sys(grid.getElements(), data.elementsCount);

        } // koniec petli po elementach skonczonych
        // rozwiaz uklad rownan w celu uzyskania nowych temperatur chwilowych
        std::vector<double> result;
        result = sys.solve();
        // przepisanie nowych wartosci temperatur do kazdego wezla
        for (int q = 0; q < result.size(); q++) {
            nodes[q].temp = result[q];
        }
        // i zabawa zaczyna sie od poczatku
    } // koniec petli po krokach czasowych
}

void FEMGrid::resetMatrices() {
    for (int i = 0; i < gd.elements_count; i++) {
        this->elements[i].mat[0][0]
                = this->elements[i].mat[1][0]
                = this->elements[i].mat[0][1]
                = this->elements[i].mat[1][1]
                = 0.0;

//        this->elements[i].mat[0][0] = 0.0;
//        this->elements[i].mat[1][0] = 0.0;
//        this->elements[i].mat[0][1] = 0.0;
//        this->elements[i].mat[1][1] = 0.0;

        this->elements[i].vec[0][0]
                = this->elements[i].vec[1][0]
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

            globalMatrix[i][i]     += e.mat[0][0];
            globalMatrix[i][i+1]   += e.mat[0][1];
            globalMatrix[i+1][i]   += e.mat[1][0];
            globalMatrix[i+1][i+1] += e.mat[1][1];

            globalMatrix[i][count+1]   += e.vec[0][0];
            globalMatrix[i+1][count+1] += e.vec[1][0];

        }
    }
    ~SystemOfEquations() {}

    void printGlobals();
    std::vector<double> solve();
};

int main()
{
    std::cout << "Program start" << std::endl;
    GlobalData data;
    data.loadFromFile("/home/niemcu/agh-stuff/pretmes/pretmes/wejscie.txt");
    //std::cout << "Dane loaded" << std::endl;
    FEMGrid grid(data);
    //std::cout << "Po konstruktorze" << std::endl;
    grid.calculate();
    //std::cout << "Po wszystkim" << std::endl;
    grid.printGrid();

//    SystemOfEquations sys(grid.getElements(), data.elementsCount);
//    std::vector<double> result;
//    result = sys.solve();


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

    this->elements_count = 8;
    this->radius_min = 0.0;
    this->radius_max = 0.08;
    this->alfa = 300;
    this->temp_start = 100;
    this->temp_air = 200;
    this->c = 700;
    this->ro = 7800;
    this->lambda = 25;
    this->time_max = 1000; // seconds

    this->nodes_count = this->elements_count + 1;

    this->integration_points_count = 2;

    return 0;
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


void FEMGrid::printGrid() {
    std::cout << "ELo";
    for (int i = 0; i < gd.elements_count; i++) {
        std::cout << "Macierz dla elementu " << i+1 << std::endl;
        Element1D e = this->elements[i];
        for (int j = 0; j < 2; j++) {
            for (int z = 0; z < 2; z++) {
                std::cout << e.mat[j][z];
            }
            std::cout << std::endl;
        }
        std::cout << "vec dla elementu " << i+1 << std::endl;
        for (int j = 0; j < 2; j++) {
            for (int z = 0; z < 1; z++) {
                std::cout << e.vec[j][z];
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
