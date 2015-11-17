//
// Created by szympeg on 11/17/15.
//

#ifndef MES_FINITEELEMENT_H
#define MES_FINITEELEMENT_H

#include <vector>
#include "Node.h"

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

#endif //MES_FINITEELEMENT_H
