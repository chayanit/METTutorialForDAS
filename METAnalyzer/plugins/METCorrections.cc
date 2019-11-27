// -*- C++ -*-
//
// Package:    METTutorialForDAS/METAnalyzer
// Class:      METCorrections
//
/**\class METCorrections METCorrections.cc METTutorialForDAS/METAnalyzer/plugins/METCorrections.cc

 Description: Hands-on 4 Learn about MET corrections 

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

class METCorrections : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit METCorrections(const edm::ParameterSet&);
      ~METCorrections();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      edm::InputTag metSrcTag_;
      bool ifPrint_;
      bool doPhiCorr_;     
      edm::EDGetTokenT<edm::View<pat::MET>> metSrcToken_;
      int nEvent;      	
      TH1F *hMET_pT_raw, *hMET_phi_raw;
      TH1F *hMET_pT, *hMET_phi;
      TH1F *hMET_pT_type1, *hMET_phi_type1;
      TH1F *hMET_pT_type1xy, *hMET_phi_type1xy;

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
METCorrections::METCorrections(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  ifPrint_     = iConfig.getUntrackedParameter<bool>("ifPrint");
  doPhiCorr_   = iConfig.getUntrackedParameter<bool>("doPhiCorr");
  metSrcTag_   = iConfig.getUntrackedParameter<edm::InputTag>("metSrc");
  metSrcToken_ = consumes<edm::View<pat::MET> >(metSrcTag_);
  
  nEvent=0;
  edm::Service<TFileService> fs;
  hMET_pT_raw   = fs->make<TH1F>("hMET_pT_raw",   "hMET_pT_raw",  100, 0.0, 200.0);
  hMET_phi_raw  = fs->make<TH1F>("hMET_phi_raw",  "hMET_phi_raw", 50, -TMath::Pi(), TMath::Pi());
  hMET_pT       = fs->make<TH1F>("hMET_pT",       "hMET_pT",      100, 0.0, 200.0);
  hMET_phi      = fs->make<TH1F>("hMET_phi",      "hMET_phi",     50, -TMath::Pi(), TMath::Pi());
  hMET_pT_type1  = fs->make<TH1F>("hMET_pT_type1",  "hMET_pT_type1", 100, 0.0, 200.0);
  hMET_phi_type1 = fs->make<TH1F>("hMET_phi_type1", "hMET_phi_type1",50, -TMath::Pi(), TMath::Pi());
  hMET_pT_type1xy  = fs->make<TH1F>("hMET_pT_type1xy",  "hMET_pT_type1xy", 100, 0.0, 200.0);
  hMET_phi_type1xy = fs->make<TH1F>("hMET_phi_type1xy", "hMET_phi_type1xy",50, -TMath::Pi(), TMath::Pi());
  usesResource("TFileService");

}


METCorrections::~METCorrections()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
METCorrections::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;

   edm::Handle<edm::View<pat::MET> > meth;
   iEvent.getByToken(metSrcToken_,meth);
   const pat::MET &met = meth->front();

   if(ifPrint_) {
   // Print event information (run number, lumisection, event number)
   std::cout << " run " << iEvent.id().run()
             << " lumi " << iEvent.id().luminosityBlock()
             << " event " << iEvent.id().event();
   // Print MET variables from each event
   std::cout << " pt " <<  met.pt()
             << " phi " <<  met.phi()
             << std::endl;
   }

   TLorentzVector rawMet(0,0,0,0);
   rawMet.SetPtEtaPhiE(met.uncorPt(),0,met.uncorPhi(),met.uncorPt());
   if(ifPrint_) {    
     	std::cout << " raw pt " <<  rawMet.Pt() << " " << met.corPt(pat::MET::Raw)
		  << " type1 " << met.corPt(pat::MET::Type1)
		  << " Type1Smear " << met.corPt(pat::MET::Type1Smear)
	      	  << " genMET "     << met.genMET()->pt() 
	      	  << std::endl;
   }

   // Access raw met
   hMET_pT_raw ->Fill(met.uncorPt());
   hMET_phi_raw->Fill(met.uncorPhi());

   // default met in miniAOD
   hMET_pT ->Fill(met.pt());
   hMET_phi->Fill(met.phi());
  
   // access type1 corrected met
   hMET_pT_type1 ->Fill(met.corPt(pat::MET::Type1));
   hMET_phi_type1->Fill(met.corPhi(pat::MET::Type1)); 

   // access phi corrected met type1 met, carefully check 
   // instructions as this requires additional packages 
   // to be checked out
   if(doPhiCorr_) {
   	hMET_pT_type1xy ->Fill(met.corPt(pat::MET::Type1XY));
    	hMET_phi_type1xy->Fill(met.corPhi(pat::MET::Type1XY));
   }

  // for additional corrections, look for METCorrectionLevel
  // in DataFormats/PatCandidates/interface/MET.h

   nEvent++;
}


// ------------ method called once each job just before starting event loop  ------------
void
METCorrections::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
METCorrections::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METCorrections::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

//define this as a plug-in
DEFINE_FWK_MODULE(METCorrections);
