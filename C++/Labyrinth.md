You're stuck in a labyrinth and need to nd a way out. The labyrinth is of size
size * size, and is always a square. You need to get from tile (0, 0) to tile (size- 1, size - 1). You can only move between adjacent squares (up, down, left, right), and you cannot move diagonally. The function you are given is defned
as shown below.

```c
typedef std::pair<int, int> location;

int Labyrinth(std::set<std::pair<location, location> > labyrinth, int size);
```

The edges of the labyrinth have been blocked o (meaning you cannot pass
through them). You are also given a set of walls that indicate adjacent pairs
in the labyrinth you cannot move between. For example, if you are given the
input set shown below:

[( (0,0) , (1,0) ),  
 ( (0,1) , (1,1) ),  
 ( (0,2) , (0,3) ),  
 ( (1,1) , (1,2) ),  
 ( (1,2) , (2,2) ),  
 ( (2,3) , (3,3) ),  
 ( (2,2) , (3,2) ),  
 ( (2,1) , (3,1) ) )]
 
 You will get a labyrinth that looks like this:
 (see pdf document.....)
 
 The method you implement should return the length of the shortest possible
path. Each tile crossed counts for 1 unit of length (so crossing 5 tiles is a length
of 5, 6 tiles is a length of 6, and so on). If no path exists or the labyrinth
cannot be exited, you must return a value of 0. In the example given above, the
shortest possible length is 13.

**Test:**
```c
std::set<std::pair<location, location> > labyrinth;
	labyrinth.insert(std::pair<location, location>(location(0, 0), location(1, 0)));
	labyrinth.insert(std::pair<location, location>(location(0, 1), location(1, 1)));
	labyrinth.insert(std::pair<location, location>(location(0, 2), location(0, 3)));
	labyrinth.insert(std::pair<location, location>(location(1, 1), location(1, 2)));
	labyrinth.insert(std::pair<location, location>(location(1, 2), location(2, 2)));
	labyrinth.insert(std::pair<location, location>(location(2, 3), location(3, 3)));
	labyrinth.insert(std::pair<location, location>(location(2, 2), location(3, 2)));
	labyrinth.insert(std::pair<location, location>(location(2, 1), location(3, 1)));

	int labAnswer = Labyrinth(labyrinth, 4);
	std::cout << labAnswer << std::endl;
	if (labAnswer == 13)
	{
		std::cout << "Exercise: Correct" << std::endl;
	}
	else
	{
		std::cout << "Exercise: Incorrect" << std::endl;
	}
  ```
  
  
  **Solution:**
  ```c
  typedef std::pair<int, int> location;

//create neighbour locations for the current location (up, down, left, right)
location getNeighbour(location current, int direction) {
	switch (direction) {
		case 1:
			return location(current.first - 1, current.second);
		case 2:
			return location(current.first, current.second - 1);
		case 3:
			return location(current.first + 1, current.second);
		default:
			return location(current.first, current.second + 1);
	}
}

int Labyrinth(std::set<std::pair<location, location> > labyrinth, int size)
{
	std::queue<location> search;
	std::set<location> path;

	location start = location(0, 0);
	location dest = location(size - 1, size - 1);
	search.push(start);

	while (!search.empty()) {
		location current = search.front();

//if the current node is the destination
		if (current.first == dest.first && current.second == dest.second) return path.size();

//remove the current position from the queue to avoid infinite loop
		search.pop();

		for(int i = 0; i < 4; i++)
		{
			location neighbour = getNeighbour(current, i);

			std::pair<location, location> way = std::pair<location, location>(current, neighbour);
//check if inside maze
			if (neighbour.first > -1 && neighbour.first < size && neighbour.second > -1 && neighbour.second < size) {
//check if it isn't already discovered and also if there isn't a wall
				if ((path.find(neighbour) == path.end()) && (labyrinth.find(way) == labyrinth.end())) {
					path.insert(current);
					search.push(neighbour);
				}
			}
		}
	}
	return 0;
}

```
 
