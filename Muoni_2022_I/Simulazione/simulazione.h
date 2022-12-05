#include <vector>

class muone{
public:
  //costruttori
  muone(double pos_x, double pos_y,double pos_z, double theta, double phi, double velocity, double time);
  muone(std::vector<double> v_posx, std::vector<double> v_posy, std::vector<double> v_posz,
        std::vector<double> v_theta, std::vector<double> v_phi, std::vector<double> v_velocity, double time);
  //distruttori
  ~muone ();

//================
  /*
    METODI
  */
//================
  void get_x(double x);
  double get_vel_component(int component);
    /*
      - component = 1 -> x
      - component = 2 -> y
      - component = 3 -> z
    */
  double posizione_x();
  double posizione_y();
  double posizione_z();

  double get_pos_x(int numero);
  double get_pos_y(int numero);
  double get_pos_z(int numero);
  double get_theta(int numero);
  double get_phi(int numero);
  double get_vel(int numero);

  double update_time(double time, double time_increment);

private:
  double x;
  double y;
  double z;
  double ang_theta;
  double ang_phi;
  double vel;
  double tempo;

  std::vector<double> v_x;
  std::vector<double> v_y;
  std::vector<double> v_z;
  std::vector<double> v_ang_theta;
  std::vector<double> v_ang_phi;
  std::vector<double> v_vel;

};
