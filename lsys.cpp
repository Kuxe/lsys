#include "lsys.hpp"
#include <chrono> //std::chrono::high_resolution_clock used with random
#include <random> //std::uniform_real_distribution etc

namespace lsys {

Rule::Rule(Symbol predecessor, std::string successor, std::function<Params(Params)> transition, float weight) :
	predecessor(predecessor), transition(transition), weight(weight) {
	for(const char c : successor) this->successor.push_back({c});
}

Lsystem::Lsystem(const Axiom& axiom, const std::vector<Rule>& rules) : axiom(axiom), word(axiom), paramsPerSymbol {{'[', 0}, {']', 0}} {
	for(const Rule& rule : rules) {
		this->rules[rule.predecessor].push_back(rule);
		weightSums[rule.predecessor] += rule.weight;
	}
}

Lsystem& Lsystem::next(int n) {
	for(int i = 0; i < n; i++) {
		Word nextWord;
		//Iterate over each module and overwrite it
		for(const Module& module : word) {
			const Symbol& symbol = module.symbol;
			if(symbol == '[' || symbol == ']') {
				nextWord.push_back({symbol});
			} else {
				/** Decide on rule to use for this module using a russian roulette on rule weights **/
				const float r = weightSums[symbol] * []() {
					//This is needed due to bug in gcc on windows, see:
					//https://stackoverflow.com/questions/34680805/why-is-random-library-producing-the-same-results-every-time-when-using-stdun
					using namespace std::chrono;
					static std::default_random_engine gen{static_cast<unsigned int>(high_resolution_clock::now().time_since_epoch().count())};
					static std::uniform_real_distribution<float> unidist(0.0f, 1.0f);
					return unidist(gen);
				}();
				float cumsum = 0.0f;
				for(const Rule& rule : rules[symbol]) {
					cumsum += rule.weight;
					if(r <= cumsum) {
						/** If a matching rule was found (this need not be the case if there's no rule defined on a module)
							Then append its successor to the nextWord **/
						const Params updatedParams = rule.transition(module.params);

						/** Now I need to distribute the updatedParams over all modules in successor **/
						size_t parami = 0;
						/** TODO: This loop makes program considerably slower.
							Maybe parameters should just be member of Lsystem instead of member of module?
							This "fix" would make it a pain to turtle the word from the Lsystem though **/
						for(const Module& successorModule : rule.successor) {
							nextWord.push_back({successorModule.symbol, std::vector<float>(updatedParams.begin()+parami, updatedParams.begin()+parami+paramsPerSymbol[successorModule.symbol])});
							parami += paramsPerSymbol[successorModule.symbol];
						}
						break;
					}
				}
			}
		}
		word = nextWord;
	}
	return *this;
}

std::string Lsystem::str() const {
	std::ostringstream oss;
	oss << word;
	return oss.str();
}

const Word& Lsystem::read() const {
	return word;
}

void Lsystem::test() {
	//Two rules which alternate A and B but doubling the parameter on each iteration
	Rule r1 = {'A', "[AB]", [](Params p) -> Params { return {0.5f*p[0], 0.5f*p[0]}; }, 1.0f};
	Rule r2 = {'B', "A", [](Params p) -> Params { return {2.0f*p[0]}; }, 1.0f};
	{
		Module m = {'A', {1.0f}};
		Axiom axiom= {m};
		Lsystem lsys(axiom, {r1, r2}, 'A', 1, 'B', 1);
		const char* expected = "[[[A(0.125)B(0.125)]A(0.5)][A(0.5)B(0.5)]]";
		if(lsys.next(3).str() != expected) {
			printf("Got %s, expected %s\n", lsys.str().c_str(), expected); 
		}
	}
}

}; //end of namespace lsys

std::ostream &operator<<(std::ostream &os, const lsys::Module& m) { 
	if(m.params.size() == 0) return os << m.symbol;
	os << m.symbol << "(";
	for(size_t i = 0; i < m.params.size()-1; i++) os << m.params[i] << ", ";
	return os << m.params[m.params.size()-1] << ")";
}

std::ostream &operator<<(std::ostream &os, const lsys::Word& w) {
	for(const lsys::Module& m : w) os << m;
	return os;
}

std::ostream &operator<<(std::ostream &os, const lsys::Lsystem& l) { 
	return os << l.str();
}