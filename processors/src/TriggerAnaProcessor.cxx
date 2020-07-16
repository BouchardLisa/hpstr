
// * @file TriggerAnaProcessor.cxx
// * @brief AnaProcessor used fill histograms by trigger, based on MCAnaProcessor
// * @author Lisa Bouchard, University of New Hampshire
// * @date 6 12 2020

#include "TriggerAnaProcessor.h"
#include <iostream>
#include "HistoManager.h"
#include <string>

TriggerAnaProcessor::TriggerAnaProcessor(const std::string &name, Process &process) : Processor(name, process) {
    h1 = new TH1I("AllDatabyTrigger", "All Data by Trigger", 20, 0, 20);
    h2 = new TH1I("AllNeutralsByTrigger", "All Neutrals in Triggers", 20, 0, 20);
    h3 = new TH1I("TwoPlusNeutrals", "2 or More Neutrals", 20, 0, 20);
    h4 = new TH1I("AllData", "All Data > 100 MeV", 20, 0, 20);
    h5 = new TH1I("AllNeutrals", "All Neutrals > 100 MeV", 20, 0, 20);
    h6 = new TH1I("TwoOrMoreNeutrals", "2 or More Neutrals > 100 MeV", 20, 0, 20);
    neutrals = new TH1I("NeutralsperEvent", "Neutrals Per Event", 5, 0, 5);
    energy = new TH1I("Energy", "Energy of all neutrals > 100 Mev", 50, 0, 5);// no idea on appropriate bins yet
    layers = new TH1I("layers", "Layers 1 - 13 only", 15, 0, 15);
    sides = new TH1I("sides", "Sides", 8, 0, 8);
    ADC = new TH1I("ADC", "ADC", 100, 0, 100);
    sys = new TH1I("sys", "sys", 100, 0, 100);
    barrel = new TH1I("barrel", "barrel", 100, 0, 100);
    modu = new TH1I("modu", "modu", 5, 0, 5);
    sensor = new TH1I("sensor", "sensor", 100, 0, 100);
    strip = new TH1I("strip", "strip", 100, 0, 100);
    trackHit = new TH1I("trackerhit", "Top or bottom", 3, 0, 3);
    noHits = new TH1I("nohits", "Neutrals with no tracker hits", 0, 15, 0);
    rawhitenergy = new TH1I("rawhitenergy", "Raw hit energy", 50, 0, 5);
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
    delete sides;
    delete ADC;
    delete sys;
    delete barrel;
    delete modu;
    delete sensor;
    delete strip;
    delete noHits;
 //   delete trackHit;

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
    tree_->SetBranchAddress("SVTRawTrackerHits", &svtHits_, &bsvtHits_);
    tree_->SetBranchAddress("FinalStateParticles", &fsParts_, &bParts_);
    tree->SetBranchAddress("RotatedHelicalTrackHits", &rhth_, &brhth_);
    tree->SetBranchAddress("")

}

bool TriggerAnaProcessor::process(IEvent *ievent) {
   // FindNeutrals("prescaled.Single_0_Bot", 0, 0.1);
    //FindNeutrals("prescaled.Single_2_Bot", 1, 0.1);
    //FindNeutrals("prescaled.Single_2_Bot", 2, 0.1);

    AllNeutrals(0.1, 5);

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

    //plot all data greater than the energy threshold
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        if (p1->getEnergy() > EnergyThresh) { FillTrigHistos(h4); }
        //   std::cout << "first for loop" << std::endl;
    }

    //plot all neutrals greater than the energy threshold
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        if (p1->getCharge() == 0 && p1->getEnergy() > EnergyThresh) {
            FillTrigHistos(h5);
            energy->Fill(p1->getEnergy());
        }
        // std::cout << "in second for loop" << std::endl;
    }
    //plot all neutrals with greater than 2 per event and greater than the energy threshold
    int iter = 0;
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        if (p1->getCharge() == 0 && p1->getEnergy() > EnergyThresh) {
            ++iter;
            /*  if (iter == 2) {
                      //calc invariant mass
                      Particle *p0 = (*fsParts_)[i-1];
                      std::vector<double> mom0 = p0->getMomentum();
                      /std::vector<double> mom1 = p1->getMomentum();

                  }*/
        }
        if (iter > 1) { FillTrigHistos(h6); }
        //std::cout << "in third loop" << std::endl;
        neutrals->Fill(iter); //this will tell us how many neutrals are in each event
    }

    //plot all neutrals according to layer, then plot top and bottom layers separately
    for (int j = 0; j < svtHits_->size(); j++) {
        RawSvtHit *p2 = (*svtHits_)[j];
        int this_layer = p2->getLayer();
        if (this_layer <= max_layer) { layers->Fill(this_layer); }
    }


    for (int k = 0; k < rhth_->size(); k++) {
        //RotatedHelicalTrackHit is type track and not track hit????
        TrackerHit *t1 = (*rhth_)[k];
        //TRefArray *rawhits_ = t1->getSvtHits();
        /*for (int l = 0; l < rawhits_->GetSize(); l++) {
            // so I don't know how to take an element of a TRefArray and have the code recognize what data type it is.
            RawSvtHit this_hit = rawhits_[l];*/
            int this_rawlayer = t1->getLayer(); //there is definitely a getLayer for RawSvtHit
            rawlayer->Fill(this_rawlayer)
        //check if every t1 has a track associated with it? 

        }

        //get list of raw hits from RotatedHelicalTrackHits, then check which layer each raw hit is at and plot
        //global Z or other things
    }

    /*  //plot other things available in code right now
      sides->Fill(p2->getSide());
      sys->Fill(p2->getSystem());
      barrel->Fill(p2->getBarrel());
      modu->Fill(p2->getModule());
      sensor->Fill(p2->getSensor());
      strip->Fill(p2->getStrip());*/
  }

/* This is the general form for what I think I want:
 * I get the particle first,
 * then I get the track from the particle
 * but if there is no track, then that's what I want.
 *
 * then I look at the layer in that tracker hit(?) are they related?
 *
 * */
/*void TriggerAnaProcessor::NoTracks(double EnergyThresh) {
    for (int i = 0; i < fsParts_->size(); i++) {
            Particle *p1 = (*fsParts_)[i];
            auto track_id = p1->getTrack();
            h1->Fill(track_id);
           // std::cout << "track_id is " << track_id << std::endl;
            auto energy = p1->getEnergy();

            //if (track_id  energy > EnergyThresh)  {
            //    notracks->Fill(p1->getEnergy());
    }

*/
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
        sides->Write();
        ADC->Write();
        sys->Write();
        barrel->Write();
        modu->Write();
        sensor->Write();
        strip->Write();
        noHits->Write();
    }
}
void TriggerAnaProcessor::finalize() {
    SaveHistos(SaveOption);
}

DECLARE_PROCESSOR(TriggerAnaProcessor);
