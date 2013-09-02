#define outTree_cxx
#include "outTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include "TH1F.h"
#include <math.h>
#include "TF1.h"

void outTree::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L outTree.C
//      Root > outTree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  /*
  TH1F* h = new TH1F("h", "muon spectrum", 500, .0, 18.5);
  TH1F* h1 = new TH1F("h1", "Pulse Output1", 2499, -2.68000004410851e-08, 2.316000013991015e-08);
  TH1F* h2 = new TH1F("h2", "Pulse Output2", 2499, -2.68000004410851e-08, 2.316000013991015e-08);
  TH1F* h3 = new TH1F("h3", "Pulse Output3", 2499, -2.68000004410851e-08, 2.316000013991015e-08);
  
  TH1F* h4 = new TH1F("h4", "Time peak spectrum", 2499, -2.68000004410851e-08, 2.316000013991015e-08);
  TH1F* h5 = new TH1F("h5", "Rise Time spectrum", 2499, -2.68000004410851e-08, 2.316000013991015e-08);
  TH1F* h6 = new TH1F("h6", "Intregral", 500, .0, 1e-9);
  */
  
  TH1F* h = new TH1F("h", "muon spectrum", 100, .0, 18.5);
  TH1F* h1 = new TH1F("h1", "Pulse Output1", 2499, -3.399999926045894e-08, 4.656000101022073e-07);
  TH1F* h2 = new TH1F("h2", "Pulse Output2", 2499, -3.399999926045894e-08, 4.656000101022073e-07);
  TH1F* h3 = new TH1F("h3", "Pulse Output3", 2499, -3.399999926045894e-08, 4.656000101022073e-07);
  TH1F* h4 = new TH1F("h4", "Time peak spectrum", 2499, -3.399999926045894e-08, 4.656000101022073e-07);
  TH1F* h5 = new TH1F("h5", "Rise Time spectrum", 2499, -3.399999926045894e-08, 4.656000101022073e-07);
  TH1F* h6 = new TH1F("h6", "Intregral", 500, .0, 1e-7);
  
  TH1F* h7 = new TH1F("h7", "Tau", 200, 20.0*1e-9, 90*1e-9);
  TH1F* h8 = new TH1F("h8", "Slope", 200, 1e9, 5*1e9);
  TH1F* h9 = new TH1F("h9", "half max time", 200, 15*1e-09, 28*1e-9);
  TH1F* h10 = new TH1F("h10", "Decay time", 200, 10*1e-09, 150*1e-9);
  TH1F* h12 = new TH1F("h12", "Diff", 200, 10*1e-09, 150*1e-9);
  TH2F* h11 = new TH2F("h11", "Decay time vs Half Time Max", 200, 15*1e-09, 28*1e-9, 200, 10*1e-09, 150*1e-9);
  
  float min2 = 1.0;
  std::cout.precision(16);
  

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  TF1* f1;
  TF1* f2;
  TF1* f3;
  TF1* f4;
  for (Long64_t jentry = 0; jentry < 1/*nentries*/; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry < 2000){ 
      //std::cout << "entry: " << jentry << std::endl;
      //std::cout << "vector size: " << Time->size() << " Time: " << Time->at(0) << " " << Time->at(1) << " " << Time->at(2) << " " << Time->at(Time->size()-1) << std::endl;	
    }
  
    h1 = new TH1F("h1", "Pulse Output1", 2499, -3.399999926045894e-08, 4.656000101022073e-07);
    int ctr = 0;
    double time[5000];
    for(std::vector<float>::iterator it = Time->begin(); it != Time->end(); ++it){
      time[ctr] = *it;
      ctr++;
    }
    
    std::cout << "=========================ientry=====================: " << jentry << std::endl;
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
    
    f1 = new TF1("f1", "[0]*exp(-1*(x-0.07e-06)/[1])", 0.07e-06, 0.3e-06);
    f2 = new TF1("f2", "[0]+x*[1]", 20.6*1e-09, 22.9*1e-09);
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

    f4 = new TF1("f4", "[3]*( 1+TMath::Erf((1/sqrt(2))*((x-[0])/[1] -[1]/[2])) )*TMath::Exp(-( (x-[0])/[2] - [1]*[1]/(2*[2]*[2])))", 0.0000000175, 0.0000003);
  
  //[0]->t0,[1]->s, [2]->tau, [3]-> Amplitude
    f4->SetParameter(0,0.00000003);
    f4->SetParameter(1,.00000001);
    f4->SetParameter(2,0.00000001);
    f4->SetParameter(3,30.0);
    //h1->Fit(f4,"VMWLR");
    
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
  TFile* fout = new TFile("file_out_Crystal_fitting.root", "recreate");
  h7->Write();
  h8->Write();
  h9->Write();
  h10->Write();
  h11->Write();
  h12->Write();
  h1->Write();
  fout->Close();
}
