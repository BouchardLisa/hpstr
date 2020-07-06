
// * @file TriggerAnaProcessor.cxx
// * @brief AnaProcessor used fill histograms by trigger, based on MCAnaProcessor
// * @author Lisa Bouchard, University of New Hampshire
// * @date 6 12 2020

#include "TriggerAnaProcessor.h"
#include <iostream>
#include "HistoManager.h"
#include <string>

TriggerAnaProcessor::TriggerAnaProcessor(const std::string &name, Process &process) : Processor(name, process) {
    h1 = new TH1I("AllDatbyTrigger", "All Data by Trigger", 20, 0, 20);
    h2 = new TH1I("AllNeutralsByTrigger", "All Neutrals in Triggers", 20, 0, 20);
    h3 = new TH1I("TwoPlusNeutrals", "2 or More Neutrals", 20, 0, 20);
    h4 = new TH1I("AllData", "All Data > 100 MeV", 20, 0, 20);
    h5 = new TH1I("AllNeutrals", "All Neutrals > 100 MeV", 20, 0, 20);
    h6 = new TH1I("TwoOrMoreNeutrals", "2 or More Neutrals > 100 MeV", 20, 0, 20);
    neutrals = new TH1I("NeutralsperEvent", "Neutrals Per Event", 5, 0, 5);
    energy = new TH1I("Energy", "Energy of all neutrals > 100 Mev", 50, 0, 5);// no idea on appropriate bins yet
    layers = new TH1I("layers", "Layers 1 - 13 only, Energy > 100 MeV", 15, 0, 15);

}

//TODO CHECK THIS DESTRUCTOR
TriggerAnaProcessor::~TriggerAnaProcessor() {
    delete h1;
    delete h2;
    delete h3;
    delete h4;
    delete h5;
    delete h6;
    delete neutrals;
    delete energy;
    delete layers;
}

void TriggerAnaProcessor::configure(const ParameterSet &parameters) {
    std::cout << "Configuring TriggerAnaProcessor" << std::endl;
    debug_ = 0;
}

void TriggerAnaProcessor::initialize(TTree *tree) {
    std::cout << "Initializing TriggerAnaProcessor" << std::endl;
    tree_ = tree;
    tree_->SetBranchAddress("EventHeader", &evtHead_, &bHeader_);
    tree_->SetBranchAddress("FinalStateParticles", &fsParts_, &bCharge_);
    tree_->SetBranchAddress("TSBank", &trigger_, &bTrigger_);
    tree_->SetBranchAddress("SVTRawHitsOnTrack", &svtHits_, &bsvtHits_);
    tree_->SetBranchAddress("FinalStateParticles", &fsParts_, &bParts_);
}

bool TriggerAnaProcessor::process(IEvent *ievent) {
   // FindNeutrals("prescaled.Single_0_Bot", 0, 0.1);
    //FindNeutrals("prescaled.Single_2_Bot", 1, 0.1);
    //FindNeutrals("prescaled.Single_2_Bot", 2, 0.1);

    AllNeutrals(0.1, 13);

    return true;
}

void TriggerAnaProcessor::FillTrigHistos(TH1I *h){
    //this should fill all triggers in 1 histo
    if (trigger_->prescaled.Single_0_Top) { h->Fill(0); }
    if (trigger_->prescaled.Single_0_Top) { h->Fill(0); }
    if (trigger_->prescaled.Single_0_Bot) { h->Fill(1); }
    if (trigger_->prescaled.Single_1_Top) { h->Fill(2); }
    if (trigger_->prescaled.Single_1_Bot) { h->Fill(3); }
    if (trigger_->prescaled.Single_2_Top) { h->Fill(4); }
    if (trigger_->prescaled.Single_2_Bot) { h->Fill(5); }
    if (trigger_->prescaled.Single_3_Top) { h->Fill(6); }
    if (trigger_->prescaled.Single_3_Bot) { h->Fill(7); }
    if (trigger_->prescaled.Pair_0)       { h->Fill(8); }
    if (trigger_->prescaled.Pair_1)       { h->Fill(9); }
    if (trigger_->prescaled.Pair_2)       { h->Fill(10); }
    if (trigger_->prescaled.Pair_3)       { h->Fill(11); }
    if (trigger_->prescaled.LED)          { h->Fill(12); }
    if (trigger_->prescaled.Cosmic)       { h->Fill(13); }
    if (trigger_->prescaled.Hodoscope)    { h->Fill(14); }
    if (trigger_->prescaled.Pulser)       { h->Fill(15); }
    if (trigger_->prescaled.Mult_0)       { h->Fill(16); }
    if (trigger_->prescaled.Mult_1)       { h->Fill(17); }
    if (trigger_->prescaled.FEE_Top)      { h->Fill(18); }
    if (trigger_->prescaled.FEE_Bot)      { h->Fill(19); }
}


