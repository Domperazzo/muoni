#include <vector>

class muone{
public:
  //costruttori
  muone(double pos_x, double pos_y,double pos_z, double theta, double phi, std::vector<double> vel, double time);
  //distruttori
  ~muone ();

//================
  /*
    METODI
  */
//================
  double modulo_vel();
  double get_vel_component(int component);
    /*
      - component = 1 -> x
      - component = 2 -> y
      - component = 3 -> z
    */
  double posizione_x();
  double posizione_y();
  double posizione_z();

private:
  double x;
  double y;
  double z;
  double ang_theta;
  double ang_phi;
  std::vector<double> v_vel;
  double tempo;
};
