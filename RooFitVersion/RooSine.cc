//---------------------------------------------------------------------------
#include "RooFit.h"

#include "Riostream.h"
#include <math.h>

#include "RooSine.h"
#include "RooRealVar.h"

ClassImp(RooSine)
//---------------------------------------------------------------------------
RooSine::RooSine(const char *name,
		 const char *title,
		 RooAbsReal &_x, RooAbsReal &_omega) :
  RooAbsPdf(name, title), 
  X("X", "Dependent", this, _x),
  Omega("Omega", "Omega", this, _omega)
{
}
//---------------------------------------------------------------------------
RooSine::RooSine(const RooSine& other, const char* name) :
   RooAbsPdf(other, name), X("X", this, other.X), Omega("Omega", this, other.Omega)
{
}
//---------------------------------------------------------------------------
Double_t RooSine::evaluate() const
{
   double value = 0;

   value = sin(Omega*X);
   
   return value;

}
//---------------------------------------------------------------------------
Int_t RooSine::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const 
{
   return 0;
}
//---------------------------------------------------------------------------
Double_t RooSine::analyticalIntegral(Int_t code, const char* rangeName) const
{
   return 0;
}
//---------------------------------------------------------------------------

