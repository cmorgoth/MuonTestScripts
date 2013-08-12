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
  
  float min2 = 1.0;
  std::cout.precision(16);
  

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 1; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry < 2000){ 
      //std::cout << "entry: " << jentry << std::endl;
      //std::cout << "vector size: " << Time->size() << " Time: " << Time->at(0) << " " << Time->at(1) << " " << Time->at(2) << " " << Time->at(Time->size()-1) << std::endl;	
    }
    
    int ctr = 0;
    double time[5000];
    for(std::vector<float>::iterator it = Time->begin(); it != Time->end(); ++it){
      //if(ctr == 0 || ctr == Time->size() -1)std::cout << ' ' << *it << std::endl;
      time[ctr] = *it;
      ctr++;
    }
    
    int ctr1 = 0;
    min2 = 1.0;
    double time_min = 20;
    double deltaT = time[1]-time[0];
    double integral = .0;
    for(std::vector<float>::iterator it1 = Amp->begin(); it1 != Amp->end(); ++it1){
      //if(ctr1 == 0 || ctr1 == Amp->size() -1)std::cout << ' ' << *it1 << std::endl;
      //if(jentry == 6)h1->Fill(time[ctr1],fabs(*it1));
      //if(jentry == 635)h2->Fill(time[ctr1],fabs(*it1));
      //if(jentry == 276)h3->Fill(time[ctr1],fabs(*it1));
      if(jentry == 6)h1->Fill(time[ctr1],*it1);
      if(jentry == 635)h2->Fill(time[ctr1],*it1);
      if(jentry == 276)h3->Fill(time[ctr1],*it1);
      if(*it1 < min2){
	min2 = *it1;
	time_min = time[ctr1];
      }
      ctr1++;
    }

    //std::cout << time[500]-time[499] << "  " << time[745] - time[744] << std::endl;
    double RiseTime = -99.;
    int ctr2 = 0;
    for(std::vector<float>::iterator it1 = Amp->begin(); it1 != Amp->end(); ++it1){
      if(*it1 <0.20*min2)integral += (*it1)*deltaT;
      if(*it1 < 0.90*min2){
	RiseTime =  time[ctr2];
	break;
      }
      ctr2++;
    }
    
    //std::cout << "Int: " << fabs(integral) << std::endl;
    //std::cout << "ctr1: " << ctr1 << std::endl;
    
    h->Fill(fabs(min2));
    h4->Fill(time_min);
    h5->Fill(RiseTime);
    h6->Fill(fabs(integral));
    //if(min2 < 1.0)std::cout << "min2: " << min2 << std::endl;
    
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
  TFile* fout = new TFile("file_out_Crystal.root", "recreate");
  h->Write();
  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();
  h5->Write();
  h6->Write();
  fout->Close();
}
