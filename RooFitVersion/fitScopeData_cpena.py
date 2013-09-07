import ROOT as rt
import sys
from array import *
    
if __name__ == '__main__':

    rt.gROOT.ProcessLine(".L RooTwoSideGaussian.cc+")
    rt.gSystem.Load("RooTwoSideGaussian_cc.so")

    rt.gROOT.ProcessLine(".L RooSine.cc+")
    rt.gSystem.Load("RooSine_cc.so")

    workspace = rt.RooWorkspace("pulse")
    workspace.factory('W[1,-INF,+INF]')
    W = workspace.var("W")
    amp = rt.RooRealVar("amp","amp",-10,10)
    time = rt.RooRealVar("time","time",-15,240)

    #High Frequency Component
    Ome = rt.RooRealVar("Omega", "Ang. Freq", 950*1.E06, 5*1.E06, 10000*1.E06)

    #Landau
    L_mean = rt.RooRealVar("LMean", "#mu_{L}", 50,-1000, 1000)
    L_sigma = rt.RooRealVar("LSigma", "#sigma_{L}", 5, 0.001, 1000)
    Landau = rt.RooLandau("Landau", "Landau Dist", time, L_mean, L_sigma)

    #Normal Gaussian paramenter    
    NgausMean = rt.RooRealVar("NgausMean", "#mu", 50,-1000, 1000)
    NgausSigma = rt.RooRealVar("NgausSigma", "#sigma", 5, 0.001, 1000)

    # gaussian paramaeters
    gausBias  = rt.RooRealVar("gausBias", "Gauss Bias", 50, -1000, 1000)
    gausSigma = rt.RooRealVar("gausSigma", "Gaus Width", 5, 0.01, 1000)
    gausSigmaL = rt.RooRealVar("gausSigmaL", "Gaus Width L", .1, 0.001, 1000)
    gausSigmaR = rt.RooRealVar("gausSigmaR", "Gaus Width R", 10, 0.01, 1000)
    

    # crystal ball parameters
    cbBias  = rt.RooRealVar("cbBias", "CB Bias", -80, -1000, 1000)
    cbSigma = rt.RooRealVar("cbSigma", "CB Width", 80, 1, 1000)
    cbCut  = rt.RooRealVar("cbCut","CB Cut",10., 0.001, 1000)                        
    cbPower = rt.RooRealVar("cbPower","CB Power", 8., 0.001, 20.0)

    # exponential parameters
    Ngaus = rt.RooGaussian("Ngaus","Nomal Gaussian", time, NgausMean, NgausSigma)
    Sin = rt.RooSine("Sine", "MySine", time, Ome)
    expLambda = rt.RooRealVar("lambda","exponential slope", -0.010, -500.0, 0.)
    expLambda2 = rt.RooRealVar("lambda2","exponential2 slope", -0.050, -500.0, 0.)
    exp = rt.RooExponential("exp", "Exponential", time, expLambda)
    exp2 = rt.RooExponential("exp2", "Exponential2", time, expLambda2)
    gaus = rt.RooTwoSideGaussian("gaus", "Two Side Gaussian", time, gausBias, gausSigmaL, gausSigmaR)
    cb = rt.RooCBShape("cb", "Crystal Ball", time, cbBias, cbSigma, cbCut, cbPower)
   

    frac = rt.RooRealVar("frac","frac #exp1",0.5,0.,1.) 
    sum_exp = rt.RooAddPdf("SumExp", "exp1 + exp2",exp, exp2,frac)
    model = rt.RooFFTConvPdf("model", "exp * DoubleGauss", time, exp, gaus)
    model_2exp = rt.RooFFTConvPdf("model_2exp","(c1*exp1+c2*exp2)*Gauss", time, sum_exp, Ngaus)
    model_Lan2exp = rt.RooFFTConvPdf("model_Lan2exp","(c1*exp1+c2*exp2)*Landau", time, sum_exp, Landau)
    model_cph = rt.RooFFTConvPdf("model_cph", "exp * SingleGauss", time, exp, Ngaus)
    model_sine = rt.RooFFTConvPdf("model_sine", "sine * exp * SingleGauss", time, model_cph, Sin)
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
            if tree.Time[i]>-15*1.0E-09 and tree.Time[i]<240.*1.0E-09:
                #print "Time: ", tree.Time[i]
                timeBins.append(tree.Time[i])
                a = rt.RooArgSet()
                a.add(time)
                a.add(W)
                a.setRealValue('time',1.0E09*tree.Time[i])
                a.setRealValue('W',-1.0*tree.Amp[i])
                #a.setRealValue('W',1.0)
                data.add(a)


    
    data = rt.RooDataSet(data.GetName(),data.GetTitle(),data,data.get(),"",W.GetName()) 
    data.Print("V")

    time.setRange("lowTime",-20,25.0)
    #time.setRange("highTime",40,250)
    time.setRange("highTime",25.0,250)
    
    
    fr = model_Lan2exp.fitTo(data,rt.RooFit.Save(),rt.RooFit.SumW2Error(False),rt.RooFit.Range("lowTime,highTime"))
    fr.Print("v")
    c = rt.TCanvas("c","c",500,500)
    frame = time.frame()
    data.plotOn(frame,rt.RooFit.Binning(data.numEntries()),rt.RooFit.DataError(rt.RooAbsData.None),rt.RooFit.MarkerStyle(8),rt.RooFit.MarkerSize(0.5))
    model_Lan2exp.plotOn(frame)
    exp.plotOn(frame,rt.RooFit.LineColor(rt.kRed))
    exp2.plotOn(frame,rt.RooFit.LineColor(rt.kViolet))
    Landau.plotOn(frame,rt.RooFit.LineColor(rt.kGreen))
    #exp.plotOn(frame,rt.RooFit.LineColor(rt.kRed),rt.RooFit.Normalization(0.5))
    #exp2.plotOn(frame,rt.RooFit.LineColor(rt.kViolet),rt.RooFit.Normalization(0.5))
    #Ngaus.plotOn(frame,rt.RooFit.LineColor(rt.kGreen),rt.RooFit.Normalization(0.05))
    frame.Draw()
    c.Print("pulse_cph.pdf")
    
