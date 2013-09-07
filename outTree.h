//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug  9 16:03:44 2013 by ROOT version 5.34/09
// from TTree outTree/outTree
// found on file: /media/data/cmorgoth/scope_data/data_Scope_Total.root
//////////////////////////////////////////////////////////

#ifndef outTree_h
#define outTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class outTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        ptime;
   std::vector<float>   *Amp;
   std::vector<float>   *Time;

   // List of branches
   TBranch        *b_ptime;   //!
   TBranch        *b_Amp;   //!
   TBranch        *b_Time;   //!

   outTree(TTree *tree=0);
   virtual ~outTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef outTree_cxx
outTree::outTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
     //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/media/data/cmorgoth/scope_data/data_Scope_Total.root");
     //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/cmorgoth/No_crystal_Data/NoCrystalData.root");
     //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/cmorgoth/CrystalData/CrystalData.root");
     //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/media/data/cmorgoth/Data/ScopeData_SmallCrystal/SmallCrystal.root");
     //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/media/data/cmorgoth/Data/ScopeData_SmallCrystal/RiseTimeData/TotalRiseTime.root");
     //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/media/data/cmorgoth/Data/ScopeData_SmallCrystal/NewScope/Tot_newScope.root");
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/media/data/cmorgoth/Data/ScopeData_SmallCrystal/NewScope/NewScope_9_2_2013/CorrectScale_NewScope.root");
     if (!f || !f->IsOpen()) {
       //f = new TFile("/media/data/cmorgoth/scope_data/data_Scope_Total.root");
       //f = new TFile("/home/cmorgoth/No_crystal_Data/NoCrystalData.root");
       //f = new TFile("/home/cmorgoth/CrystalData/CrystalData.root");
       //f = new TFile("/media/data/cmorgoth/Data/ScopeData_SmallCrystal/SmallCrystal.root");
       //f = new TFile("/media/data/cmorgoth/Data/ScopeData_SmallCrystal/RiseTimeData/TotalRiseTime.root");
       //f = new TFile("/media/data/cmorgoth/Data/ScopeData_SmallCrystal/NewScope/Tot_newScope.root");
       f = new TFile("/media/data/cmorgoth/Data/ScopeData_SmallCrystal/NewScope/NewScope_9_2_2013/CorrectScale_NewScope.root");
      }
      f->GetObject("outTree",tree);

   }
   Init(tree);
}

outTree::~outTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t outTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t outTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void outTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   Amp = 0;
   Time = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ptime", &ptime, &b_ptime);
   fChain->SetBranchAddress("Amp", &Amp, &b_Amp);
   fChain->SetBranchAddress("Time", &Time, &b_Time);
   Notify();
}

Bool_t outTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void outTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t outTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef outTree_cxx
