#ifndef LSYS_HPP
#define LSYS_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <functional> //std::function
#include <sstream> //std::ostringstream

namespace lsys {

using Symbol = char;
using Params = std::vector<float>;

/** A module is a symbol equipped with parameters **/
struct Module {
	Symbol symbol;
	Params params;
};

using Word = std::vector<Module>;
using Axiom = Word;

/** A rule defined a mapping from a module, whose symbol is equal to that of the rules predecessor,
	to another module **/
struct Rule {
	Symbol predecessor;
	Word successor;
	std::function<Params(Params)> transition;
	float weight;

	Rule(Symbol predecessor, std::string successor, std::function<Params(Params)> transition, float weight);
};

/** A parametric, stochastic and bracketed L-system
	This class is not thread-safe. **/
class Lsystem {
private:
	Axiom axiom;
	Word word;
	std::unordered_map<Symbol, std::vector<Rule>> rules; //All rules accesible by its symbol
	std::unordered_map<Symbol, float> weightSums; //Sum of weights of all rules applicable to symbol
	std::unordered_map<Symbol, size_t> paramsPerSymbol;

public:

	Lsystem(const Axiom& axiom, const std::vector<Rule>& rules);
	template<typename ...Ts>
	Lsystem(const Axiom& axiom, const std::vector<Rule>& rules, const Symbol symbol, size_t numParams, Ts... ts) : Lsystem(axiom, rules, ts...) {
		paramsPerSymbol.insert({symbol, numParams});
	}

	/** Iterate the Lsystem n times **/
	Lsystem& next(int n = 1);

	/** Read the string of the system at it's current state **/
	std::string str() const;
	const Word& read() const;


	static void test();
};

}; //end of namespace lsys

std::ostream &operator<<(std::ostream &os, const lsys::Module& m);
std::ostream &operator<<(std::ostream &os, const lsys::Word& w);
std::ostream &operator<<(std::ostream &os, const lsys::Lsystem& l);

#endif //LSYS_HPP