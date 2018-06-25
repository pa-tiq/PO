#include <iostream>
#include "tableau.hpp"
#include "tableau2.hpp"
#include "simplex.hpp"

int main(){
	tableau<float> t({2, 3, 1},
					{
						{1, 1, 1},
						{2, 1, -1},
						{3, 2, -1},
					},
					{40,20,30}
					);
	tableau2<double> t2({1, 1, 1},
					{
						{2, 1, -1},
						{1, 1, 2},
						{2, 1, 3},
					},
					{10,20,60},
					{comp::lessOrEqual, comp::greaterOrequal, comp::equal}
					);
	tableau2<double> t3({4, 1, 0},
					{
						{3, 1, 0, 0},
						{4, 3, -1, 0},
						{1, 2, 0, 1},
					},
					{3,6,4},
					{comp::equal, comp::equal, comp::equal}
					);
	tableau2<double> t4({0.4, 0.5},
					{
						{0.3, 0.1},
						{0.5, 0.5},
						{0.6, 0.4},
					},
					{2.7,6,6},
					{comp::lessOrEqual, comp::equal, comp::greaterOrequal},
					false
					);
	if(auto tmp = simplex<double>::solve(t4)){
		auto [tab, sol] = tmp.value();
		sol.print();
	} else cout << "Solução artificial é inválida\n";
	return 0;
};
