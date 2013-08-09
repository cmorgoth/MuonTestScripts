#define outTree_cxx
#include "outTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include "TH1F.h"
#include <math.h>

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
  TH1F* h = new TH1F("h", "muon spectrum", 1000, .0, 18.5);
  TH1F* h1 = new TH1F("h1", "Pulse Output1", 2499, -2.68000004411e-08, 2.31600001399e-08);
  TH1F* h2 = new TH1F("h2", "Pulse Output2", 2499, -2.68000004411e-08, 2.31600001399e-08);
  TH1F* h3 = new TH1F("h3", "Pulse Output3", 2499, -2.68000004411e-08, 2.31600001399e-08);
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
      for (std::vector<float>::iterator it = Time->begin() ; it != Time->end(); ++it){
	//if(ctr == 0 || ctr == Time->size() -1)std::cout << ' ' << *it << std::endl;
	ctr++;
      }
      int ctr1 = 0;
      min2 = 1.0;
      for (std::vector<float>::iterator it1 = Amp->begin() ; it1 != Amp->end(); ++it1){
	//if(ctr1 == 0 || ctr1 == Amp->size() -1)std::cout << ' ' << *it1 << std::endl;
	ctr1++;
	if(*it1 < min2){
	  min2 = *it1;
	}
      }

      h->Fill(fabs(min2));
      if(min2 < 1.0)std::cout << "min2: " << min2 << std::endl;
      
   }

   TFile* fout = new TFile("file_out.root", "recreate");
   h->Write();
   fout->Close();
}
