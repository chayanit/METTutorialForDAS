import FWCore.ParameterSet.Config as cms

process = cms.Process("DAS2019MET")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:../data/MET2018B_17Sep2018.root'
	#'file:../data/DYJetsToLL_M-50_amcatnloFXFX_Autumn18.root'
    )
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("metCorrections.root"))

process.metexercise = cms.EDAnalyzer('METCorrections',
			metSrc = cms.untracked.InputTag("slimmedMETs"),
#			metSrc = cms.untracked.InputTag("slimmedMETsPuppi"),
			ifPrint = cms.untracked.bool(False),
			doPhiCorr = cms.untracked.bool(False),
) 

process.p = cms.Path(process.metexercise)



