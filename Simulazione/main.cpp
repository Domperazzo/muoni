/*
c++ -o main main.cpp
*/

#include <cstdlib>
#include <iostream>
#include <ctime>

int main (int argc, char ** argv)
  {
    srand (3) ;
    for (int i = 0 ; i < 5 ; ++i)
      std::cout << "indice " << i << " --> " << rand () << "\n" ;

    std::cout << "\n" ;

    srand (time (NULL)) ;
    for (int i = 0 ; i < 5 ; ++i)
      std::cout << "indice " << i << " --> " << rand () << "\n" ;

    return 0 ;
  }