void TriggerAnaProcessor::FillTrigHistos(std::basic_string<char> trigger, TH1I *h){
    //this should fill one bin per histo

    if (trigger == "prescaled.Single_0_Top") { h->Fill(0); };
    if (trigger == "prescaled.Single_0_Bot") { h->Fill(1); };
    if (trigger == "prescaled.Single_1_Top") { h->Fill(2); };
    if (trigger == "prescaled.Single_1_Bot") { h->Fill(3); };
    if (trigger == "prescaled.Single_2_Top") { h->Fill(4); };
    if (trigger == "prescaled.Single_2_Bot") { h->Fill(5); };
    if (trigger == "prescaled.Single_3_Top") { h->Fill(6); };
    if (trigger == "prescaled.Single_3_Bot") { h->Fill(7); };
    if (trigger == "prescaled.Pair_0")       { h->Fill(8); };
    if (trigger == "prescaled.Pair_1")       { h->Fill(9); };
    if (trigger == "prescaled.Pair_2")       { h->Fill(10); };
    if (trigger == "prescaled.Pair_3")       { h->Fill(11); };
    if (trigger == "prescaled.LED")          { h->Fill(12); };
    if (trigger == "prescaled.Cosmic")       { h->Fill(13); };
    if (trigger == "prescaled.Hodoscope")    { h->Fill(14); };
    if (trigger == "prescaled.Pulser")       { h->Fill(15); };
    if (trigger == "prescaled.Mult_0")       { h->Fill(16); };
    if (trigger == "prescaled.Mult_1")       { h->Fill(17); };
    if (trigger == "prescaled.FEE_Top")      { h->Fill(18); };
    if (trigger == "prescaled.FEE_Bot")      { h->Fill(19); };
}


void TriggerAnaProcessor::FindNeutrals(std::basic_string<char> trigger, int option, double EnergyThresh) {
// option 0 - don't look for neutral, just sort data by trigger
// option 1 - if any neutral is found in the event, add to bin
// option 2 - if 2 or more neutrals are found in the event, add to bin
    SaveOption = option;
    if (option == 0) {
        for (int i = 0; i < fsParts_->size(); i++) {
            Particle *p1 = (*fsParts_)[i];
            if (p1->getEnergy() > EnergyThresh) {
                FillTrigHistos(trigger, h1);
            }
        }
    }

    if (option == 1) {
        for (int i = 0; i < fsParts_->size(); i++) {
            Particle *p1 = (*fsParts_)[i];
            if (p1->getCharge() == 0 && p1->getEnergy() > EnergyThresh) {
                FillTrigHistos(trigger, h2);
                // if (trigger =="prescaled.Single_0_Top") { h2->Fill(0); };
                // this fills in the energy histo for every neutral particle
                energy->Fill(p1->getEnergy()); //seems bad that we are getting the energy twice here
            }
        }

    }

    if (option == 2) {
        int iter = 0;
        for (int i = 0; i < fsParts_->size(); i++) {
            Particle *p1 = (*fsParts_)[i];
            if (p1->getCharge() == 0 && p1->getEnergy() > EnergyThresh) { ++iter; }
            neutrals->Fill(iter);  //This will tell us how many neutrals are in each event.
            if (iter > 1) {
              //  if (trigger == "prescaled.Single_0_Top") { h3->Fill(0); };
                FillTrigHistos(trigger, h3);
            }
        }

    }
}

void TriggerAnaProcessor::AllNeutrals(double EnergyThresh, int max_layer) {
    SaveOption = 3;
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        if (p1->getEnergy() > EnergyThresh) {FillTrigHistos(h4);}
    }

    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        if (p1->getCharge() == 0 && p1->getEnergy() > EnergyThresh) {
            FillTrigHistos(h5);
            energy->Fill(p1->getEnergy());
        }
    }
    int iter = 0;
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        if (p1->getCharge() == 0 && p1->getEnergy() > EnergyThresh) { ++iter; }
        if (iter > 1) {FillTrigHistos(h6);}
        neutrals->Fill(iter); //this will tell us how many neutrals are in each event
    }
    for (int j = 0; j < svtHits_->size(); j++) {
        RawSvtHit *p2 = (*svtHits_)[j];
        int this_layer = p2->getLayer();
        if (this_layer <= max_layer){layers->Fill(this_layer);}
    }
}

void TriggerAnaProcessor::SaveHistos(int option) {
    outF_->cd();
    if (option == 0) {
        h1->Write();
    }
    if (option == 1) {
        h2->Write();
        energy->Write();
    }
    if (option == 2) {
        h3->Write();
        neutrals->Write();
    }
    if (option == 3) {
        h4->Write();
        h6->Write();
        energy->Write();
        neutrals->Write();
        layers->Write();
    }
}
void TriggerAnaProcessor::finalize() {
    SaveHistos(SaveOption);
}

DECLARE_PROCESSOR(TriggerAnaProcessor);
