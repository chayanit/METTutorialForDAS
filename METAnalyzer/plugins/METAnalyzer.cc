// -*- C++ -*-
//
// Package:    METTutorialForDAS/METAnalyzer
// Class:      METAnalyzer
//
/**\class METAnalyzer METAnalyzer.cc METTutorialForDAS/METAnalyzer/plugins/METAnalyzer.cc

 Description: Hands-on 2 Access MET objects in miniAOD 

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

// ROOT includes                                                                                                                                               
#include "TTree.h"
#include <TFile.h>
#include <TROOT.h>
#include "TBranch.h"
#include <string>
#include <vector>
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

class METAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit METAnalyzer(const edm::ParameterSet&);
      ~METAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      edm::InputTag metSrcTag_;
      edm::EDGetTokenT<edm::View<pat::MET>> metSrcToken_;
      int nEvent;      	

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
METAnalyzer::METAnalyzer(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

   metSrcTag_ = iConfig.getUntrackedParameter<edm::InputTag>("metSrc");
   metSrcToken_ = consumes<edm::View<pat::MET> >(metSrcTag_);
 
   nEvent=0;
   usesResource("TFileService");

}


METAnalyzer::~METAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
METAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;

   edm::Handle<edm::View<pat::MET> > meth;
   iEvent.getByToken(metSrcToken_,meth);
   const pat::MET &met = meth->front();

   // Print event information (run number, lumisection, event number)
   std::cout << " Print out : " << nEvent+1 << std::endl;
   std::cout << " run " << iEvent.id().run()
             << " lumi " << iEvent.id().luminosityBlock()
             << " event " << iEvent.id().event();

   // Print MET variables from each event
   std::cout << " pt " <<  met.pt()
             << " px " <<  met.px()
             << " py " <<  met.py()
             << " phi " <<  met.phi()
             << std::endl;

   nEvent++;
}


// ------------ method called once each job just before starting event loop  ------------
void
METAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
METAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

//define this as a plug-in
DEFINE_FWK_MODULE(METAnalyzer);
