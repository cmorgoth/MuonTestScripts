//---------------------------------------------------------------------------
#include "RooFit.h"

#include "Riostream.h"
#include <math.h>

#include "RooTwoSideGaussian.h"
#include "RooRealVar.h"

ClassImp(RooTwoSideGaussian)
//---------------------------------------------------------------------------
RooTwoSideGaussian::RooTwoSideGaussian(const char *name,
   const char *title,
   RooAbsReal &_x, RooAbsReal &_x0,
   RooAbsReal &_sigma_l, RooAbsReal &_sigma_r) :
   RooAbsPdf(name, title), 
   X("X", "Dependent", this, _x),
   X0("X0", "Gaussian Mean", this, _x0),
   SigmaL("SigmaL", "Left sigma", this, _sigma_l),
   SigmaR("SigmaR", "Right sigma", this, _sigma_r)
{
}
//---------------------------------------------------------------------------
RooTwoSideGaussian::RooTwoSideGaussian(const RooTwoSideGaussian& other, const char* name) :
   RooAbsPdf(other, name), X("X", this, other.X), X0("X0", this, other.X0),
   SigmaL("SigmaL", this, other.SigmaL), SigmaR("SigmaR", this, other.SigmaR)
{
}
//---------------------------------------------------------------------------
Double_t RooTwoSideGaussian::evaluate() const
{
   double value = 0;

   if(X < X0)   // Left-hand side, normal gaussian
      value = exp(-((X - X0) * (X - X0)) / (2 * SigmaL * SigmaL));
   else    // Right-hand side, normal gaussian
      value = exp(-((X - X0) * (X - X0)) / (2 * SigmaR * SigmaR));
   return value;

}
//---------------------------------------------------------------------------
Int_t RooTwoSideGaussian::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const 
{
   return 0;
}
//---------------------------------------------------------------------------
Double_t RooTwoSideGaussian::analyticalIntegral(Int_t code, const char* rangeName) const
{
   return 0;
}
//---------------------------------------------------------------------------

