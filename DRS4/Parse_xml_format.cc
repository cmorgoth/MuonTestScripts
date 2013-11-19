/*
Code written to parse DRS4 XML data
Author: Cristian Pena.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TROOT.h"
#include <stdlib.h>     /* atof */
#include <math.h> 

#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

int main(int argc, char** argv){
  
  if(argc > 3){
    std::cout << "Wrong Usage!\nPlease use as follows:\n ./parse_DRS4 file.xml" << std::endl;
    return -1;
  }
  gROOT->Reset();
  gROOT->ProcessLine("#include <vector>");

  TFile* f = new TFile("DRS4_data.root", "RECREATE");
  TTree* tree = new TTree("outTree","DRS4 Tree");
  
  int evt = 0;
  std::vector<float>   *Amp = new std::vector<float>();
  std::vector<float>   *Time = new std::vector<float>();
  std::vector<float>   *Amp2 = new std::vector<float>();
  std::vector<float>   *Time2 = new std::vector<float>();
  std::vector<float>   *Amp3 = new std::vector<float>();
  std::vector<float>   *Time3 = new std::vector<float>();

  /*
  // List of branches
  TBranch   *b_Amp;//!
  TBranch   *b_Time;//!
  TBranch   *b_Amp2;//!
  TBranch   *b_Time2;//!
  TBranch   *b_Amp3;//!
  TBranch   *b_Time3;//!
  */
  tree->Branch("evt", &evt, "evt/I");
  tree->Branch("Amp", "std::vector<float>*", Amp);
  tree->Branch("Time", "std::vector<float>*" ,Time);
  tree->Branch("Amp2", "std::vector<float>*", Amp2);
  tree->Branch("Time2", "std::vector<float>*", Time2);
  tree->Branch("Amp3", "std::vector<float>*", Amp3);
  tree->Branch("Time3", "std::vector<float>*", Time3);
  
  std::ifstream ifs (argv[1], std::ifstream::in);
  std::string line;
  
  bool valid_msm = false;
  if(ifs.is_open()){
    while(ifs.good()){
      if(ifs.eof())break;
      getline(ifs, line);
      //std::cout << line << std::endl;
      if(line.find("<DRSOSC>") != std::string::npos){
	std::cout << "Starting Measurement!!" << std::endl;
	valid_msm = true;
      }
      
      if( valid_msm && (line.find("<Event>") != std::string::npos) ){
	if(evt!=0){
	  tree->Fill();//Filling the Tree
	  Amp->clear();
	  Amp2->clear();
	  Amp3->clear();
	  Time->clear();
	  Time2->clear();
	  Time3->clear();
	}
	getline(ifs, line);//Reading Serial
	getline(ifs, line);//Reading Time Stamp
	getline(ifs, line);//Reading Horizontal Units
	//std::cout << "HUnit: " << line << std::endl;
	getline(ifs, line);//Reading Vertical Units
	//std::cout << "VUnit: " << line << std::endl;
	evt++;
      }
      
      if(valid_msm && (line.find("<CHN1>") != std::string::npos)){
	while(1){
	  getline(ifs, line);//Reading CHN1 data
	  if(line.find("</CHN1>") != std::string::npos)break;
	  int p_low = line.find(">");
	  int p_med = line.find(",");
	  int p_high = line.find("<",p_med+1);
	  float t1 = atof(line.substr(p_low+1,p_med-(p_low+1)).c_str());
	  float a1 = atof(line.substr(p_med+1,p_high-(p_med+1)).c_str());
	  Amp->push_back(a1);
	  Time->push_back(t1);
	  //std::cout << t1 << " " << a1 << std::endl;
	}
      }

      if(valid_msm && (line.find("<CHN2>") != std::string::npos)){
	while(1){
	  getline(ifs, line);//Reading CHN2 data
	  if(line.find("</CHN2>") != std::string::npos)break;
	  int p_low = line.find(">");
	  int p_med = line.find(",");
	  int p_high = line.find("<",p_med+1);
	  float t1 = atof(line.substr(p_low+1,p_med-(p_low+1)).c_str());
	  float a1 = atof(line.substr(p_med+1,p_high-(p_med+1)).c_str());
	  Amp2->push_back(a1);
	  Time2->push_back(t1);
	  //std::cout << t1 << " " << a1 << std::endl;
	}
      }

      if(valid_msm && (line.find("<CHN3>") != std::string::npos)){
	while(1){
	  getline(ifs, line);//Reading CHN3 data
	  if(line.find("</CHN3>") != std::string::npos)break;
	  int p_low = line.find(">");
	  int p_med = line.find(",");
	  int p_high = line.find("<",p_med+1);
	  float t1 = atof(line.substr(p_low+1,p_med-(p_low+1)).c_str());
	  float a1 = atof(line.substr(p_med+1,p_high-(p_med+1)).c_str());
	  Amp3->push_back(a1);
	  Time3->push_back(t1);
	}
      }
      
    }
  }else{
    std::cout << "Unable to open file: " << argv[1] << std::endl;
  }
  
  std::cout << evt << std::endl;
  tree->Write();
  f->Close();
  return 0;
}
