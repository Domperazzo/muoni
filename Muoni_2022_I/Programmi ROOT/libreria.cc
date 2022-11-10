#include "libreria.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

int lettura (std::string name_file,
             std::vector<double> & V, 
	     std::vector<double> & n_T1, std::vector<double> & n_S1, std::vector<double> & n_S2, std::vector<double> & n_T2,
	     std::vector<double> & n_double, std::vector<double> & n_triple1, std::vector<double> & n_triple2)  
 {
   std::ifstream temp_file;
   temp_file.open (name_file, std::ios::in);

   double temp_V, temp_n_T1, temp_n_S1, temp_n_S2, temp_n_T2, temp_n_double, temp_n_triple1, temp_n_triple2;
 
   if (temp_file.is_open() == false)
     {
       std::cout << "Impossibile aprire file " << name_file << std::endl;
       return 1;
     }

   while (true)
     {
       temp_file >> temp_V;
       temp_file >> temp_n_T1;
       temp_file >> temp_n_S1;
       temp_file >> temp_n_S2;
       temp_file >> temp_n_T2;
       temp_file >> temp_n_double;
       temp_file >> temp_n_triple1;
       temp_file >> temp_n_S2;

       if (temp_file.eof() == true)  break;

       V.push_back (temp_V_down);
       n_T1.push_back (temp_n_T1);
       n_S1.push_back (temp_n_S1);
       n_S2.push_back (temp_n_S2);
       n_T2.push_back (temp_n_T2);
       n_double.push_back (temp_n_double);
       n_triple1.push_back (temp_n_triple1);
       n_triple2.push_back (temp_n_triple2);
     }

   temp_file.close();

   return 0;
 }

