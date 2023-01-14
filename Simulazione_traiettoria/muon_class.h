#include <vector>

class muone{
public:
  
  muone(double theta, double phi);
  
  ~muone ();

  double coeff_x();
  double coeff_y();

  double get_theta();
  double get_phi();

private:
  double ang_theta;
  double ang_phi;

};
