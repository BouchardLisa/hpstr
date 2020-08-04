//
/*
 * @file InvarMassAnaProcessor.cxx
 * @brief AnaProcessor to fill histograms with invariant mass data
 * @author Lisa Bouchard, University of New Hampshire
 * @date 07/24/2020
 * 
*/

#include "InvarMassAnaProcessor.h"
#include <iostream>
#include "HistoManager.h"
#include <string>
#include <math.h>
#include<list>
#include<algorithm>

InvarMassAnaProcessor::InvarMassAnaProcessor(const std::string &name, Process &process) : Processor(name, process) {
    AllEnergy = new TH1I("AllEnergy", "Energy of all particles", 2500, 0, 2.5);
    EnergyThreshold = new TH1I("EnergyThreshold", "Energy of all particles above threshold", 2500, 0, 2.5);
    Neutrals = new TH1I{"AllNeutralsEnergy", "Energy of all neutral particles", 2500, 0, 2.5};
    InvarMass = new TH1I{"InvariantMass", "Invariant Mass, Sum Energies < beam ", 250, 0, 2.5};
    MassLayer = new TH1I{"MassLayer", "Invariant mass, sum of Energies < beam,  to layer 10", 250, 0, 0.02};
    SeparateY = new TH1I("SeparateY", "Energy of neutrals in opposite Y directions per pair", 250, 0, 2.5);
    TotalEBothNeutrals = new TH1I{"TotalEnergy", "Energy of both neutrals", 2500, 0, 5};
    TimeCut = new TH1I{"TimeCut", "T0 < 1 apart", 250, 0, 2.5};
}

//TODO CHECK THIS DESTRUCTOR
InvarMassAnaProcessor::~InvarMassAnaProcessor() {
    delete AllEnergy;
    delete EnergyThreshold;
    delete InvarMass;
    delete MassLayer;
    delete TotalEBothNeutrals;
    delete SeparateY;
    delete TimeCut;
    delete Neutrals;
}

void InvarMassAnaProcessor::configure(const ParameterSet &parameters) {
    std::cout << "Configuring InvarMassAnaProcessor" << std::endl;
    debug_ = 0;
}

void InvarMassAnaProcessor::initialize(TTree *tree) {
    std::cout << "Initializing InvarMassAnaProcessor" << std::endl;
    tree_ = tree;
    tree_->SetBranchAddress("EventHeader", &evtHead_, &bHeader_);
    tree_->SetBranchAddress("SVTRawTrackerHits", &svtHits_, &bsvtHits_);
    tree_->SetBranchAddress("FinalStateParticles", &fsParts_, &bParts_);
}

bool InvarMassAnaProcessor::process(IEvent *ievent) {
    AllNeutrals(0.3, 10);  //(0.0, 15) gives all data
    return true;
}

void InvarMassAnaProcessor::AllNeutrals(double EnergyThresh, int max_layer) {
    float beamEnergy_ = 2.4; // change this for use in 2016 and 2019 data
    int iter_ = 0;
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p2 = (*fsParts_)[i];
        auto energy_ = p2->getEnergy();
        // plot all energy without cuts
        AllEnergy->Fill(energy_);
        auto charge_ = p2->getCharge();
        if (charge_ == 0) {
            //plot energy of all neutrals
            Neutrals->Fill(energy_);
            iter_++;
            if (energy_ > EnergyThresh) {
                //plot energy of neutrals above the threshold
                EnergyThreshold->Fill(energy_);
                std::vector<double> momentum_; //= {nullptr};
                std::vector<double> energy_;
                std::vector<double> total_momentum_;
                if (iter_ > 1) {
                    Particle *p0 = (*fsParts_)[iter_ - 2];
                    Particle *p1 = (*fsParts_)[iter_ - 1];
                    double energy_ = p1->getEnergy();
                    auto p0x = p0->getMomentum()[0];
                    auto p0y = p0->getMomentum()[1];
                    auto p0z = p0->getMomentum()[2];
                    auto p1x = p1->getMomentum()[0];
                    auto p1y = p1->getMomentum()[1];
                    auto p1z = p1->getMomentum()[2];
                    auto TotalMom0_ = sqrt(p0x * p0x + p0y * p0y + p0z * p0z);
                    auto TotalMom1_ = sqrt(p1x * p1x + p1y * p1y + p1z * p1z);
                    double Energy0_ = p0->getEnergy();
                    double Energy1_ = p1->getEnergy();
                    //Plot energy of two neutrals
                    TotalEBothNeutrals->Fill(Energy1_ + Energy0_);

                    auto mom_12 = p1x * p1y * p1z + p0x * p0y * p0z;
                    auto cos_theta_ = mom_12 / (TotalMom1_ * TotalMom0_);
                    auto mass_ = 2 * mom_12 * (1.0 - cos_theta_);
                    if (Energy0_ + Energy1_ < beamEnergy_) {
                        //plot the invariant mass of the 2 neutrals
                        InvarMass->Fill(sqrt(mass_));
                        auto cluster2_ = p0->getCluster();
                        auto cluster1_ = p1->getCluster();
                        double Time0 = cluster2_.getTime();
                        double Time1 = cluster1_.getTime();
                        std::vector<double> y0_ = cluster1_.getPosition();
                        std::vector<double> y1_ = cluster2_.getPosition();
                        // plotting energy of neutrals when they have opposite y motions
                        // discounting all neutrals with a y = -1000 because that looked like an error
                        if (y0_[1] != -1000 || y1_[1] != -1000) {
                            if (y0_[1] > 0 && y1_[1] < 0) {
                                SeparateY->Fill(Energy0_);
                                SeparateY->Fill(Energy1_);
                            }
                            else if (y0_[1] < 0 && y1_[1] > 0) {
                                SeparateY->Fill(Energy0_);
                                SeparateY->Fill(Energy1_);
                            }
                        }
                        bool bad_layer = false;
                        // determine whether there are hits past the layer we want to iclude
                        for (int b = 0; b < svtHits_->size(); b++) {
                            if (!bad_layer) {
                                int this_layer = (*svtHits_)[b]->getLayer();
                                if (this_layer > max_layer) {
                                    bad_layer = true;
                                }
                            }
                        }
                        if (!bad_layer) {
                            //no hits in layer greater than specified
                            MassLayer->Fill(sqrt(mass_));
                            if (abs(Time0 - Time1) < 1.0000) {
                                // plot neutrals where the time interval is less than 10 ns (not nec. what I have now)
                                TimeCut->Fill(sqrt(mass_));
                            }
                        }
                    }
                }
            }
        }
    }
}

void InvarMassAnaProcessor::SaveHistos() {
    outF_->cd();
    AllEnergy->Write();
    Neutrals->Write();
    EnergyThreshold->Write();
    TotalEBothNeutrals->Write();
    InvarMass->Write();
    SeparateY->Write();
    MassLayer->Write();
    TimeCut->Write();
}

void InvarMassAnaProcessor::finalize() {
    SaveHistos();
}

DECLARE_PROCESSOR(InvarMassAnaProcessor);

