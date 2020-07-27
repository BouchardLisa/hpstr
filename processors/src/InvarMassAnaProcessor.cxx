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

InvarMassAnaProcessor::InvarMassAnaProcessor(const std::string &name, Process &process) : Processor(name, process) {
    AllEnergy = new TH1I("AllEnergy", "Energy of all particles", 2500, 0, 2.5);
    EnergyThreshold = new TH1I("EnergyThreshold", "Energy of all particles above threshold", 2500, 0, 2.5);
    AllNeutralsHisto = new TH1I("AllNeutralsByEnergy", "All Neutrals by Energy", 2500, 0, 2.5);
    mom1_histo = new TH1I{"Momentum1", "Momentum of second neutral in pair", 5, 0, 10};
    mom0_histo = new TH1I{"Momentum0", "Momentum of first neutral in pair", 5, 0, 10};
    mass = new TH1I{"InvariantMass", "Invariant Mass, Sum Energies < beam", 250, 0, 1.1};
    mass_layer = new TH1I{"MassLayer", "Invariant mass to layer 9", 9, 0, 9};
    neutrals = new TH1I("NeutralsperEvent", "Number of neutrals per event", 10, 0, 10);
    SeparateY = new TH1I ("SeparateY", "Energy of neutrals in opposite Y directions per pair", 250, 0, 1.1);
    energy = new TH1I("Energy", "Energy of all neutrals", 2500, 0, 2.5);// no idea on appropriate bins yet
    energy0_histo = new TH1I{"particle_1_energy", "Energy of particle 1", 5, 0, 10};
    energy1_histo = new TH1I{"particle_2_energy", "Energy of particle 2", 5, 0, 10};
    total_energy_histo = new TH1I {"totalenergy", "Energy of both neutrals", 2500, 0, 5};
    NeutralsByPair = new TH1I{"NeutralsByPair", "Layers with paired neutrals", 250, 0, 5};
}

//TODO CHECK THIS DESTRUCTOR
InvarMassAnaProcessor::~InvarMassAnaProcessor() {
    delete AllEnergy;
    delete EnergyThreshold;
    delete neutrals;
    delete energy;
    delete mass;
    delete mass_layer;
    delete energy1_histo;
    delete energy0_histo;
    delete total_energy_histo;
    delete AllNeutralsHisto;
    delete mom1_histo;
    delete mom0_histo;
    delete SeparateY;
    delete NeutralsByPair;
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

void InvarMassAnaProcessor::AllNeutrals(double EnergyThresh, int max_layer) {
    float beamEnergy_ = 2.4; // change this for use in 2016 and 2019 data
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        int iter = 0;
        auto energy_ = p1->getEnergy();
        // plot all data regardless of energy or charge
        AllEnergy->Fill(energy_);
        //plot energy of all neutrals
        auto charge_ = p1->getCharge();
        if (charge_ == 0){
            AllNeutralsHisto->Fill(energy_);
        }
        //plot all data greater than the energy threshold
        if (energy_ > EnergyThresh) {
            EnergyThreshold->Fill(energy_);
            //plot all neutrals greater than the energy threshold
            if (charge_ == 0) {
                energy->Fill(p1->getEnergy());
                NeutralsByPair->Fill(iter);  //This will tell us how many neutrals are in each event.
                // start histos for just 2 neutrals
                ++iter;
                if (iter == 2) {
                    Particle *p0 = (*fsParts_)[i - 1];
                    CalCluster cluster1_ = p1->getCluster();
                    CalCluster cluster0_ = p0->getCluster();
                    auto Energy1_ = cluster1_.getEnergy();
                    auto Energy0_ = cluster0_.getEnergy();
                    /*std::vector<double> y0_ = cluster0_.getPosition();
                    std::vector<double> y1_ = cluster1_.getPosition();
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
*/
                    energy0_histo->Fill(Energy0_);
                    energy1_histo->Fill(Energy1_);
                    total_energy_histo ->Fill(Energy1_+Energy0_);
                    auto p1x = p1->getMomentum()[0];
                    auto p1y = p1->getMomentum()[1];
                    auto p1z = p1->getMomentum()[2];

                    auto TotalMom1 = sqrt(p1x * p1x + p1y * p1y + p1z * p1z);
                    mom1_histo->Fill(TotalMom1);

                    auto p0x = p0->getMomentum()[0];
                    auto p0y = p0->getMomentum()[1];
                    auto p0z = p0->getMomentum()[2];
                    auto TotalMom0 = sqrt(p0x * p0x + p0y * p0y + p0z * p0z);
                    mom0_histo->Fill(TotalMom0);
                    auto mom_12 = p1x * p1y * p1z + p0x * p0y * p0z;
                    auto cos_theta = mom_12 / (TotalMom1 * TotalMom0);
                    auto mass_ = 2 * mom_12 * (1.0 - cos_theta);
                    if (Energy0_+ Energy1_ < beamEnergy_) {
                        for (int j = 0; j < svtHits_->size(); j++) {
                            mass->Fill(mass_);
                            RawSvtHit *p2 = (*svtHits_)[j];
                            int this_layer = p2->getLayer();
                            if (this_layer < 10) {
                                mass_layer->Fill(mass_);
                            }
                        }
                    }
                }
            }
        }
        //not sure this is still in the right loop.
        neutrals->Fill(iter); //this will tell us how many neutrals are in each event
    }
}

void InvarMassAnaProcessor::SaveHistos() {
    outF_->cd();
    AllEnergy->Write();
    EnergyThreshold->Write();
    energy->Write();
    neutrals->Write();
    mass->Write();
    mass_layer->Write();
    mom1_histo->Write();
    mom0_histo->Write();
    energy0_histo->Write();
    energy1_histo->Write();
    total_energy_histo->Write();
    AllNeutralsHisto->Write();
    SeparateY->Write();
    NeutralsByPair->Write();
}


void InvarMassAnaProcessor::finalize() {
    SaveHistos();
}

DECLARE_PROCESSOR(InvarMassAnaProcessor);

