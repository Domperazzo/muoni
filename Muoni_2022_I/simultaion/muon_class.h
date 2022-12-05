#include <vector>

class muone{
public:
  //costruttori
  muone(double ragg, double theta, double phi);
  //distruttori
  ~muone ();

//================
  /*
    METODI
  */
//================

double coeff_x();
double coeff_y();

double rand_phi (double min, double max);
double rand_theta(double valore_centrale);

private:
  double raggio;
  double ang_theta;
  double ang_phi;

};
