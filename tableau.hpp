#pragma once
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

template<class N = float>
//N é o tipo númerico.
class tableau{
	vector<vector<N>> data;
	const bool maximize;
	vector<size_t> indexBaseVars;
	size_t columns;
	size_t lines;
	public:
	tableau(vector<N> Z, //Z são as constantes da função objetiva
			vector<vector<N>> A, //A são as constantes da  matriz de restrições
			vector<N> b, //b é o lado direito das restrições
			//vector<T4> lowerBound,
			bool max = true
			) : maximize(max){

		//primeiramente testarei se as dimensões são válidas
		if( A.size() != b.size()) throw;

		columns = Z.size() + A.size() + 2;
		lines = A.size() + 1;

		//fill tableau with 0's
		data = vector<vector<N>>(lines);
		for(auto& line : data)
			for(size_t i = 0; i < columns; ++i)
				line.push_back(N());

		//Z
		data[0][0] = maximize ? 1 : -1;
		for(size_t i  = 1; i < Z.size()+1; ++i){
			data[0][i] = (maximize ? -1 : 1)*Z[i-1];
		}

		//A
		for(size_t line = 1; line < A.size()+1; ++line){
			for(size_t column = 1; column < Z.size()+1; ++column){
				data[line][column] = A[line-1][column-1];
			}
		}

		//S
		for(size_t i = Z.size()+1; i < Z.size()+A.size()+1; ++i){
			data[i-Z.size()][i] = 1;
		}

		//b
		for(size_t i = 1; i < b.size()+1; ++i){
			data[i][columns-1] = b[i-1];
		}

		//
		for(size_t i = Z.size() + 1; i < columns-1; ++i){
			indexBaseVars.push_back(i);
		}
	}

	tableau():maximize(true){
	}

	void print(){
		for(const auto& line: data){
			for(const auto& var: line)
				cout << var << "\t";
			cout << endl;
		}
	};

	vector<N>& operator[](const size_t i){ return data[i]; }

	vector<size_t> baseVars() const {
		return indexBaseVars;
	}

	size_t getColumns(){ return columns; }
	size_t getLines(){ return lines; }

	//TODO: alterar para permitir simplex de duas fases
	//TODO: add caso para restrições <=, = e >=. Aka var auxiliar
	//TODO: aonde no tableau entra as restrições de nulidade/lowerBound?
};
