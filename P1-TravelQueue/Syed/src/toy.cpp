#include "TravelOptions.h"

#include <stdlib.h>
#include <iostream>

// using namespace std;

/*
 simple demo program exercising some of the TravelOptions functions.

to compile:  g++ -std=c++11 toy.cpp

 purpose:  just to make sure you have an idea of how to write 
             driver/tester programs (this is not itself a tester program
             per-se, just a pretty random sequence of calls..
*/


int main(int argc, char *argv[]){
  TravelOptions *options_p, *options_p2, *options_p3;

  std::vector<std::pair<double, double>>
	// vec{ {1, 9}, {2, 8}, {3, 7}, 
	// 	{4, 6}, {5, 7}, {6, 6}, 
	// 	{7, 5}, {8, 4} };
      //vec{{2,20}, {5,15}, {7,25}, {8,24},{12,20},{15,12} };
      //vec{{2,20}, {5,19}, {7,18}, {8,15},{12,14},{15,21} };
        // vec{{2,20},{5,15},{15,11}};//insert sorted
        //vec{{2,16},{2,18},{3,15}}; // insert sorted
        //vec{{2,20},{5,15},{15,12}};//insert sorted
        //vec{{2,1},{2,3},{3,15}};//insert sorted
        // vec{{1,2}};
       // vec{{21,20},{23,28},{23,40}};
        // vec{{19,20},{20,18},{21,10}};
         //vec{{1,2},{3,10},{5,11},{5,14},{6,11}};
           // vec{{3,17},{8,16},{11,14},{12,12}};
           vec{{2,20},{5,15},{15,14}};
           //vec{{2,20},{5,19},{7,18},{8,17}};
  std::vector<std::pair<double, double>> *vec2;

  // remember:  from_vec is a static function (hence, TravelOptions:: form)
  options_p  = TravelOptions::from_vec(vec);
  options_p2 = TravelOptions::from_vec(vec);

  options_p->display();
  std::cout << "size: " << options_p->size() << std::endl; 
 
  TravelOptions::Relationship a,b,c,d;
   
  a = TravelOptions::compare(3, 6, 4, 12);

  if (a == TravelOptions::incomparable) 
     std::cout << "INCOMPARABLE!" << std::endl;
  else if (a == TravelOptions::better) 
     std::cout << "better!" << std::endl;
  else if (a == TravelOptions::worse) 
     std::cout << "worse!" << std::endl;
  else if (a == TravelOptions::equal) 
     std::cout << "equal!" << std::endl;


  vec2 = options_p->to_vec();

  std::cout << "CONTENTS OF EXPORTED VECTOR:\n";
  for(int i=0; i<vec2->size(); i++) {
     std::cout << (*vec2)[i].first << " ,  " << (*vec2)[i].second << std::endl;
  }

  std::cout << "\n";

  if(options_p->is_sorted() ) 
          std::cout << "Sorted!" << std::endl;
  else 
	  std::cout << "Not Sorted!" << std::endl<<std::endl;


  if( options_p->is_pareto() )
     std::cout << "PARETO!" << std::endl;
  else 
	  std::cout << "NOT PARETO!" << std::endl<<std::endl;

  if(options_p->is_pareto_sorted())
     std::cout << "SORTED PARETO!" << std::endl;
   else{
      std::cout << "Not SORTED Pareto" << std::endl;
   }

//   if(options_p->insert_sorted(5, 12))
//      std::cout << "INSERT_SORTED WORKED!" << std::endl;
//    else{
//       std::cout << "didnt work!" << std::endl;
//    }
//      options_p->display();
//   std::cout << "size: " << options_p->size() << std::endl; 

  if(options_p->insert_pareto_sorted(1, 10)){
     std::cout << "INSERT_PARETO_SORTED WORKED!" << std::endl;
     options_p->display();
     std::cout << "size: " << options_p->size() << std::endl; 
  }
   else{
          std::cout << "Didnt work(NOT SORTED AND NOT PARETO)" << std::endl;
   }

 // options_p3 = options_p->union_pareto_sorted(*options_p2);

//   if(options_p3->prune_sorted())
//      std::cout << "prune_sorted worked!" << std::endl;

//   delete options_p3;

//   options_p3 = options_p->join_plus_plus(*options_p2);

//   delete options_p3;
//   options_p3 = options_p->join_plus_max(*options_p2);
  

//   options_p->clear();
//   delete options_p;
//   delete options_p2;
//   delete options_p3;

  return 0;
}
