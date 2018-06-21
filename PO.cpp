#include <iostream>
#include "tableau.hpp"
#include "simplex.hpp"

int main(){
	tableau<float> t({2, 3, 1},
					{
						{1, 1, 1},
						{2, 1, -1},
						{3, 2, -1},
						{3, 2, -1}
					},
					{40,20,30,10},
					false
					);
	t.print();
	return 0;
};
