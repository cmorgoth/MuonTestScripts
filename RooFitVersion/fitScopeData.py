import ROOT as rt
import sys
from array import *
    
if __name__ == '__main__':

    rt.gROOT.ProcessLine(".L RooTwoSideGaussian.cc+")
    rt.gSystem.Load("RooTwoSideGaussian_cc.so")

    workspace = rt.RooWorkspace("pulse")
    workspace.factory('W[1,-INF,+INF]')
    W = workspace.var("W")
    amp = rt.RooRealVar("amp","amp",-10,100)
    time = rt.RooRealVar("time","time",-20,250)

    # gaussian paramaeters
    gausBias  = rt.RooRealVar("gausBias", "Gauss Bias", 60, -1000, 1000)
    gausSigma = rt.RooRealVar("gausSigma", "Gaus Width", 10, 0.01, 1000)
    gausSigmaL = rt.RooRealVar("gausSigmaL", "Gaus Width L", .1, 0.001, 1000)
    gausSigmaR = rt.RooRealVar("gausSigmaR", "Gaus Width R", 10, 0.01, 1000)
    

    # crystal ball parameters
    cbBias  = rt.RooRealVar("cbBias", "CB Bias", -80, -1000, 1000)
    cbSigma = rt.RooRealVar("cbSigma", "CB Width", 80, 1, 1000)
    cbCut  = rt.RooRealVar("cbCut","CB Cut",10., 0.001, 1000)                        
    cbPower = rt.RooRealVar("cbPower","CB Power", 8., 0.001, 20.0)

    # exponential parameters
    expLambda = rt.RooRealVar("lambda","exponential slope", -0.015, -1., 0.)

    exp = rt.RooExponential("exp", "Exponential", time, expLambda)
    gaus = rt.RooTwoSideGaussian("gaus", "Two Side Gaussian", time, gausBias, gausSigmaL, gausSigmaR)
    cb = rt.RooCBShape("cb", "Crystal Ball", time, cbBias, cbSigma, cbCut, cbPower)
    model = rt.RooFFTConvPdf("model", "exp X gaus", time, exp, gaus)

    # retreive file
    inFile = rt.TFile.Open(sys.argv[1])
    tree = inFile.Get("outTree")

    # create dataset
    a = rt.RooArgSet()
    a.add(time)
    a.add(W)
    data = rt.RooDataSet('data','amplitude and time data',a)


    tree.Draw('>>elist','','entrylist')
    timeBins = array('d',[])
    elist = rt.gDirectory.Get('elist')
    entry = -1
    while True:
        entry = elist.Next()
        if entry == -1: break
        tree.GetEntry(entry)
        #set the RooArgSet and save
        for i in range(0,len(tree.Time)):
            if 1.0E+09*tree.Time[i]>-20 and 1.0E+09*tree.Time[i]<250.:
                timeBins.append(tree.Time[i])
                a = rt.RooArgSet()
                a.add(time)
                a.add(W)
                a.setRealValue('time',1.0E+09*tree.Time[i])
                a.setRealValue('W',-1.0*tree.Amp[i])
                #a.setRealValue('W',1.0)
                data.add(a)


    
    data = rt.RooDataSet(data.GetName(),data.GetTitle(),data,data.get(),"",W.GetName()) 
    data.Print("V")

    time.setRange("lowTime",-20,25.5)
    #time.setRange("highTime",40,250)
    time.setRange("highTime",27.8,250)
    
    
    #fr = model.fitTo(data,rt.RooFit.Save(),rt.RooFit.SumW2Error(False),rt.RooFit.Range("lowTime,highTime"))
    fr.Print("v")
    c = rt.TCanvas("c","c",500,500)
    frame = time.frame()
    data.plotOn(frame,rt.RooFit.Binning(data.numEntries()),rt.RooFit.DataError(rt.RooAbsData.None),rt.RooFit.MarkerStyle(8),rt.RooFit.MarkerSize(0.5))
    model.plotOn(frame)
    exp.plotOn(frame,rt.RooFit.LineColor(rt.kRed),rt.RooFit.Normalization(0.5))
    gaus.plotOn(frame,rt.RooFit.LineColor(rt.kGreen),rt.RooFit.Normalization(0.15))
    frame.Draw()
    c.Print("pulse.pdf")
    
