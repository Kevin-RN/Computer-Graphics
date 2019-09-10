For the Statistics assignment, you must write a method which provides a number of
simple statistics. The input given is a list of 
oat values. These values are all
valid, correct values (we will not give you nasty input, such as invalid 
oats or
other datatypes). The output should be a pair of values, where the rst value
is the average of all the values in the list, and the second value is the standard
deviation of list of values.

An example could be an input list of [-4, -2, 0, 0, 0, 2, 2, 2], which should
have an output of (0.0, 2.0).

```cpp
#include <list>
#include <set>
#include <numeric>
#include <math.h>

////////////////// Exercise 1 ////////////////////////////////////
std::pair<float, float> Statistics(const std::list<float>& values)
{
	float avg = (std::accumulate(std::begin(values), std::end(values), 0.0)) / values.size(); // calculates mean
	
	//calculates standard deviation
	float temp = 0.f;
	for(auto const& i : values) {
		temp += pow(i-avg, 2);
	};
	float dev = sqrt(temp / values.size());

	return std::pair<float, float>(avg, dev);
};
```
Test:
```cpp
std::list<float> stats;
	stats.push_back(-4.f);
	stats.push_back(-2.f);
	stats.push_back(0.f);
	stats.push_back(0.f);
	stats.push_back(0.f);
	stats.push_back(2.f);
	stats.push_back(2.f);
	stats.push_back(2.f);

	std::pair<float, float> statsAnswer = Statistics(stats);

	if (equalFloats(statsAnswer.first, 0.f) && equalFloats(statsAnswer.second, 2.0))
	{
		std::cout << "Exercise Statistics: Correct" << std::endl;
	}
	else
	{
		std::cout << "Exercise Statistics: Incorrect" << std::endl;
 }
 ```
