#pragma once
#include "tableau.hpp"
#include <optional>
#include <vector>
#include <tuple>
#include <iostream>
#include <limits>

template<class T>
using opt = std::optional<T>;
using std::vector;
using std::tuple;
using std::iostream;

template<class T>
class solution{
	public:
	T Z;
	vector<T> X;
};

template<class T>
class simplex{
	tableau<T> tab;
	opt<vector<tuple<T, size_t>>> getCoefs(){
		vector<tuple<T, size_t>> ret;
		for(size_t i = 1; i < tab.getColumns()-1; ++i ){
			if(tab[0][i] < 0){ // TODO: válido apenas para maximizar, depois fazer alerações para minimização
				ret.push_back(tuple<T,size_t>(tab[0][i], i));
			}
		}
		if(ret.empty()) return {};
		else return ret;
	}
	opt<size_t>  indexEnterVar(){
		T smallest = T();
		size_t indexSmallest;
		bool valid = false;
		if(const auto& vec = getCoefs())
			for(const auto& [val, index] : vec.value()){
				if(val < smallest && val < 0){ // TODO,: Depois alterar para minimização
					valid = true;
					smallest = val;
					indexSmallest = index;
				}
			}
		else return {};
		if(valid) return indexSmallest;
		else return {};
	}

	opt<vector<tuple<T, size_t>>> getRatios(T indexEnterVar){
		vector<tuple<T, size_t>> ret;
		auto column = tab.getColumns()-1;
		for(size_t i = 1; i < tab.getLines(); ++i ){
			if(tab[i][column] > 0){ // TODO: válido apenas para maximizar, depois fazer alerações para minimização
				ret.push_back(tuple<T,size_t>(tab[i][column]/tab[i][indexEnterVar], i));
			}
		}
		if(ret.empty()) return {};
		else return ret;
	}

	opt<size_t> indexExitVar(T indexEnterVar){
		T smallest = std::numeric_limits<T>::max();
		size_t indexSmallest;
		bool valid = false;
		if(const auto& vec = getRatios(indexEnterVar))
			for(const auto& [val, index] : vec.value()){
				if(val < smallest && val > 0){ // TODO,: Depois alterar para minimização
					valid = true;
					smallest = val;
					indexSmallest = index;
				}
			}
		else return {};
		if(valid) return indexSmallest;
		else return {};
	}
	public:
	simplex(tableau<T> tableau):tab(tableau){};

	opt<solution<T>> solve(){
		//algorithm goes here
		//Selecionar variavel que entra na base
		//Variável básica -> var identidade
		//realizar elimminação de gauss
		/*
		if(auto tmp = getCoefs())
			for(const auto& i: tmp.value())
		cout << std::get<0>(i) << "\t";
		*/
		cout << indexEnterVar().value() << endl;
		cout << indexExitVar(indexEnterVar().value()).value() << endl;
		return{};
	};
};
