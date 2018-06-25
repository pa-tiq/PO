#pragma once
#include "tableau.hpp"
#include "tableau2.hpp"
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
	protected:
	static opt<vector<tuple<T, size_t>>> getCoefs(tableau<T> tab){
		vector<tuple<T, size_t>> ret;
		for(size_t i = 1; i < tab.getColumns()-1; ++i ){
			if(tab[0][i] < 0){
				ret.push_back(tuple<T,size_t>(tab[0][i], i));
			}
		}
		if(ret.empty()) return {};
		else return ret;
	}

	static opt<size_t>  indexEnterVar(tableau<T> tab){
		T smallest = T();
		size_t indexSmallest;
		bool valid = false;
		if(const auto& vec = getCoefs(tab))
			for(const auto& [val, index] : vec.value()){
				if(val < smallest && val < 0){
					valid = true;
					smallest = val;
					indexSmallest = index;
				}
			}
		if(valid) return indexSmallest;
		else return {};
	}

	static opt<vector<tuple<T, size_t>>> getRatios(tableau<T> tab, T indexEnterVar){
		vector<tuple<T, size_t>> ret;
		auto column = tab.getColumns()-1;
		for(size_t i = 1; i < tab.getLines(); ++i ){
			if(tab[i][column] > 0){
				ret.push_back(tuple<T,size_t>(tab[i][column]/tab[i][indexEnterVar], i));
			}
		}
		if(ret.empty()) return {};
		else return ret;
	}

	static opt<size_t> indexExitVar(tableau<T> tab,T indexEnterVar){
		T smallest = std::numeric_limits<T>::max();
		size_t indexSmallest;
		bool valid = false;
		if(const auto& vec = getRatios(tab, indexEnterVar))
			for(const auto& [val, index] : vec.value()){
				if(val < smallest && val > 0){
					valid = true;
					smallest = val;
					indexSmallest = index;
				}
			}
		if(valid) return indexSmallest;
		else return {};
	}

	static void gaussElimination(tableau<T>& tab, size_t linePivot, size_t columnPivot){
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

	public:

	static opt<tuple<tableau2<T>, solution<T>>> solveFirst(tableau2<T> tab){
		while(indexEnterVar(tab)){
			auto in = indexEnterVar(tab).value();
			auto out = indexExitVar(tab, in).value();
			gaussElimination(tab, out, in);
			tab.setBaseVar(in, out);
			/*
					cout << "in(col):" << in << "\tout(line):" << out << endl;
					for(const auto& [line, column] : tab.baseVars())
						cout << "line: " << line << "\t" << "column: " << column << endl;
					tab.print();
					cout << "\n\n\n\n";
			*/
		}
		return tuple(tab, solution<T>(tab));
	}

	static opt<tuple<tableau<T>, solution<T>>> solve(tableau<T> tab){
		while(indexEnterVar(tab)){
			auto in = indexEnterVar(tab).value();
			auto out = indexExitVar(tab, in).value();
			gaussElimination(tab, out, in);
			tab.setBaseVar(in, out);
			/*
					cout << "in(col):" << in << "\tout(line):" << out << endl;
					for(const auto& [line, column] : tab.baseVars())
						cout << "line: " << line << "\t" << "column: " << column << endl;
					cout << "\n\n\n\n";
			*/
		}
		return tuple(tab, solution<T>(tab));
	}

	static opt<tuple<tableau<T>, solution<T>>> solve(tableau2<T> tab){
		if(auto tup = simplex<T>::solveFirst(tab)){
			auto [tab2, sol2] = tup.value();

			//sol2.Z é a solução do problema articial. Se for diferente de zero, significa que as restriçẽso são inconsistentes
			if(abs(sol2.Z) > std::numeric_limits<T>::epsilon() ){
				return {};
			}

			tableau<T> test = tab2.get2fase();
			/*
			cout << "Solução da primeira fase está concluída, fazendo segunda fase" << endl;
			for(const auto& [line, column] : test.baseVars())
				cout << "line: " << line << "\t" << "column: " << column << endl;
			test.print();
			*/
			return simplex<T>::solve(test);
		} else return {};
	}
};
