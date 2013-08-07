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
  TH1F* h1 = new TH1F("h1", "Pulse Output1", 2499, -2.68000004411e-08, 2.31600001399e-08);
  TH1F* h2 = new TH1F("h2", "Pulse Output2", 2499, -2.68000004411e-08, 2.31600001399e-08);
  TH1F* h3 = new TH1F("h3", "Pulse Output3", 2499, -2.68000004411e-08, 2.31600001399e-08);
  float min2 = 1.0;
  for(int i = 0; i < tree->GetEntries(); i++){
    tree->GetEntry(i);
    float min = 1.0;
    min2 = 1.0;
    int size = Time->size();
    std::cout << "vector size: " << Amp->size() << " Time: " << Time->at(0) << " " << Time->at(1) << " " << Time->at(2) << " " << Time->at(size-1) << std::endl;
    std::cout << " Time: " << Time->at(0)-Time->at(1) << " " << Time->at(1)-Time->at(2) << " " << Time->at(2)-Time->at(3) << " " << Time->at(size-1) << std::endl;
    //std::cout << "time stamp: " << ptime << std::endl;
    for(int j = 0; j < Amp->size(); j++){
      if(i = 1)h1->Fill(Time->at(j),Amp->at(j));
      if(i = 109)h2->Fill(Time->at(j),Amp->at(j));
      if(i = 267)h3->Fill(Time->at(j),Amp->at(j));
      if(Amp->at(j) < min2){
	min2 = Amp->at(j);
      }
    }
    
    h->Fill(TMath::Abs(min2));
    if(min2 != 1.0)std::cout << "Min: " << min2 << std::endl;  
    
  }
  
  h1->Draw();
  TFile* f1 = new TFile("test.root","RECREATE");
  h->Write();
  h1->Write();
  h2->Write();
  h3->Write();
  f1->Close();
  f->Close();
}
