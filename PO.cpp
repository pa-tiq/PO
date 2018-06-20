#include <iostream>
#include "tableau.hpp"

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
	return 0;
};
