You are given a structure with differing heights at different indexes. You are
tasked with calculating the total possible water that can be held within the
structure due to indexes differing in heights.

You are also given a method called WaterLevels which takes an std::list10 con-
taining the heights at each index. The method is defined as shown below.

```c
  float WaterLevels(std::list<float> heights);
 ```
 
 Your goal is to implement the method such that it calculates the total water
that can be stored within one of the structures. Note that no water can be held
at the end points (so if you have end points that are lower than their direct
neighbors, it cannot hold water, such as the case with the rightmost index in
the example provided above). The output must only be a 
oat containing the
water levels. For an input of [7, 6, 4, 7, 6, 9, 3, 1, 5, 3], an output of 11 is
expected.

Test:
```c
	std::list<float> heights;
	heights.push_back(7);
	heights.push_back(6);
	heights.push_back(4);
	heights.push_back(7);
	heights.push_back(6);
	heights.push_back(9);
	heights.push_back(3);
	heights.push_back(1);
	heights.push_back(5);
	heights.push_back(3);

	float waterAnswer = WaterLevels(heights);

	if (equalFloats(waterAnswer, 11.f))
	{
		std::cout << "Exercise 4: Correct" << std::endl;
	}
	else
	{
		std::cout << "Exercise 4: Incorrect" << std::endl;
	}
```

Solution:
```c
float WaterLevels(std::list<float> heights) {
	float level = 0.f;
	float left = 0, right = 0;

	//turn list into a vector for easier access
	std::vector<float> hvector(heights.size());
	std::copy(heights.begin(), heights.end(), hvector.begin());

	float l = 0, r = heights.size() - 1;


	while (l <= r) {
		if (hvector[l] < hvector[r])
		{
//if value of current left is bigger than max left, update value else substract max left with current left and add it to total level.
			if (hvector[l] > left)
				left = hvector[l];
			else
				level += left - hvector[l];
			l++;
		}
		else
		{
//if value of current right is bigger than max right, update value else substract max right with current right and add it to level.
			if (hvector[r] > right)
				right = hvector[r];
			else
				level += right - hvector[r];
			r--;
		}
	}

	return level;
}
```
