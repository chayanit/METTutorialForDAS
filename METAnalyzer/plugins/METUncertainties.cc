// -*- C++ -*-
//
// Package:    METTutorialForDAS/METAnalyzer
// Class:      METUncertainties
//
/**\class METUncertainties.cc METTutorialForDAS/METAnalyzer/plugins/METUncertainties.cc

 Description: Hands-on 5 Access to MET Uncertainties 

 Implementation:
     [Notes on implementation]
*/
//

// system include files
#include <memory>
#include <iostream>
#include <vector>
#include <TROOT.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

// ROOT includes                                                                                                                                               
#include "TTree.h"
#include <TFile.h>
#include <TROOT.h>
#include "TBranch.h"
#include <string>
#include <vector>
#include "TMath.h"
#include "TSystem.h"
#include "TVector3.h"
#include "TLorentzVector.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

class METUncertainties : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit METUncertainties(const edm::ParameterSet&);
      ~METUncertainties();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      bool ifPrint_;
      edm::InputTag metSrcTag_;
      edm::EDGetTokenT<edm::View<pat::MET>> metSrcToken_;
      int nEvent;      	

      // various met corrections
      TH1F *h_metpt;
      TH1F *h_metrawpt;
      TH1F *h_metsmearpt;

      // various met uncertainties
      TH1F *h_metjesuppt;
      TH1F *h_metjesdnpt;
      TH1F *h_metjeruppt;
      TH1F *h_metjerdnpt;
      TH1F *h_metuncuppt;
      TH1F *h_metuncdnpt;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
METUncertainties::METUncertainties(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
 
   ifPrint_     = iConfig.getUntrackedParameter<bool>("ifPrint");
   metSrcTag_   = iConfig.getUntrackedParameter<edm::InputTag>("metSrc");
   metSrcToken_ = consumes<edm::View<pat::MET> >(metSrcTag_);
 
   nEvent=0;
   edm::Service<TFileService> file;
   h_metpt = file->make<TH1F>("h_metpt" , "p_{T}^{miss}" , 100 , 0 , 200 );

   h_metjesuppt = file->make<TH1F>("h_metjesuppt" , "p_{T}^{miss}" , 100 , 0 , 200 );
   h_metjesdnpt = file->make<TH1F>("h_metjesdnpt" , "p_{T}^{miss}" , 100 , 0 , 200 );
   h_metjeruppt = file->make<TH1F>("h_metjeruppt" , "p_{T}^{miss}" , 100 , 0 , 200 );
   h_metjerdnpt = file->make<TH1F>("h_metjerdnpt" , "p_{T}^{miss}" , 100 , 0 , 200 );
   h_metuncuppt = file->make<TH1F>("h_metuncuppt" , "p_{T}^{miss}" , 100 , 0 , 200 );
   h_metuncdnpt = file->make<TH1F>("h_metuncdnpt" , "p_{T}^{miss}" , 100 , 0 , 200 );

   usesResource("TFileService");

}


METUncertainties::~METUncertainties()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
METUncertainties::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;

   edm::Handle<edm::View<pat::MET> > meth;
   iEvent.getByToken(metSrcToken_,meth);
   const pat::MET &met = meth->front();

   // MET unceetainties (print only first 5 events)
   if (ifPrint_ && (nEvent < 5)) {
     std::cout << " Type-1 PFMET : \n";
     std::cout << "  pt [GeV] = " << met.pt() << "\n"; 
     std::cout << "  phi [rad] = " << met.phi() << "\n";
     std::cout << " MET uncertainties : \n";
     std::cout << "  JES up/down [GeV] = " << met.shiftedPt(pat::MET::JetEnUp) << "/" << met.shiftedPt(pat::MET::JetEnDown) << "\n"; 
     std::cout << "  JER up/down [GeV] = " << met.shiftedPt(pat::MET::JetResUp) << "/" << met.shiftedPt(pat::MET::JetResDown) << "\n";
     std::cout << "  Unc up/down [GeV] = " << met.shiftedPt(pat::MET::UnclusteredEnUp) << "/" << met.shiftedPt(pat::MET::UnclusteredEnDown) << "\n";
     std::cout << "\n";
   }

   // store MET central value and uncertainties to the tree
   h_metpt->Fill(met.pt());

   h_metjesuppt->Fill(met.shiftedPt(pat::MET::JetEnUp));
   h_metjesdnpt->Fill(met.shiftedPt(pat::MET::JetEnDown));
   h_metjeruppt->Fill(met.shiftedPt(pat::MET::JetResUp));
   h_metjerdnpt->Fill(met.shiftedPt(pat::MET::JetResDown));
   h_metuncuppt->Fill(met.shiftedPt(pat::MET::UnclusteredEnUp));
   h_metuncdnpt->Fill(met.shiftedPt(pat::MET::UnclusteredEnDown));

   nEvent++;
}


// ------------ method called once each job just before starting event loop  ------------
void
METUncertainties::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
METUncertainties::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METUncertainties::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

//define this as a plug-in
DEFINE_FWK_MODULE(METUncertainties);
