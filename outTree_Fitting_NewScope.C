#define outTree_cxx
#include "outTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include "TH1F.h"
#include <math.h>
#include "TF1.h"

void outTree::Loop(){
  
  TH1F* h = new TH1F("h", "muon spectrum", 100, .0, 18.5);
  TH1F* h1 = new TH1F("h1", "Pulse Output1", 65527, -1.071052651013815e-07, 3.024322268174728e-07);

  TH1F* h4 = new TH1F("h4", "Time peak spectrum", 2499, -3.399999926045894e-08, 4.656000101022073e-07);
  TH1F* h5 = new TH1F("h5", "Rise Time spectrum", 2499, -3.399999926045894e-08, 4.656000101022073e-07);
  TH1F* h6 = new TH1F("h6", "Intregral", 500, .0, 1e-7);
  
  TH1F* h7 = new TH1F("h7", "Tau", 300, 20.0*1e-9, 90*1e-9);
  TH1F* h8 = new TH1F("h8", "Slope", 300, 1e9, 5*1e9);
  TH1F* h9 = new TH1F("h9", "half max time", 300, 10*1e-09, 28*1e-9);
  TH1F* h10 = new TH1F("h10", "Decay time", 300, 0.1*1e-07, 150*1e-9);
  TH1F* h12 = new TH1F("h12", "Diff", 300, 10*1e-09, 150*1e-9);
  TH2F* h11 = new TH2F("h11", "Decay time vs Half Time Max", 500, 15*1e-09, 28*1e-9, 200, 10*1e-09, 150*1e-9);
  
  //TH1F* h20 = new TH1F("h20", "t0", 1000, .1*1e-9, 90*1e-9);
  //TH1F* h21 = new TH1F("h21", "s", 1000, .1*1e-9, 90*1e-9);
  //TH1F* h22 = new TH1F("h22", "tau", 80, .1*1e-09, 28*1e-9);
  //TH1F* h23 = new TH1F("h23", "Amp", 500, 10*1e-09, 150*1e-9);

  TH1F* h20 = new TH1F("h20", "t0", 200, 15.0*1e-9, 25*1e-9);
  TH1F* h21 = new TH1F("h21", "s", 200, .1*1e-9, 3*1e-9);
  TH1F* h22 = new TH1F("h22", "tau", 200, .1*1e-09, 28*1e-9);
  TH1F* h23 = new TH1F("h23", "Amp", 800, 0, 50);
  

  float min2 = 1.0;
  std::cout.precision(16);
  

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  TF1* f1;
  TF1* f2;
  TF1* f3;
  TF1* f4;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    std::cout << "vector size: " << Time->size() << " Time: " << Time->at(0) << " " << Time->at(1) << " " << Time->at(2) << " " << Time->at(Time->size()-1) << std::endl;
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if(jentry < 2000){ 
      //std::cout << "vector size: " << Time->size() << " Time: " << Time->at(0) << " " << Time->at(1) << " " << Time->at(2) << " " << Time->at(Time->size()-1) << std::endl;	
    }
    
    h1 = new TH1F("h1", "Pulse Output1", 10000, -1.270499979000306e-06, 7.291000088116562e-07);
    int ctr = 0;
    double time[175527];
    for(std::vector<float>::iterator it = Time->begin(); it != Time->end(); ++it){
      time[ctr] = *it;
      ctr++;
    }
    
    int ctr1 = 0;
    double Max = -1.0;
    double time_min = 20;
    double deltaT = time[1]-time[0];
    double integral = .0;
    for(std::vector<float>::iterator it1 = Amp->begin(); it1 != Amp->end(); ++it1){
      h1->Fill(time[ctr1],-1*(*it1));
      if(-1*(*it1) > Max){
	Max = -1*(*it1);
	time_min = time[ctr1];
      }
      ctr1++;
    }

    double t_low = 0.0;
    double t_high = 0.0;
    int ctr_2 = 0;
    for(std::vector<float>::iterator it2 = Amp->begin(); it2 != Amp->end(); ++it2){
      if(-1*(*it2) > 0.10*Max){
	t_low = time[ctr_2];
	break;
      }
      ctr_2++;
    }
    
    ctr_2 = 0;
    for(std::vector<float>::iterator it2 = Amp->begin(); it2 != Amp->end(); ++it2){
      if(-1*(*it2) > 0.85*Max){
	t_high = time[ctr_2];
	break;
      }
      ctr_2++;
    }
    
    if(Max < 0.1)break;
    std::cout << "===============================" << std::endl;
    std::cout << "=============Max--------------> " << Max << std::endl;
    std::cout << "==== Time_low:  " << t_low << "  Time_high:  " << t_high << "===============" << std::endl;
    std::cout << "===============================" << std::endl;
    f1 = new TF1("f1", "[0]*exp(-1*(x-0.065e-06)/[1])", 0.065e-06, 0.25e-06);
    //f2 = new TF1("f2", "[0]+x*[1]", 15.3*1e-09, 19.2*1e-09);
    f2 = new TF1("f2", "[0]+x*[1]", t_low, t_high);
    f3 = new TF1("f3", "TMath::Landau(x,[0],[1],0)", 0.05*1e-09, .1*1e-06);
    f1->SetParameters(11., 0.07e-06);
    h1->Fit(f1,"VMWLR");
    h7->Fill(f1->GetParameter(1));
    double Tau = f1->GetParameter(1);
    double Amp = f1->GetParameter(0);
    double decay_time = -1*Tau*log(0.5*Max/Amp) + 1e-07;
    std::cout << "==================== Decay Time ==========================: " << decay_time << std::endl;
    h1->Fit(f2,"VMWLR");
    h10->Fill(decay_time);
    double m = f2->GetParameter(1);
    double b = f2->GetParameter(0);
    double time_half_max = (0.5*Max-b)/m;
    
    h8->Fill(m);
    h9->Fill(time_half_max);
    h11->Fill(time_half_max, decay_time);
    h12->Fill(decay_time-time_half_max);

    //f4 = new TF1("f4", "[3]*( 1+TMath::Erf((1/sqrt(2))*((x-[0])/[1] -[1]/[2])) )*TMath::Exp(-( (x-[0])/[2] - [1]*[1]/(2*[2]*[2])))", 0.00000000995, 0.0000003);
    f4 = new TF1("f4", "[3]*( 1+TMath::Erf((1/sqrt(2))*((x-[0])/[1] -[1]/[2])) )*TMath::Exp(-( (x-[0])/[2] - [1]*[1]/(2*[2]*[2])))", t_low -7.2*1e-9, 0.0000003);
  
    //[0]->t0,[1]->s, [2]->tau, [3]-> Amplitude
    f4->SetParameter(0,0.00000003);
    f4->SetParameter(1,.00000001);
    f4->SetParameter(2,0.00000001);
    f4->SetParameter(3,30.0);
    //h1->Fit(f4,"VMWLR");

    h20->Fill(f4->GetParameter(0));
    h21->Fill(f4->GetParameter(1));
    h22->Fill(f4->GetParameter(2));
    h23->Fill(f4->GetParameter(3));
    

    //delete h1;
    delete f1;
    delete f2;
  }
  /*
  TF1 *f1 = new TF1("f1", "[0]*exp(-1*(x-1e-07)/[1])", 0.09e-06, 0.3e-06);
  f1->SetParameters(11., 1e-07);
  h1->Fit(f1,"VMWLR");
  f1->SetParameters(11., 1e-07);
  h2->Fit(f1,"VMWLR");
  f1->SetParameters(11., 1e-07);
  h3->Fit(f1,"VMWLR");
  */
  TFile* fout = new TFile("file_linear_fit_example.root", "recreate");
  h1->SetXTitle("Time [sec]");
  h1->SetYTitle("Amp [Volts]");
  h7->Write();
  h8->Write();
  h9->Write();
  h10->Write();
  h11->Write();
  h12->Write();
  h1->Write();
  h20->Write();
  h21->Write();
  h22->Write();
  h23->Write();
  fout->Close();
}
