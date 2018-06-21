#pragma once
#include "tableau.hpp"
#include <optional>
#include <vector>

template<class T>
using opt = std::optional<T>;
using std::vector;

template<class T>
class solution{
	public:
	T Z;
	vector<T> X;
};

template<class T>
class simplex{
	public:

	static opt<solution<T>> solve(tableau<T> tab){
		//algorithm goes here
		//Selecionar variavel que entra na base
		//realizar elimminação de gauss
		tab.print();
	};
};
