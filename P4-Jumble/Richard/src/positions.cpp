
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

/**
  This file contains some toy code which illustrate example
    usage of the standard template library unordered_map class 
    among other things.  Some other C++ concepts illustrated
    include:

        the std::sort function
        command-line arguments
        opening and reading from files
        the "foreach" construct to iterate over elements
           in an STL "container"

  The program itself reads a text file and builds a
  "frequency-count" data structure using an unordered_map.

  For each ditsinct string in the input file, the map 
  keeps track of the number of time that string appears
  in the file.
**/


/*
*  this function rearranged the characters in a string
*    so that they are sorted (according to their ASCII
*    value).
*
*  Resource:  https://en.cppreference.com/w/cpp/algorithm/sort
*
*  Note:  this function is not actually used the the program
*    in this file.  But you might find it useful... :)
*/
void ssort(std::string &s) {

  /*
     strings are "iterable" objects and so have the begin()
     and end() functions.  These functions (in a pretty
     abstract way) return "iterators" which specify the
     "beginning" and "end" of the associated object).

     Thus, this call is asking the sort function to sort
     the entire string s.
  */
  std::sort(s.begin(), s.end());
}


/*
*   usage:   ./freq <filename>
*
*   example:  ./freq words.txt
*
*/
int main(int argc, char *argv[]){
  /*
     the variable word2pos is an unordered_map
        from strings to integers.
  */
  std::unordered_map<std::string, std::vector<int>> word2pos;
  std::ifstream file;
  std::string word;

  if(argc != 2) {
    std::cout << "usage:  ./freq <filename>\n";
    std::cout << "goodbye\n";
    return 1;
  }

  /*
  *  argv[1] is a C-string which is the filname specified
  *    by the user.  Let's try to open it.
  */

  file.open(argv[1], std::ios::in);

  if(!file.is_open()){
    std::cout << "Error: could not open file '" <<
                     argv[1] << "'\n";
    std::cout << "goodbye\n";
    return 1;
  }

  std::cout << "reading input file...\n";

  int pos=0;
  while(file >> word) { 
    // file >> word; 
    pos++;

    /**
    if(word2pos.count(word) == 0)
       word2pos[word] = 1;
    else {
    **/
       word2pos[word].push_back(pos);
  }

  std::cout << 
    "enter a word and I will tell you what I know about it\n";
  std::cout << 
    "  when you are done, type ctrl-d (ctrl-z on windows)\n\n";
  std::cout << "> ";

  /**

  while(std::cin >> word) {

    if(word2pos.count(word)==0) {
       std::cout << "sorry, '" << word << 
            "' does not appear in the input file '" <<
            argv[1] << "'\n";
    }
    else {
       std::cout << "'" << word << "' appears " <<
           word2pos[word] << " times in the file '" <<
            argv[1] << "'\n";
    }
  }
  // user has quit.  Let's dump the map contents
  **/

  std::cout << "word2pos contents:\n\n";

  printf("      WORD              #positions\n");
  printf("------------------------------------------\n\n");

  for(std::pair<std::string, std::vector<int>> entry : word2pos) {
     printf("  %10s     : ", entry.first.c_str());
     for(int p : entry.second) {
       printf(" %3d ", p);
     }
     printf("\n");
  }

  return 0;
}


