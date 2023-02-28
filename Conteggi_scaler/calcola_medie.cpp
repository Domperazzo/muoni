/*
	c++ -o medie_to_file calcola_medie.cpp `root-config --glibs --cflags`
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>

double calcMedia (const std::vector<double> & campione)
{
  double media = 0. ;
  for (int i = 0 ; i < campione.size () ; ++i)
    media += campione.at (i) ;
  return media / campione.size () ;
}

double calcVarianza (const std::vector<double> & campione)
{
  double media = 0. ;
  double mediaSq = 0. ;
  for (int i = 0 ; i < campione.size () ; ++i)
    {
      media += campione.at (i) ;
      mediaSq += campione.at (i) * campione.at (i) ;
    }
  media /= campione.size () ;
  return campione.size () * (mediaSq / campione.size () - media * media) / (campione.size () - 1) ;
}

int main(int argc, char const *argv[]) {

  std::string filename = argv[1];
  std::ifstream dati;
  dati.open(filename, std::ios::in);
  std::vector<double> v_S1, v_S2;
  while (true) {
      double s1, s2;
      dati >> s1 >> s2;
      if(dati.eof()==true) break;
      v_S1.push_back(s1);
      v_S2.push_back(s2);
  }
	dati.close();

  double mediaS1, mediaS2, varS1, varS2;

  mediaS1 = calcMedia(v_S1);
  varS1 = sqrt(calcVarianza(v_S1)/v_S1.size());
  mediaS2 = calcMedia(v_S2);
  varS2 = sqrt(calcVarianza(v_S2)/v_S2.size());

  std::ofstream outfile_media1("Dati/S1.txt", std::ios::app);
  if (outfile_media1.is_open()) {
    outfile_media1 << mediaS1 << "\t" << varS1 << std::endl;
    outfile_media1.close();
  }

  std::ofstream outfile_media2("Dati/S2.txt", std::ios::app);
  if (outfile_media2.is_open()) {
    outfile_media2 << mediaS2 << "\t" << varS2 << std::endl;
    outfile_media2.close();
  }





  return 0;
}
