//---------------------------------------------------------------------------
#ifndef ROO_TwoSideGaussian
#define ROO_TwoSideGaussian
//---------------------------------------------------------------------------
#include "RooAbsPdf.h"
#include "RooRealProxy.h"
//---------------------------------------------------------------------------
class RooRealVar;
class RooAbsReal;
//---------------------------------------------------------------------------
class RooTwoSideGaussian : public RooAbsPdf
{
public:
   RooTwoSideGaussian() {} ;
   RooTwoSideGaussian(const char *name, const char *title,
      RooAbsReal &_x, RooAbsReal &_x0,
      RooAbsReal &_sigma_l, RooAbsReal &_sigma_r);
   RooTwoSideGaussian(const RooTwoSideGaussian& other,
      const char* name = 0);
   virtual TObject* clone(const char* newname) const { return new RooTwoSideGaussian(*this,newname); }
   inline virtual ~RooTwoSideGaussian() { }

   Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const;
   Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const;

protected:
   RooRealProxy X;        // dependent variable
   RooRealProxy X0;       // center of gaussian
   RooRealProxy SigmaL;   // width of gaussian
   RooRealProxy SigmaR;   // width of gaussian
   RooRealProxy S;        // exponent of the tail - has to be greater than zero for now

   Double_t evaluate() const;
private:
  ClassDef(RooTwoSideGaussian,1) // TwoSideGaussian function
};
//---------------------------------------------------------------------------
#endif
