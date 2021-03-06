
// * @file TriggerAnaProcessor.cxx
// * @brief AnaProcessor used fill histograms by trigger, based on MCAnaProcessor
// * @author Lisa Bouchard, University of New Hampshire
// * @date 6 12 2020

#include "TriggerAnaProcessor.h"
#include <iostream>
#include "HistoManager.h"
#include <string>
#include <math.h>

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
    noTracks = new TH1I{"notracks", "No tracks", 50, 0, 50};
    rawlayer = new TH1I{"rawlayer", "Raw Hit Layers of RHTH",0, 15, 0};
    rawtrackid = new TH1I{"rawtrackid", "Raw Track ID", 0, 1000, 0}; // no idea on appropriate bins here
    on_track_layer = new TH1I{"ontrackrawlayer", "On Track Raw Hit Layers of RHTH", 0, 100, 0};
    mass = new TH1I{"invar_mass", "Invar Mass 2500 bins, x from 0 to 2.5", 2500, 0, 2.5};
    mass2 = new TH1I{"energy_momentum", "Cluster Energy minus momentum", 5, 1, 10};
    mom1_histo = new TH1I{"momentum1", "Momentum of second particle in pair", 5, 0, 10};
    mom0_histo = new TH1I{"momentum0", "Momentum of first particle in pair", 5, 0, 10};
    energy1_histo = new TH1I{"particle_1_energy", "Energy of particle 1", 5, 0, 10};
    energy2_histo = new TH1I{"particle_2_energy", "Energy of particle 2", 5, 0, 10};
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
    delete rawhitenergy;
    delete noTracks;
    delete trackHit;
    delete mass;
    delete mass2;
    delete energy1_histo;
    delete energy2_histo;

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
    tree_->SetBranchAddress("RotatedHelicalTrackHits", &rhth_, &brhth_);
    tree_->SetBranchAddress("RotatedHelicalOnTrackHits", &rhonth_, &brhonth_);
    tree_->SetBranchAddress("GBLTracks", &tracks_, &bTrack_);

}

bool TriggerAnaProcessor::process(IEvent *ievent) {
  //  FindNeutrals("prescaled.Single_0_Bot", 0, 0.1);
    //FindNeutrals("prescaled.Single_2_Bot", 1, 0.1);
    //FindNeutrals("prescaled.Single_2_Bot", 2, 0.1);

    AllNeutrals(0.0, 15);  //(0.0, 15) gives all data
 //  FindNoTracks(0.01);

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
            auto energy_ = p1->getEnergy();
            if (p1->getCharge() == 0 && energy_ > EnergyThresh) {
                FillTrigHistos(trigger, h2);
                // this fills in the energy histo for every neutral particle
                energy->Fill(energy_); //seems bad that we are getting the energy twice here
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
    }

    //plot all neutrals greater than the energy threshold
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        if (p1->getCharge() == 0 && p1->getEnergy() > EnergyThresh) {
            FillTrigHistos(h5);
            energy->Fill(p1->getEnergy());
        }
    }

    //analyze the particles that are in both sets as neutrals
    std::vector<TObject> all_svthits_;
    std::vector<TObject> track_hits_;
    for (int i = 0; i < svtHits_->size(); i++) {
        TObject *t1 = (*svtHits_)[i];
        all_svthits_.push_back(*t1);
    }

    for (int i = 0; i < tracks_->size(); i++) {
        Track *t1 = (*tracks_)[i];
        TRefArray *hits = t1->getSvtHits();
        for (int j = 0; j < hits->GetEntries(); j++) {
            TObject *add_hit = (*hits)[j];
            track_hits_.push_back(*add_hit);
        }
    }

    //only finding invariant mass when there are exactly two neutrals
    int iter = 0;
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        if (p1->getCharge() == 0 && p1->getEnergy() > EnergyThresh) {
            ++iter;
             if (iter == 2) {
                 Particle *p0 = (*fsParts_)[i - 1];

                 CalCluster cluster1_ = p1->getCluster();
                 CalCluster cluster0_ = p0->getCluster();

                 auto Energy1_ = cluster1_.getEnergy();
                 auto Energy0_ = cluster0_.getEnergy();
                 energy1_histo->Fill(Energy0_);
                 energy2_histo->Fill(Energy1_);

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
                 auto cos_theta = mom_12/(TotalMom1*TotalMom0);
                 auto mass_ = 2*mom_12 *(1.0-cos_theta);
                 mass->Fill(mass_);
                 //confirm Energy is the same as the momentum
                 mass2->Fill(Energy1_ - TotalMom1);
             }
        }
        if (iter > 1) { FillTrigHistos(h6); }
        neutrals->Fill(iter); //this will tell us how many neutrals are in each event
    }

    //plot all neutrals according to layer, then plot top and bottom layers separately
    for (int j = 0; j < svtHits_->size(); j++) {
        RawSvtHit *p2 = (*svtHits_)[j];
        int this_layer = p2->getLayer();
        if (this_layer <= max_layer) { layers->Fill(this_layer); }
    }
}

