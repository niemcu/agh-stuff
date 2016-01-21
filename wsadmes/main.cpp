#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

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
};

class SystemOfEquations {
public:
    std::vector < std::vector<double> > globalMatrix;
    int rowsCount;

    SystemOfEquations() {}
    ~SystemOfEquations() {}

    void setSystem(std::vector<Element1D> elements, int count);
    void resetGlobal();
    std::vector<double> solve();
};

void SystemOfEquations::setSystem(std::vector<Element1D> elements, int count) {

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

void SystemOfEquations::resetGlobal() {
    for (int i = 0; i < rowsCount + 1; i++) {

        globalMatrix[i][i]     = 0;
        globalMatrix[i][i+1]   = 0;
        globalMatrix[i+1][i]   = 0;
        globalMatrix[i+1][i+1] = 0;

        globalMatrix[i][rowsCount]   = 0;
        globalMatrix[i+1][rowsCount] = 0;
    }
}

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
// rozwiaz zadanie
void FEMGrid::calculate() {
    double a_modifier = gd.lambda / (gd.c * gd.ro);

    double int_point1, int_point2;

    int_point1 = -0.5773502692;
    int_point2 = int_point1 * (-1);
    // Notice: poniewaz wagi ptk calkowania przy dwupunktowym sa rowne 1, nie uwzgledniam ich we wzorach

    double N1[2], N2[2];

    N1[0] = 0.5 * (1 - int_point1);
    N1[1] = 0.5 * (1 - int_point2);

    N2[0] = 0.5 * (1 + int_point1);
    N2[1] = 0.5 * (1 + int_point2);

    double dr = (gd.radius_max - gd.radius_min) / gd.elements_count;

    double time_step = ( dr * dr ) / ( 0.5 * a_modifier);

    double n_time = (gd.time_max / time_step);
    time_step = gd.time_max / n_time;

    double x = 0;

    // petla po wezlach ktora wpisze wspolrzedne oraz temp. poczatkowe do wezlow
    for (int i = 0; i < gd.nodes_count; i++) {
        nodes[i].temp = gd.temp_start;
        nodes[i].coord = x;
        x += dr;
    }

    double temp_tau[2];

    // petla po czasie
    for (int i = 0; i < n_time; i++) {
        // zerowanie macierzy globalnej
        SystemOfEquations sys;

        // petla po elementach
        for (int j = 0; j < gd.elements_count; j++) {

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
            resetMatrices();

            // petla po punktach calkowania
            for (int k = 0; k < gd.integration_points_count; k++) {
                double rp = (N1[k] * r1) + (N2[k] * r2);
                double tp_tau = N1[k] * temp_tau[0] + N2[k] * temp_tau[1];

                // obliczanie macierzy lokalnej
                this->elements[j].mat[0][0]
                        += ((gd.lambda * rp) / (dr))
                         + gd.c * gd.ro * dr * rp * std::pow(N1[k], 2) / time_step;

                this->elements[j].mat[0][1]
                        += ((gd.lambda * rp) / (dr))*(-1)
                         + (gd.c * gd.ro * dr * rp * N1[k] * N2[k]) / (time_step);

                this->elements[j].mat[1][0] = this->elements[j].mat[0][1];

                this->elements[j].mat[1][1]
                        += ((gd.lambda * rp) / (dr))
                         + (gd.c * gd.ro * dr * rp * std::pow(N2[k], 2)) / (time_step)
                         + (2 * alfa * gd.radius_max); // ostatni czynnik jest zerem jak nie ma konwekcji

                // obliczanie lokalnego wektora
                this->elements[j].vec[0][0] += (gd.c * gd.ro * dr * tp_tau * rp * N1[k]) / (time_step);

                this->elements[j].vec[1][0]
                        += (gd.c * gd.ro * dr * tp_tau * rp * N2[k]) / (time_step)
                         + (2 * alfa * gd.radius_max * gd.temp_air);
            } // koniec petli po punktach calkowania

            // niech kazdy element znajdzie swoje miejsce w macierzy globalnej
            sys.setSystem(this->elements, gd.elements_count);

        } // koniec petli po elementach skonczonych

        // rozwiaz uklad rownan w celu uzyskania nowych temperatur chwilowych
        std::vector<double> result;

        result = sys.solve();
        // przepisanie nowych wartosci temperatur do kazdego wezla
        for (int q = 0; q < result.size(); q++) {
            std::cout << result[q] << ";";
            nodes[q].temp = result[q];
        }
        std::cout << std::endl;
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

        this->elements[i].vec[0][0]
                = this->elements[i].vec[1][0]
                = 0.0;
    }
}

int main()
{
    GlobalData data;
    data.loadFromFile("wejscie.txt");

    FEMGrid grid(data);

    grid.calculate();

    return 0;
}

bool GlobalData::loadFromFile(std::string path) {
    std::fstream f(path.c_str(), std::ios_base::in);

    if (f.good()) {
        std::cout << "Pomyslnie otwarlem plik z danymi." << std::endl;
    } else {
        std::cout << "nie udalo sie otworzyc pliku wejsciowego.";
        return false;
    }

    f >> this->elements_count
            >> this->radius_min
            >> this->radius_max
            >> this->alfa
            >> this->temp_start
            >> this->temp_air
            >> this->c
            >> this->ro
            >> this->lambda
            >> this->time_max;

    f.close();

//    this->elements_count = 8;
//    this->radius_min = 0.0;
//    this->radius_max = 0.08;
//    this->alfa = 300;
//    this->temp_start = 100;
//    this->temp_air = 200;
//    this->c = 700;
//    this->ro = 7800;
//    this->lambda = 25;
//    this->time_max = 1000; // seconds

    this->nodes_count = this->elements_count + 1;

    this->integration_points_count = 2;

    return 0;
}
// metoda gaussa rozwiazywania ukladow rownan
std::vector<double> SystemOfEquations::solve() {

    for (int i = 0; i < rowsCount; i++) {
        double maxEl = std::abs(globalMatrix[i][i]);
        int maxRow = i;
        for (int k = i + 1; k < rowsCount; k++) {
            if (std::abs(globalMatrix[k][i]) > maxEl) {
                maxEl = std::abs(globalMatrix[k][i]);
                maxRow = k;
            }
        }

        for (int k = i; k < rowsCount + 1; k++) {
            double tmp = globalMatrix[maxRow][k];
            globalMatrix[maxRow][k] = globalMatrix[i][k];
            globalMatrix[i][k] = tmp;
        }
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
    std::vector<double> x(rowsCount);
    for (int i = rowsCount - 1; i >= 0; i--) {
        x[i] = globalMatrix[i][rowsCount] / globalMatrix[i][i];
        for (int k = i-1; k >= 0; k--) {
            globalMatrix[k][rowsCount] -= globalMatrix[k][i] * x[i];
        }
    }
    return x;
}
