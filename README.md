# lsys
[![Build Status](https://travis-ci.org/Kuxe/lsys.svg?branch=master)](https://travis-ci.org/Kuxe/lsys)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/kuxe/lsys?svg=true)](https://ci.appveyor.com/project/Kuxe/lsys)

lsys is a tiny C++ library which implements a bracketed, stochastic and parametric Lindenmayer system (L-system).

## Usage
In `example.cpp` there is a short, self contained, correct (compilable, with `-std=c++14`) example:

```C++
/** A short compilable example (c++14) demonstrating how to construct and use lsys.
	It is also illustrated how a turtle could be implemented for use with lsys. **/

#include "lsys.hpp"

int main(int argc, char** argv) {
	using namespace lsys;
	/** Two (production) rules r1 and r2 which goes from A to [AB] and B to A respectively.
		The lambdas define how parameters of the predecessor carry over to the successor.
		Here "[](Params p) -> Params { return {0.5f*p[0], 0.5f*p[0]}; }, 1.0f}" seem arcane.
		Fear not, in plain english this means:
			"The first parameter of the successor is half of first parameter of predecessor,
			 the second parameter of the successor is half of the first parameter of predecessor,
			 the third parameter of the successor is 1"
		If there are several rules with the same predecessor, then the last parameter is the
		weight. The heavier the stochastic rule is, the more likely it is for selection. **/
	Rule r1 = {'A', "[AB]", [](Params p) -> Params { return {0.5f*p[0], 0.5f*p[0]}; }, 1.0f};
	Rule r2 = {'B', "A", [](Params p) -> Params { return {2.0f*p[0]}; }, 1.0f};
	Lsystem lsystem = {{{'A', {1.0f}}}, {r1, r2}, 'A', 1, 'B', 1};
	
	/** The example source code now proceeds to illustrate how lsys can be used with
		some (user supplied) turtle-lambda to create a 3D-model. **/
	struct Custom3DModel { /** Just some arbitrary struct holding 3D-data **/ };
	Custom3DModel model = [](const Word& word) -> Custom3DModel {
		Custom3DModel model;
		for(const Module& module : word) {
			switch(module.symbol) {
				case 'A': /** Forward turtle, possibly parametrized by model.params **/ break;
				case 'B': /** Rotate turtle, possibly parametrized by model.params **/break;
				case '[': /** Push turtle **/; break;
				case ']': /** Pop turtle **/; break;
			}
		}
		return model;
	}(lsystem.next(4).read());

	return 0;
}
```
That's it. To get started, copy `lsys.hpp` into your include-directory and `lsys.cpp` into your source-directory.

Happy coding :+1:!