void sistema_vec(std::string filename, std::vector<double> v_vector) {
  std::ifstream dati;
  dati.open(filename, std::ios::in);
  std::vector<double> v_elements;
  while (true) {
      double element;
      dati >> element;
      if(dati.eof()==true) break;
      v_elements.push_back(element);
  }
	dati.close();

  for (int i = 0; i < v_elements.size(); i++) {
    if (i==0) {
      v_vector.push_back(v_elements.at(i));
    } else {
      v_vector.push_back(v_elements.at(i)-v_elements.at(i-1));
    }
  }
}
