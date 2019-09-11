For the Tree Traversal assignment, you need to implement a method that will perform
tree traversal. As you might recall from Algorithms & Datastructures, a tree is
a data structure in which each node has at most two children, and at most one
parent. In the given tree, each node has a value, dened as a 
oat.
The tree functionality is defined behind-the-scenes in the codebase as shown below.
```c
Class Tree { 
    public: 
      float value;
      std::list<Tree> children;
};

float TreeTraversal(const Tree& t, TreeVisitor& visitor, bool countOnlyEvenLevels) {
  return visitor.visitTree(t, countOnlyEvenLevels);
}
```

The Treevisitor class is defined partially below.

```c
class Treevisitor {
    public:
      float visitTree(const Tree& tree, bool countOnlyEvenLevels);
 }
 ```
Part 1: Treevisitor You need to write a function called Treevisitor::visitTree
such that when this method is called, it traverses over all the nodes in the tree
and returns the total value found. Thus, if you iterate over a tree with nodes
worth 3, 5, and 6, you need to return the sum of total values (3 + 5 + 6) = 14.

NOTE: for both part 1 and part 2, you should not modify the TreeTraver-
sal function.

Part 2: Only Even Nodes If the boolean countOnlyEvenLevels is true,
you should only sum the tree values from nodes that are on even levels. Note
that 0 (the root node of the tree) is considered an even number. Extend your
Treevisitor::visitTree method to support this functionality.

Solution:
```c
class TreeVisitor {
public:
	float total = 0.f;
	float visitTree(const Tree& tree, bool countOnlyEvenLevels) {
		//When counting nodes on only even levels
		if (countOnlyEvenLevels) {				
			total = 0.f;
			return visitEvenTree(tree, countOnlyEvenLevels);
		} 
		//When counting all nodes
		else {
			for (auto i : tree.children) {
				total += i.value;
				visitTree(i, countOnlyEvenLevels);
			}
		}
		return total; 
	}

	//Helper method in case the tree only counts nodes on even levels
	float visitEvenTree(const Tree& tree, bool even) {
		if (even) {
			total += tree.value;
		}
		for (auto i : tree.children) {
			visitEvenTree(i, !even);
		}
		return total;
	}
};
```

Test:
```c
class Tree {
public:
	float value;
	std::list<Tree> children;
};

float TreeTraversal(const Tree& t, TreeVisitor& visitor, bool countOnlyEvenLevels)
{
	return visitor.visitTree(t, countOnlyEvenLevels);
}

void fillTree(Tree& tree, int level)
{
	tree.value = 2.f * level;
	if (level >= 4)
		return;

	Tree child;
	fillTree(child, level + 1);

	for (int i = 0; i < level + 2; ++i)
	{
		tree.children.push_back(child);
	}
}

Tree root;
	fillTree(root, 0);
	TreeVisitor visitor;
	float visitAnswer1 = visitor.visitTree(root, false);
	float visitAnswer2 = visitor.visitTree(root, true);

	if (equalFloats(visitAnswer1, 1132.f) && equalFloats(visitAnswer2, 984.f))
	{
		std::cout << "Exercise 2: Correct" << std::endl;
	}
	else
	{
		std::cout << "Exercise 2: Incorrect" << std::endl;
	}
  ```
