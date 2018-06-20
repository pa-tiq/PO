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
	vector<T> Xi;
};

template<class T>
class simplex{
	public:
	static opt<solution<T>> solve(tableau<T> tab){
		//algorithm goes here
		tab.print();
	};
};
