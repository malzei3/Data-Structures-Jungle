


Name:
Richard C. Remigoso, Jr.
-----------------------------------------------

Please confirm that you compiled your solution with test cases exercising ALL
functions using g++ -std=c++11.  Confirm this statement by typing YES below.
(If you did not do this, compilation errors may result in an overall grade of
zero!)

YES




Describe what augmentations to the bst data structures you made to complete the 
project -- i.e., what types / data members did you change and why?

In bst_node, I added 2 additional bookkeeping integer variables, leftSize and rightSize. leftSize keeps tabs of 
the number of nodes to the left of the current node, while rightSize keeps tabs of the number of nodes to the 
right of the current node. To keep track and update these bookkeeping variables for each node, I either increment
or decrement their value depending if a node is added or removed/deleted respectively. These variables proved to
be useful in several TODO functions in this project.


-----------------------------------------------
Which functions did you need to modify as a result of the augmentations from the previous
question?  

These updates can be found in insert, remove, delete, and from_vector. The following functions also use these
bookkeeping variables: num_geq, num_leq, position_of, and num_range.



-----------------------------------------------
For each function from the previous question, how did you ensure that the (asymptotic) runtime 
remained the same?

Since these bookkeping variables have a constant time complexity, the overall time complexity of a function
using these variables is minimally affected.



-----------------------------------------------
For each of the assigned functions, tell us how far you got using the choices 0-5 and
answer the given questions.  


0:  didn't get started
1:  started, but far from working
2:  have implementation but only works for simple cases
3:  finished and I think it works most of the time but not sure the runtime req is met. 
4:  finished and I think it works most of the time and I'm sure my design leads to 
       the correct runtime (even if I still have a few bugs).
5:  finished and confident on correctness and runtime requirements


to_vector level of completion:  _____4______  


-----------------------------------------------
get_ith level of completion:  _____4______  

    How did you ensure O(h) runtime?

    ANSWER: Avoided branching to unnecessary nodes, and did not use other given functions.
-----------------------------------------------
position_of level of completion:  _____3______  

    How did you ensure O(h) runtime?

    ANSWER: Calls and returns using a function of similar runtime.
-----------------------------------------------
num_geq level of completion:  _____4______  

    How did you ensure O(h) runtime?

    ANSWER: Avoided branching to unnecessary nodes, like those elements that are less than x.

-----------------------------------------------
num_leq level of completion:  _____4_______

    How did you ensure O(h) runtime?

    ANSWER: Avoided branching to unnecessary nodes, like those elements that are greater than x.

-----------------------------------------------
num_range level of completion:  ______4______

    How did you ensure O(h) runtime?

    ANSWER: Only subtracted the overlapping results of num_leq() and num_geq() with the size of the tree.

-----------------------------------------------
extract_range level of completion:  _____3_______

    How did you ensure O(h+k) runtime?

    ANSWER: Avoided branching to unnecessary nodes, like being outside the given range or not.

Implementation of size-balanced criteria according to 
the given guidelines:

    Level of completion: _____0______











