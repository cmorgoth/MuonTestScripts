{
  gROOT->Reset();
  
  TFile* f = new TFile("/media/data/cmorgoth/scope_data/NoCrystalData/NoCrystalData.root");
  TTree* tree = (TTree*)f->Get("outTree");
  
  std::cout.precision(12);
  double ptime;
  std::vector<float>   *Amp;
  std::vector<float>   *Time;
  
  tree->SetBranchAddress("ptime", &ptime);
  tree->SetBranchAddress("Amp", &Amp);
  tree->SetBranchAddress("Time", &Time);
  
  TH1F* h = new TH1F("h", "muon spectrum", 1000, .0, .5);
  float min2 = 1.0;
  for(int i = 0; i < tree->GetEntries(); i++){
    tree->GetEntry(i);
    float min = 1.0;
    min2 = 1.0;
    //std::cout << "vector size: " << Amp->size() << std::endl;
    //std::cout << "time stamp: " << ptime << std::endl;
    for(int j = 0; j < Amp->size(); j++){
      if(Amp->at(j) < min2){
	min2 = Amp->at(j);
      }
    }
    
    h->Fill(TMath::Abs(min2));
    if(min2 != 1.0)std::cout << "Min: " << min2 << std::endl;  
    
  }
  
  h->Draw();
  TFile* f1 = new TFile("test.root","RECREATE");
  h->Write();
  f1->Close();
  f->Close();
}
