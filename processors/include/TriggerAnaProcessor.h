//TriggerAnaProcessor.h
//Modeled after MCAnaProcessor.h
//Lisa Bouchard
//6/20/2020


#ifndef __TRIGGER_ANAPROCESSOR_H__
#define __TRIGGER_ANAPROCESSOR_H__

//HPSTR
#include <TH1I.h>
#include "HpsEvent.h"
#include "TSData.h"
#include "Collections.h"
#include "Particle.h"
#include "TrackerHit.h"
#include "CalHit.h"
#include "Event.h"
#include "Processor.h"
#include "FinalStateParticleProcessor.h"
#include "RawSvtHit.h"
//ROOT
#include "Processor.h"
#include "TClonesArray.h"
#include "TBranch.h"
#include "TTree.h"
#include "TFile.h"

class TTree;


class TriggerAnaProcessor : public Processor {

    public:

        TriggerAnaProcessor(const std::string& name, Process& process);

        ~TriggerAnaProcessor();

        virtual bool process(IEvent* ievent);

        virtual void initialize(TTree* tree);

        virtual void finalize();

        virtual void configure(const ParameterSet& parameters);

        virtual void FindNeutrals(std::basic_string<char> trigger, int options, double EnergyThresh_in_GeV);

        virtual void AllNeutrals(double EnergyThresh_in_GeV, int max_layer);

        virtual void FillTrigHistos(TH1I *h);

        virtual void FillTrigHistos(std::basic_string<char> trigger, TH1I *h);

        virtual void SaveHistos(int option);

       // virtual void NoTracks(double EnergyThresh_in_GeV);





private:


        //TODO Change this to be held from HPSEvent
        TTree* tree_;

        TBranch *bHeader_{nullptr};
        TBranch *bTrigger_{nullptr};
        TBranch *bCharge_{nullptr};
        TBranch *bParts_{nullptr};
        TBranch *bsvtHits_{nullptr};
        TBranch *brhth_{nullptr};
        TBranch *bTrackerHits_{nullptr};



        EventHeader *evtHead_{nullptr};
        TSData *trigger_{nullptr};

        std::vector<Particle*> *fsParts_{nullptr};
        std::vector<RawSvtHit*> *svtHits_{nullptr};
        std::vector<TrackerHit*> *rhth_{nullptr};
        std::vector<TrackerHit*> *tracker_hit_{nullptr};

        TH1I *h1 {nullptr};
        TH1I *h2 {nullptr};
        TH1I *h3 {nullptr};
        TH1I *h4 {nullptr};
        TH1I *h5 {nullptr};
        TH1I *h6 {nullptr};
        TH1I *neutrals {nullptr};
        TH1I *energy {nullptr};
        TH1I *layers {nullptr};
        TH1I *sides {nullptr};
        TH1I *ADC {nullptr};
        TH1I *modu {nullptr};
        TH1I *sys {nullptr};
        TH1I *barrel {nullptr};
        TH1I *sensor {nullptr};
        TH1I *strip {nullptr};
        TH1I *trackHit {nullptr};
        TH1I *noHits {nullptr};
        TH1I *rawhitenergy {nullptr};


        int SaveOption;

        //Debug Level
        int debug_{0};

};


#endif
