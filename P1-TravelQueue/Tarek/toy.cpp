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
	vec{{2,20},{5,19},{6,18},{10,17},{11,16},{12,15}};

  std::vector<std::pair<double, double>> *vec2;

  // remember:  from_vec is a static function (hence, TravelOptions:: form)
  options_p  = TravelOptions::from_vec(vec);
  options_p2 = TravelOptions::from_vec(vec);

  options_p->display();
  std::cout << "size: " << options_p->size() << std::endl; 
 
  TravelOptions::Relationship a,b,c,d;
   
  a = TravelOptions::compare(10, 8.0, 11, 9.0);

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
          std::cout << "Sorted!" << std::endl <<std::endl;
  else 
	  std::cout << "Not Sorted!" << std::endl<<std::endl;


  if( options_p->is_pareto() )
  	std::cout << "PARETO!" << std::endl;
  else 
	std::cout << "NOT PARETO!" << std::endl<<std::endl;



  if(options_p->is_pareto_sorted())
     std::cout << "PARETO is sorted!" << std::endl<<std::endl;
  else 
	std::cout << "Pareto NOT sorted!" << std::endl<<std::endl;


  //if(options_p->insert_sorted(15,11))
 //    std::cout << "INSERT_SORTED WORKED!" << std::endl<< std::endl;
  //else 
//	std::cout << "INSERT_SORTED did not worked!" << std::endl<<std::endl;


   if(options_p->insert_pareto_sorted(1, 10))
     std::cout << "INSERT_PARETO_SORTED WORKED!" << std::endl;

    options_p->display();
    std::cout << "size: " << options_p->size() << std::endl;

  //options_p3= options_p->union_pareto_sorted(*options_p2);*/

  //if(options_p->prune_sorted())
  //  std::cout << "prune_sorted worked!" << std::endl;

  //options_p3 = options_p->split_sorted_pareto(10);

  //options_p3->display();
  //options_p3->display();

  //std::cout << "size: " << options_p->size() << std::endl;

  /*delete options_p3;

  options_p3 = options_p->join_plus_plus(*options_p2);

  delete options_p3;
  options_p3 = options_p->join_plus_max(*options_p2);
  

  options_p->clear();
  delete options_p;
  delete options_p2;
  delete options_p3;*/

  return 0;
}
