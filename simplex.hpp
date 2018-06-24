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
	solution(T z, vector<T>x):Z(z), X(x){}
	void print(){
		cout << "Z* = " << Z << "\t\n";
		for(size_t i = 0; i < X.size(); ++ i)
			cout << "X" << i+1 << "=" <<  X.at(i) <<";\t\t\t";
		cout << endl;
	}
	solution(tableau<T> tab){
		X = vector<T>(tab.getDecisionVars().size());
		Z = tab[0][tab.getColumns()-1];
		for(size_t i = 1; i < X.size()+1; ++i){
			if(tab.isBase(i)){
				X.at(i-1) = tab[tab.getLineIndexBaseVar(i).value()][tab.getColumns()-1];
			}
		}
	}
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
				if(val < smallest && val > 0){ // TODO: Depois alterar para minimização
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
		for(auto& i : tab[linePivot]) i /= pivot;
		for(size_t line = 0; line < tab.getLines(); ++line){
			auto pivotLine = tab[line][columnPivot];
			for(size_t column = 1; column < tab.getColumns(); ++column){
				if(line != linePivot)
					tab[line][column] -= pivotLine*tab[linePivot][column];
			}
		}
	}
	simplex(tableau<T> tableau):tab(tableau){};

	opt<solution<T>> solve(){

		while(indexEnterVar()){
			auto in = indexEnterVar().value();
			auto out = indexExitVar(in).value();
			gaussElimination(out, in);
			tab.setBaseVar(in, out);
			/*debug
			cout << "in(col):" << in << "\tout(line):" << out << endl;
			for(const auto& [line, column] : tab.baseVars())
				cout << "line: " << line << "\t" << "column: " << column << endl;
			cout << "\n\n\n\n";
			tab.print();
			*/
		}
		//TODO: Critério de parada
		return solution<T>(tab);
	};
};
