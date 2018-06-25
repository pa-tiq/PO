#pragma once
#include  "tableau.hpp"

enum class comp{ //comparison
	lessOrEqual,
	equal,
	greaterOrequal
};

template <class M>
class tableau2: public tableau<M>{
	using super = tableau<M>;
	vector<comp> comparison;

	template <comp lhs>
	static size_t numberComp(const vector<comp>& comparison){
		size_t count = 0;
		for(auto& i: comparison)
			if(lhs == i)
				count++;
		return count;
	}

	static size_t numberSlacks(const vector<comp>& comparison){
		return numberComp<comp::lessOrEqual>(comparison)
			+  numberComp<comp::greaterOrequal>(comparison);
	}

	static size_t numberAuxVars(const vector<comp>& comparison){
		return numberComp<comp::equal>(comparison)
			+  numberComp<comp::greaterOrequal>(comparison);
	}

	public:
	tableau2(	vector<M> Z,
				vector<vector<M>> A,
				vector<M> b,
				vector<comp> comparison,
				bool max = true
			):comparison(comparison){
		super::maximize = max;
		super::z = Z;

		if(A.size() != b.size()) throw ;
		if(comparison.size() != b.size()) throw ;
		super::columns = Z.size()+2
			+ numberComp<comp::lessOrEqual>(comparison) //Adiciona uma coluna para slack
			+ 2*numberComp<comp::equal>(comparison) //Adiciona coluna para slack e auxVar
			+ numberComp<comp::greaterOrequal>(comparison); //Adiciona uma coluna para auxVar
		super::lines = A.size()+2;

		//fill tableau with 0's
		super::data = vector<vector<M>>(super::lines);
		for(auto& line : super::data){
			for(size_t i = 0; i < super::columns; ++i)
				line.push_back(M());
		}

		//A seguir é a definição da estrutura do tableau.
		//Zart
		//Z
		//R1
		//R2 ...

		//Z
		super::data[1][0] = super::maximize ? 1 : -1;
		for(size_t i = 1; i < Z.size()+1; ++i){
			super::data[1][i] = (super::maximize ? -1 : 1)*Z[i-1];
			}

		//R
		size_t slackCount = 0;
		size_t auxCount = 0;
		for(size_t line = 2; line < super::lines; ++line){
			size_t offset = 1;
			const auto& thisLineComparison = comparison[line-2];
			for(size_t i = offset; i < Z.size()+offset; ++i)
				super::data[line][i] = A[line-2][i-offset];
			offset = Z.size()+1;
			if(thisLineComparison == comp::lessOrEqual){
				super::data[line][offset+slackCount] = 1;
				super::indexBaseVars.push_back(pos(line, offset+slackCount));
				slackCount++;
			} else if(thisLineComparison == comp::greaterOrequal){
				super::data[line][offset+slackCount] = -1;
				super::data[line][offset+numberSlacks(comparison)+auxCount] = 1;
				super::indexBaseVars.push_back(pos(line, offset+numberSlacks(comparison)+auxCount));
				slackCount++;
				auxCount++;
			} else if(thisLineComparison == comp::equal){
				super::data[line][offset+numberSlacks(comparison)+auxCount] = 1;
				super::indexBaseVars.push_back(pos(line, offset+numberSlacks(comparison)+auxCount));
				auxCount++;
			}
		}

		//b
		for(size_t i = 2; i < b.size()+2; ++i){
			super::data[i][super::columns-1] = b[i-2];
		}

		//Zart
		super::data[0][0] = -1;
		for(size_t col = 1; col < Z.size()+numberSlacks(comparison)+1; ++col){
			for(size_t line = 2; line < super::lines; line++){
				const auto& thisLineComparison = comparison[line-2];
				if(thisLineComparison != comp::lessOrEqual){
					super::data[0][col] -= super::data[line][col];
				}
			}
		}
		for(size_t line = 2; line < super::lines; line++){
			const auto& thisLineComparison = comparison[line-2];
			if(thisLineComparison != comp::lessOrEqual){
				super::data[0].back() += super::data[line].back();
			}
		}
		super::data[0].back() = -super::data[0].back();
		/*
		for(auto& [line, column] : super::baseVars())
			cout << "line:" << line << "\t column:" << column << endl;
		super::print();
		*/
	}

	tableau<M> get2fase(){
		//remove 1°line
		tableau<M> ret= *this;
		for(size_t line = 0; line < super::lines-1; ++line){
			ret.data[line] = ret.data[line+1];
		}
		ret.data.pop_back();
		for(size_t line = 0; line < super::lines; ++line){
			ret.data[line][ret.data[0].size()-numberAuxVars(comparison)-1] = ret.data[line].back();
		}
		for(size_t line = 0; line < super::lines; ++line){
			for(size_t i = 0; i < numberAuxVars(comparison);++i)
				ret.data[line].pop_back();
		}
		for(auto& [line, column]: ret.indexBaseVars)
			line -= 1;

		ret.lines = ret.data.size();
		ret.columns = ret.data[0].size();
		return ret;
	}
};
