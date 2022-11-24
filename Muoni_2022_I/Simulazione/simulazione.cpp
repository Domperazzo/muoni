#include <vector>
#include <cmath>
#include <time.h>
#include "TH1F.h"

#define xmax 100;
#define ymax 100;
#define zmax 100;
#define points_number 1000;
#define time_increment 1.0;

double rand_range (double min, double max)
  {
    return min + (max - min) * srand (time(NULL)) / static_cast<double> (RAND_MAX);
  }

double legge_oraria(double velocita_i, double posizione_iniziale_i, double tempo){
  return posizione_iniziale_i + velocita_i*tempo
}

int main(int argc, char const *argv[]) {

  std::vector<double> v_angoli, v_posX, v_posY, v_posZ;
  /*
    CONDIZIONI INIZIALI:
        - v(t=0) = v_0 = velocità del muone
        - x(t=0) = posizione generata casualmente
        - y(t=0) = posizione generata casualmente
        - z(t=0) = posizione generata casualmente
  */
  




  return 0;
}
