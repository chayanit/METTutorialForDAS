import FWCore.ParameterSet.Config as cms

process = cms.Process("DAS2019MET")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:../data/MET2018B_17Sep2018.root'
	#'file:../data/DYJetsToLL_M-50_amcatnloFXFX_Autumn18.root'
    )
)

process.metexercise = cms.EDAnalyzer('METAnalyzer',
			metSrc = cms.untracked.InputTag("slimmedMETs")
#			metSrc = cms.untracked.InputTag("slimmedMETsPuppi")
)

process.p = cms.Path(process.metexercise)



