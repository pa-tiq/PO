#include <iostream>
#include "tableau.hpp"
#include "tableau2.hpp"
#include "simplex.hpp"
#include "stdafx.h"

template<class T>
opt<tuple<tableau2<T>, solution<T>>> branchAndBound(tableau2<T> tableau);
double optimalSolution;

int main(){
	tableau2<double> t1({2, 3, 1},
					{
						{1, 1, 1},
						{2, 1, -1},
						{3, 2, -1},
					},
					{40,20,30},
					{comp::lessOrEqual, comp::lessOrEqual, comp::lessOrEqual}
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

	optimalSolution = 0.0;
	branchAndBound(t4);

	system("pause");
	return 0;	
};

template<class T>
opt<tuple<tableau2<T>, solution<T>>> branchAndBound(tableau2<T> tableau)
{
	bool branched = false;
	double intpart;
	double decimalpart;
	tableau2<T> copyTableau = tableau;

	if (auto tmp = simplex<double>::solve(tableau))
	{
		auto[tab, sol] = tmp.value();
		sol.print();

		if (tableau.maximize)
		{
			if (sol.Z < optimalSolution) return {};
			else optimalSolution = sol.Z;
		}
		else
		{
			if (sol.Z > optimalSolution) return {};
			else optimalSolution = sol.Z;
		}

		for (int i = 0; i < sol.X.size(); i++)
		{
			if ((int)sol.X[i] == 0) continue;
			
			decimalpart = std::modf(sol.X[i], &intpart);

			if (decimalpart > 0.0001)
			{
				branched = true;
				cout << endl;

				copyTableau.addRestriction(i, comp::lessOrEqual, intpart);
				auto ltmp = branchAndBound(copyTableau);

				copyTableau = tableau;

				copyTableau.addRestriction(i, comp::greaterOrequal, intpart + 1);
				auto rtmp = branchAndBound(copyTableau);
			}
		}

		if (!branched && sol.Z == optimalSolution)
		{
			cout << "\n\nSOLUCAO OTIMA ENCONTRADA!\n\n";
			return tmp;			
		}
			

	}
	else return {};
}
