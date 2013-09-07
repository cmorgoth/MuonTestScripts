//---------------------------------------------------------------------------
#ifndef ROO_Sine
#define ROO_Sine
//---------------------------------------------------------------------------
#include "RooAbsPdf.h"
#include "RooRealProxy.h"
//---------------------------------------------------------------------------
class RooRealVar;
class RooAbsReal;
//---------------------------------------------------------------------------
class RooSine : public RooAbsPdf
{
public:
   RooSine() {} ;
   RooSine(const char *name, const char *title,
      RooAbsReal &_x, RooAbsReal &_omega);
   RooSine(const RooSine& other,
      const char* name = 0);
   virtual TObject* clone(const char* newname) const { return new RooSine(*this,newname); }
   inline virtual ~RooSine() { }

   Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const;
   Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const;

protected:
   RooRealProxy X;        // dependent variable
   RooRealProxy Omega;    // Sine Angular Frequency

   Double_t evaluate() const;
private:
  ClassDef(RooSine,1) // Sine function
};
//---------------------------------------------------------------------------
#endif
