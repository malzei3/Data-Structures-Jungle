
#include <string>
using namespace std;
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

void ssort(std::string &s) {

  std::sort(s.begin(), s.end());
}
string get_list(unordered_map<string , vector<string> > jumbleWord, string key){

    //First, define a string that will hold the list
    string temp="";
    string c = " ";

    //Iterate over the vector of values for the given key, concatenating all values in a new line
    for(int i=0; i<jumbleWord[key].size();i++){
        temp+= c + c+c+ '\'' + jumbleWord[key][i] + '\'' + "\n";
    }

    //Finally, return the list as a string
    return temp;

}

int main(int argc, char *argv[]){
  /*
     the variable word2freq is an unordered_map
        from strings to integers.
  */
  unordered_map<string, vector<string>> jumbleWord;
  std::ifstream file;
  std::string word;
  int maxNum = 0;
  int counter = 0;
  string biggestKey;
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

  while(file >> word) {

    string tempWord = word;
    ssort(word);
    counter = counter + 1;
    if(jumbleWord.count(word) == 0){
       jumbleWord[word] = vector<string>();
       jumbleWord[word].push_back(tempWord);
     }
    else {
       jumbleWord[word].push_back(tempWord);
    }
    if(jumbleWord[word].size() > maxNum){
      biggestKey = word;
      maxNum = jumbleWord[word].size();
    }
  }

  std::cout <<
    "enter a word and I will tell you what I know about it\n";
  std::cout <<
    "  when you are done, type ctrl-d (ctrl-z on windows)\n\n";
  std::cout << "> ";


  while(cin >> word) {
    ssort(word);
    if(jumbleWord.count(word)==0) {
      cout<<"No anagrams found..try again"<<endl;
    }
    else {
      cout<<"English Anagrams found"<<endl;
      for(int x = 0; x < jumbleWord[word].size(); x++){
        cout<< " "<< jumbleWord[word][x]<<endl;
      }
    }
    cout<<">";
  }
  // user has quit.  Let's dump the map contents

  cout << "\n";
  cout << "> REPORT:\n\n";
  cout << "  num_words             :  "<<counter<<"\n";
  cout << "  num_classes           :  "<<jumbleWord.size()<<"\n";
  cout << "  size-of-largest-class :  "<<maxNum<<"\n";
  cout << "  largest-class key     :  "<<"'"<<biggestKey<<"'\n";
  cout << "  members of largest class:\n\n"<<get_list(jumbleWord,biggestKey)<<"\n";

 
  return 0;
}