void TriggerAnaProcessor::FindNoTracks(double EnergyThresh) {
// Take all RotatedHelicalTrackHits, remove all RotatedHelicalOnTrackHits to have all particles not on tracks
// and look at that data
    /*SaveOption = 4;

    //is this for loop in the wrong function?? These are not rhth
    for (int i = 0; i < fsParts_->size(); i++) {
        Particle *p1 = (*fsParts_)[i];
        Track track = p1->getTrack();
        if (track.getCharge() == 0) {std::cout << "zero particle" << std::endl;}
      //
      //  else {std::cout << "not zero particle" << std::endl;}
        //just looking at the raw track ids
        rawtrackid->Fill(track.getID());
        auto energy = p1->getEnergy();
        if (energy > EnergyThresh) {
            //looking at energy of all particles
            noTracks->Fill(p1->getEnergy());
        }
    }
    std::vector<int> t1_id;
    std::vector<int> t2_id;

    for (int k = 0; k < rhth_->size(); k++) {
        TrackerHit *t1 = (*rhth_)[k];
      //  std::cout << "charge is " << t1->getRawCharge() << std::endl;
        if (t1->getCharge() == 0) {
            int new_id = t1->getID();
            t1_id.push_back(new_id);
        }
        //do I need to get the raw hits from t1 or can I just use t1_id to subtract?
       // TRefArray *t1_rawhits = t1->getRawHits();
       // for (int a = 0; a = t1_rawhits->GetEntries(); a++){
        //    std::cout << "in rawhits get entries loop" << std::endl;
       // }
     //   std::cout << "t1_rawhits is " << t1_rawhits << std::endl;
//        auto t1_rawhits_size = t1_rawhits->GetEntries();

    //    std::cout << "t1_rawhits->size() is " << t1_rawhits_size << std::endl;

    }
     /*   // looking at layers of rhth
        for (int l = 0; l < rawhits_->GetEntries(); l++) {
            int this_rawlayer = t1->getLayer();
            rawlayer->Fill(this_rawlayer);
        }
    }

    for (int m = 0; m < rhonth_->size(); m++){
        TrackerHit *t2 = (*rhonth_)[m];

        if (t2->getCharge() == 0) {
           int new_id = t2->getID();
           t2_id.push_back(new_id);
        }
        //looking at layers of rhonth
     //   for (int n = 0; n < rawhits2_->GetEntries(); n++) {
      //      int new_rawlayer = t2->getLayer();
      //      on_track_layer->Fill(new_rawlayer);
      //  }
    }

    //Make a third vector by removing all the on track hits from rotated helical track hits a
    //and analyzing that data
    // can skip sort if I am certain they are already sorted
    //next 8 lines of code copied from stackoverflow
    std::sort(t1_id.begin(), t1_id.end());
    std::sort(t2_id.begin(), t2_id.end());

    std::vector<int> difference;
    std::set_difference(
            t1_id.begin(), t1_id.end(),
            t2_id.begin(), t2_id.end(),
            std::back_inserter( difference )
    );


    //plot all neutrals with greater than 2 per event and greater than the energy threshold
 /*   int iter = 0;
    for (int i = 0; i < difference.size(); i++) {

        TrackerHit *hit_ = (*rhth_)[difference[i]];


        //there is no energy in TrackerHit
       if (hit_->getCharge() == 0) {
            ++iter;
            std::cout << "charge is  "<< hit_->getCharge() << std::endl;
       }
*/
        //if iter = 2 calc invariant mass here
        // how many neutrals we have
       // if (iter > 1) { FillTrigHistos(h6); }
     //   neutrals->Fill(iter); //this will tell us how many neutrals are in each event


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
        mass->Write();
        mass2->Write();
        mom1_histo->Write();
        mom0_histo->Write();
        energy2_histo->Write();
        energy1_histo->Write();
       // sides->Write();
     //   ADC->Write();
     //   sys->Write();
     //   barrel->Write();
     //   modu->Write();
      //  sensor->Write();
      //  strip->Write();
     //   noHits->Write();
    }
    if (option == 4){
        rawtrackid->Write();
        noTracks->Write();
        rawlayer->Write();
        neutrals->Write();
    }
}

void TriggerAnaProcessor::finalize() {
    SaveHistos(SaveOption);
}

DECLARE_PROCESSOR(TriggerAnaProcessor);
