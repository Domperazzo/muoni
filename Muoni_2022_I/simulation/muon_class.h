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

double get_raggio();
double get_theta();
double get_phi();

private:
  double raggio;
  double ang_theta;
  double ang_phi;

};
