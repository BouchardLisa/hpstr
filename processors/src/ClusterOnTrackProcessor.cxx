#include "ClusterOnTrackProcessor.h"
#include "TBranch.h"

ClusterOnTrackProcessor::ClusterOnTrackProcessor(const std::string& name, Process& process) : Processor(name,process){}
//TODO CHECK THIS DESTRUCTOR
ClusterOnTrackProcessor::~ClusterOnTrackProcessor(){}


void ClusterOnTrackProcessor::configure(const ParameterSet& parameters) {
  
  baselineFits_ = parameters.getString("BaselineFits");
  baselineRun_  = parameters.getString("BaselineRun");
  std::cout<<"Configured: "<<baselineFits_<<" "<<baselineRun_<<std::endl;
  
}

void ClusterOnTrackProcessor::initialize(TTree* tree) {
  clusterHistos = new ClusterHistos("hitOnTrack_2D");
  
  if (!baselineFits_.empty() && !baselineRun_.empty()) {
    clusterHistos->setBaselineFitsDir(baselineFits_);
    if (!clusterHistos->LoadBaselineHistos(baselineRun_))
      std::cout<<"WARNING: baselines not loaded in Cluster on Track histos."<<std::endl;}
  
  clusterHistos->Define1DHistos();
  clusterHistos->Define2DHistos();
  tree_= tree;
  //TODO Change this.
  tree_->SetBranchAddress("GBLTracks",&tracks_,&btracks_);
  //TODO Change this.
  //outF_ = new TFile("outputFile.root","recreate");
  
}

bool ClusterOnTrackProcessor::process(IEvent* ievent) {

  
  for (int itrack = 0; itrack<tracks_->size();itrack++) {
    Track *track = tracks_->at(itrack);
    //Loop on hits
    for (int ihit = 0; ihit<track->getSvtHits()->GetEntries(); ++ihit) {
      TrackerHit* hit3d = (TrackerHit*) track->getSvtHits()->At(ihit);
      clusterHistos->FillHistograms(hit3d,1.);
    }
  }
  return true;
}

void ClusterOnTrackProcessor::finalize() {

  clusterHistos->saveHistos(outF_,"");
  //outF_->Close();
  delete clusterHistos;
  clusterHistos = nullptr;
}

DECLARE_PROCESSOR(ClusterOnTrackProcessor);
