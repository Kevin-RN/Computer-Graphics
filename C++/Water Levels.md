You are given a structure with diering heights at dierent indexes. You are
tasked with calculating the total possible water that can be held within the
structure due to indexes diering in heights.

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

Solution:


Test:
