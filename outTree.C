#define outTree_cxx
#include "outTree.h"
#include <TH2.h>
#include "TH1F.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
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
   if (fChain == 0) return;

   TH1F* h = new TH1F("h", "muon spectrum", 1000, .0, .5);
   TH1F* h1 = new TH1F("h1", "Pulse Output1", 2499, -2.68000004411e-08, 2.31600001399e-08);
   TH1F* h2 = new TH1F("h2", "Pulse Output2", 2499, -2.68000004411e-08, 2.31600001399e-08);
   TH1F* h3 = new TH1F("h3", "Pulse Output3", 2499, -2.68000004411e-08, 2.31600001399e-08);
   float min2 = 1.0;
   
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      //std::cout << "entry: " << jentry << std::endl;
      
float min = 1.0;
    min2 = 1.0;
    int size = Time->size();
    //std::cout << "vector size: " << Amp->size() << " Time: " << Time->at(0) << " " << Time->at(1) << " " << Time->at(2) << " " << Time->at(size-1) << std::endl;
    //std::cout << "time stamp: " << ptime << std::endl;
    for(int j = 0; j < Amp->size(); j++){
      if(jentry = 1)h1->Fill(Time->at(j),Amp->at(j));
      if(jentry = 109)h2->Fill(Time->at(j),Amp->at(j));
      if(jentry = 267)h3->Fill(Time->at(j),Amp->at(j));
      if(Amp->at(j) < min2){
	min2 = Amp->at(j);
      }
    }
    
    h->Fill(TMath::Abs(min2));
   }
   
   h1->Draw();
   TFile* f1 = new TFile("test.root","RECREATE");
   h->Write();
   h1->Write();
   h2->Write();
   h3->Write();
   f1->Close();
   
}
