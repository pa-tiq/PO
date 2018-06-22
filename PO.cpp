#include <iostream>
#include "tableau.hpp"
#include "simplex.hpp"

int main(){
	tableau<float> t({2, 3, 1},
					{
						{1, 1, 1},
						{2, 1, -1},
						{3, 2, -1}
					},
					{40,20,30}
					);
	t.print();
	simplex<float> s(t);
	s.solve();
	/*
	auto tmp = t.baseVars();
	for(const auto& i: tmp)
		cout << i << "\t";
	cout << endl;
	*/
	return 0;
};
