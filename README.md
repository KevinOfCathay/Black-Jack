# Black-Jack
A C++ approach to black-jack problem (From Reinforcement Learning, Richard S. Sutton)

The rule is UNFINISHED. In a normal Black-Jack, an A will count as 1 OR 11. However, in this program, A is always counted as 1. However, the general idea is the same.
It is worth mentioning that since C++ map is based on RB-tree, it is inefficient to search and modify elements. Though C++ support unordered map, using a vector as the key is not a good idea either. In python, the same program can easily simulate the game millions of times within few seconds, whereas in C++, the progress is much more slower.
