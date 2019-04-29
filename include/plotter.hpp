#ifndef __PLOTTER_HPP__
#define __PLOTTER_HPP__

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

struct canvas{
  std::string name;
  int height;
  int width;
  std::string title;
  std::string histclass;
};

struct marker{
  int style;
  int color;
  float size;
};

struct line{
  int style;
  int color;
  float width;
};

struct hist{
  std::string filename;
  std::string treename;
  std::string varname;
  std::string selection;

  marker marker_style;
  line line_style;

  bool norm;
  std::string drawopt;
};

struct scale{
  bool log;
  float x_min;
  float x_max;
  float y_min;
  float y_max;
  int xbins;
  int ybins;
};

struct legend{
  std::string style;
  float x_min;
  float x_max;
  float y_min;
  float y_max;
};
struct annotation{
  std::string h_title;
  std::string x_title;
  std::string y_title;
  legend lg;
};

class plotter{
public:
  plotter(nlohmann::json input_json){
    js = input_json;
  };
  void Init();
  void Exec();
  void End();

private:
  nlohmann::json js;
  canvas c;
  std::vector<hist> h;
  scale sc;
  annotation note;
};



#endif
