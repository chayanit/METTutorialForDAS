import FWCore.ParameterSet.Config as cms

process = cms.Process("DAS2019MET")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:../data/MET2018B_17Sep2018.root'
	#'file:../data/DYJetsToLL_M-50_amcatnloFXFX_Autumn18.root'
    )
)

process.metexercise = cms.EDAnalyzer('METFilters',
				   doprints = cms.untracked.bool(False),
                                   metFilters = cms.InputTag("TriggerResults","","RECO"), # Data file
				   #metFilters = cms.InputTag("TriggerResults","","PAT"), # MC file	
                                   beamHaloFilterSel = cms.string("Flag_globalSuperTightHalo2016Filter"),
                                   hbheFilterSel = cms.string("Flag_HBHENoiseFilter"),
                                   hbheIsoFilterSel = cms.string("Flag_HBHENoiseIsoFilter"),
                                   ecalTPFilterSel = cms.string("Flag_EcalDeadCellTriggerPrimitiveFilter"),
                                   badPFMuonFilterSel = cms.string("Flag_BadPFMuonFilter"),
                                   badChargedCandFilterSel = cms.string("Flag_BadChargedCandidateFilter"),
                                   eeBadScFilterSel = cms.string("Flag_eeBadScFilter"),
) 

process.p = cms.Path(process.metexercise)



