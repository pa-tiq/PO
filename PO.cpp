#include <iostream>
#include "tableau.hpp"
#include "tableau2.hpp"
#include "simplex.hpp"

int main(){
	/*
	tableau<float> t({2, 3, 1},
					{
						{1, 1, 1},
						{2, 1, -1},
						{3, 2, -1},
					},
					{40,20,30}
					);*/
	tableau2<double> t2({1, 1, 1},
					{
						{2, 1, -1},
						{1, 1, 2},
						{2, 1, 3},
					},
					{10,20,60},
					{comp::lessOrEqual, comp::greaterOrequal, comp::equal}
					);
	//simplex<float> s(t);
	//s.solve().value().print();
	return 0;
};
