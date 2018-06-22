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
		if(valid) return indexSmallest;
		else return {};
	}
	public:


	void gaussElimination(size_t linePivot, size_t columnPivot){
		auto pivot = tab[linePivot][columnPivot];
		for(size_t line = 0; line < tab.getLines(); ++line){
			auto pivotLine = tab[line][columnPivot];
			for(size_t column = 1; column < tab.getColumns(); ++column){
				if(line == linePivot)
					tab[line][column] /= pivotLine;
				else
					tab[line][column] = -pivotLine*tab[linePivot][column]/pivot+tab[line][column];
			}
		}
	}
	simplex(tableau<T> tableau):tab(tableau){};

	opt<solution<T>> solve(){

		while(indexEnterVar()){
			auto in = indexEnterVar().value();
			auto out = indexExitVar(in).value();
			gaussElimination(out, in);
			/*TODO: está errado
			*
			* Out é indice da linha da variável que está na base( na eliminação de gauss);
			* porém deve ser adicionado à base é o indice da coluna
			*
			*/
			//debug
			tab.setBaseVar(in, out);
			cout << "in:" << in << "\tout:" << out << endl;
			for(const auto& i : tab.baseVars())
				cout << i << "\t";
			cout << "\n\n\n\n";
			tab.print();
		}
		//end debug
		//TODO: Critério de parada
		//TODO: construir classe solution
		return{};
	};
};
