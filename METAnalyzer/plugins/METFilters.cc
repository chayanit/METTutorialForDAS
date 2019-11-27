// -*- C++ -*-
//
// Package:    METTutorialForDAS/METAnalyzer
// Class:      METFilters
//
/**\class METFilters.cc METTutorialForDAS/METAnalyzer/plugins/METFilters.cc

 Description: Hands-on 6 Access to MET Filters 

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

#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

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

class METFilters : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit METFilters(const edm::ParameterSet&);
      ~METFilters();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      bool doprints_;
      edm::EDGetTokenT<edm::TriggerResults> metFiltersToken_;
      std::string beamHaloFilter_;
      std::string hbheFilter_;
      std::string hbheIsoFilter_;
      std::string ecalTPFilter_;
      std::string badPFMuonFilter_;
      std::string badChargedCandFilter_;
      std::string eeBadScFilter_;


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
METFilters::METFilters(const edm::ParameterSet& iConfig):
		      metFiltersToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("metFilters")))
{
   //now do what ever initialization is needed
 
   doprints_    = iConfig.getUntrackedParameter<bool>("doprints");

   beamHaloFilter_       = iConfig.getParameter<std::string>("beamHaloFilterSel");
   hbheFilter_           = iConfig.getParameter<std::string>("hbheFilterSel");
   hbheIsoFilter_        = iConfig.getParameter<std::string>("hbheIsoFilterSel");
   ecalTPFilter_         = iConfig.getParameter<std::string>("ecalTPFilterSel");
   badPFMuonFilter_      = iConfig.getParameter<std::string>("badPFMuonFilterSel");
   badChargedCandFilter_ = iConfig.getParameter<std::string>("badChargedCandFilterSel");
   eeBadScFilter_        = iConfig.getParameter<std::string>("eeBadScFilterSel");

}


METFilters::~METFilters()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
METFilters::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;
 
   edm::Handle<edm::TriggerResults> metFilters; 
   iEvent.getByToken(metFiltersToken_, metFilters);
   const edm::TriggerNames &names = iEvent.triggerNames(*metFilters);

   std::cout << "MET Filters decision: \n";
   for (unsigned int i = 0, n = metFilters->size(); i < n; ++i) {
     if (doprints_) { std::cout << names.triggerName(i) << std::endl; } 
     if (names.triggerName(i) == beamHaloFilter_) { std::cout << " Beam Halo = " << metFilters->accept(i) << "\n"; }
     if (names.triggerName(i) == hbheFilter_) { std::cout << " HBHE = " << metFilters->accept(i) << "\n"; }
     if (names.triggerName(i) == hbheIsoFilter_) { std::cout << " HBHE (Iso) = " << metFilters->accept(i) << "\n"; }
     if (names.triggerName(i) == ecalTPFilter_) { std::cout << " Ecal TP = " << metFilters->accept(i) << "\n"; }
     if (names.triggerName(i) == badPFMuonFilter_) { std::cout << " Bad PF Muon = " << metFilters->accept(i) << "\n"; }
     if (names.triggerName(i) == badChargedCandFilter_) { std::cout << " Bad Charged Hadron = " << metFilters->accept(i) << "\n"; }
     if (names.triggerName(i) == eeBadScFilter_) { std::cout << " EE SuperCluster = " << metFilters->accept(i) << "\n"; }
   }

}


// ------------ method called once each job just before starting event loop  ------------
void
METFilters::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
METFilters::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METFilters::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

//define this as a plug-in
DEFINE_FWK_MODULE(METFilters);
