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
#include "TH2.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TStyle.h"

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
  c.histclass = js["Canvas"]["class"];


  // set hist collection
  // std::cout << js["Hist"].dump(4) << std::endl;

  for(int i=0; i<js["Hist"].size(); i++){
    hist histo;
    auto js_temp = js["Hist"][i];
    histo.filename = js_temp["file"];
    histo.treename = js_temp["tree"];
    histo.varname = js_temp["var"];
    histo.selection = js_temp["selection"];

    if(c.histclass == "TH1"){
      histo.marker_style.style = js_temp["marker"]["style"];
      histo.marker_style.color = js_temp["marker"]["color"];
      histo.marker_style.size = js_temp["marker"]["size"];

      histo.line_style.style = js_temp["line"]["style"];
      histo.line_style.color = js_temp["line"]["color"];
      histo.line_style.width = js_temp["line"]["width"];
    }

    histo.norm = js_temp["opt"]["norm"];
    histo.drawopt = js_temp["opt"]["drawopt"];
    h.push_back(histo);
  }
  std::cout << "Plotting class: " << c.histclass << std::endl;
  std::cout << "number of hists: " << h.size() << std::endl;


  // set plot scale
  sc.log = js["Scale"]["log"];
  sc.x_min = js["Scale"]["x_range"][0];
  sc.x_max = js["Scale"]["x_range"][1];
  sc.y_min = js["Scale"]["y_range"][0];
  sc.y_max = js["Scale"]["y_range"][1];
  sc.xbins = js["Scale"]["xbins"];

  std::cout << "Scale:" << std::endl;
  std::cout << "x: [" << sc.x_min << ", " << sc.x_max << "]" << std::endl;
  std::cout << "y: [" << sc.y_min << ", " << sc.y_max << "]" << std::endl;

  if(c.histclass == "TH2"){
    sc.ybins = js["Scale"]["ybins"];
    sc.z_min = js["Scale"]["z_range"][0];
    sc.z_max = js["Scale"]["z_range"][1];
    std::cout << "z: [" << sc.z_min << ", " << sc.z_max << "]" << std::endl;
  }

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


  std::vector<TH1F*> hv1;
  std::vector<TH2F*> hv2;

  for(int i=0; i<h.size(); i++){
    TFile* f = new TFile(h[i].filename.c_str(), "READ");
    TTree* tr = (TTree*) f->Get(h[i].treename.c_str());

    TString hn = Form("h%d", i);

    if(c.histclass == "TH1"){
      tr->Draw(Form("%s>>%s(%d,%f,%f)", h[i].varname.c_str(), hn.Data(), sc.xbins, sc.x_min, sc.x_max), h[i].selection.c_str(), "");
    }
    else if(c.histclass == "TH2"){
      std::string varname = h[i].varname;
      auto pos = varname.find(':');
      std::string var1 = varname.substr(0,pos);
      std::string var2 = varname.substr(pos+1, varname.size());
      note.x_title = var2;
      note.y_title = var1;


      tr->Draw(Form("%s:%s>>%s(%d,%f,%f,%d,%f,%f)", var1.c_str(), var2.c_str(), hn.Data(), sc.xbins, sc.x_min, sc.x_max, sc.ybins, sc.y_min, sc.y_max), h[i].selection.c_str(), "");
    }
    else{
      std::cout << "Wrong plotting class!" << std::endl;
      exit(1);
    }

    if(c.histclass=="TH1"){
      TH1F* h_temp = (TH1F*)gDirectory->Get(hn.Data());
      h_temp->SetLineColor(h[i].line_style.color);
      h_temp->GetYaxis()->SetRangeUser(sc.y_min, sc.y_max);
      h_temp->SetTitle(Form("%s;%s;%s", note.h_title.c_str(), note.x_title.c_str(), note.y_title.c_str()));
      if(h[i].norm == true){
        h_temp->Scale(1./h_temp->Integral());
        h_temp->SetTitle(Form("%s;%s;%s", note.h_title.c_str(), note.x_title.c_str(), "Probability"));
      }
      hv1.push_back(h_temp);
    }

    else if(c.histclass=="TH2"){
      TH2F* h_temp = (TH2F*)gDirectory->Get(hn.Data());
      h_temp->GetXaxis()->SetTitle(note.x_title.c_str());
      h_temp->GetYaxis()->SetTitle(note.y_title.c_str());


      if(h[i].norm == true){
        h_temp->Scale(1./h_temp->Integral());
      }
      hv2.push_back(h_temp);
    }
  }

  ca->cd();
  ca->Clear();
  gStyle->SetOptStat(0);
  for(int i=0; i<h.size(); i++){
    if(c.histclass=="TH1"){
      if(sc.log == true){
        gPad->SetLogy();
      }
      hv1[i]->Draw(Form("%ssame",h[i].drawopt.c_str()));
    }
    else if(c.histclass=="TH2"){
      if(sc.log == true){
        gPad->SetLogz();
      }
      hv2[i]->SetMinimum(sc.z_min);
      hv2[i]->SetMaximum(sc.z_max);
      hv2[i]->Draw(Form("%s",h[i].drawopt.c_str()));
    }
  }


  ca->SaveAs(Form("%s.pdf", c.name.c_str()));

  return;
};
void plotter::End(){
  return;
};
