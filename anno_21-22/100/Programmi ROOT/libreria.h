#ifndef LIBRERIA_H
#define LIBRERIA_H

#include <vector>
#include <string>

//--- --- --- --- --- --- --- --- --- --- LETTURA --- --- --- --- --- --- --- --- --- --- --- --- 
//Legge un file per studiare la migliore tensione di soglia. 1 = insuccesso, 0 = successo

int lettura (std::string name_file,
             std::vector<double> & V, 
	     std::vector<double> & n_T1, std::vector<double> & n_S1, std::vector<double> & n_S2, std::vector<double> & n_T2,
	     std::vector<double> & n_double, std::vector<double> & n_triple1, std::vector<double> & n_triple2);

#endif
