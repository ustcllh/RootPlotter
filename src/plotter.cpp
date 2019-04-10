// cpp libraries
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// json library
#include "nlohmann/json.hpp"

// plotter library
#include "plotter.hpp"

// root libraries

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TLegend.h"
#include "TCanvas.h"

void plotter::Init(){
  if(js.empty()){
    std::cout << "Error: json empty!" << std::endl;
    return;
  }

  // std::cout << "Print Configuration:" << std::endl;
  // std::cout<< js.dump(4) <<std::endl;


  // set canvas properties
  c.name = js["Canvas"]["name"];
  c.height = js["Canvas"]["height"];
  c.width = js["Canvas"]["width"];
  c.title = js["Canvas"]["title"];


  // set hist collection
  // std::cout << js["Hist"].dump(4) << std::endl;

  for(int i=0; i<js["Hist"].size(); i++){
    hist histo;
    auto js_temp = js["Hist"][i];
    histo.filename = js_temp["file"];
    histo.treename = js_temp["tree"];
    histo.varname = js_temp["var"];
    histo.selection = js_temp["selection"];

    histo.marker_style.style = js_temp["marker"]["style"];
    histo.marker_style.color = js_temp["marker"]["color"];
    histo.marker_style.size = js_temp["marker"]["size"];

    histo.line_style.style = js_temp["line"]["style"];
    histo.line_style.color = js_temp["line"]["color"];
    histo.line_style.width = js_temp["line"]["width"];

    histo.norm = js_temp["opt"]["norm"];
    histo.drawopt = js_temp["opt"]["drawopt"];

    h.push_back(histo);
  }

  std::cout << "number of hists: " << h.size() << std::endl;


  // set plot scale
  sc.log = js["Scale"]["log"];
  sc.x_min = js["Scale"]["x_range"][0];
  sc.x_max = js["Scale"]["x_range"][1];
  sc.y_min = js["Scale"]["y_range"][0];
  sc.y_max = js["Scale"]["y_range"][1];
  sc.nbins = js["Scale"]["nbins"];

  // set title
  note.h_title = js["Annotation"]["title"]["hist"];
  note.x_title = js["Annotation"]["title"]["x_axis"];
  note.y_title = js["Annotation"]["title"]["y_axis"];

  // set legend
  note.lg.style = js["Annotation"]["legend"]["style"];
  note.lg.x_min = js["Annotation"]["legend"]["x_pos"][0];
  note.lg.x_max = js["Annotation"]["legend"]["x_pos"][1];
  note.lg.y_min = js["Annotation"]["legend"]["y_pos"][0];
  note.lg.y_max = js["Annotation"]["legend"]["y_pos"][1];

  return;
};

void plotter::Exec(){

  TCanvas* ca = new TCanvas("ca", c.title.c_str(), c.height, c.width);

  std::vector<TH1F*> hv;

  for(int i=0; i<h.size(); i++){
    TFile* f = new TFile(h[i].filename.c_str(), "READ");
    TTree* tr = (TTree*) f->Get(h[i].treename.c_str());

    TString hn = Form("h%d", i);
    tr->Draw(Form("%s>>%s(%d,%f,%f)", h[i].varname.c_str(), hn.Data(), sc.nbins, sc.x_min, sc.x_max), h[i].selection.c_str(), "");

    TH1F* h_temp = (TH1F*)gDirectory->Get(hn.Data());

    h_temp->SetLineColor(h[i].line_style.color);
    h_temp->GetYaxis()->SetRangeUser(sc.y_min, sc.y_max);

    hv.push_back(h_temp);
  }

  for(int i=0; i<h.size(); i++){
    ca->cd();
    hv[i]->Draw(Form("%ssame",h[i].drawopt.c_str()));
  }


  ca->SaveAs(Form("%s.pdf", c.name.c_str()));

  return;
};
void plotter::End(){
  return;
};
