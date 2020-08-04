/*
 * @file InvarMassAnaProcessor.h
 * @brief header for AnaProcessor to fill histograms with invariant mass data
 * @author Lisa Bouchard, University of New Hampshire
 * @date 07/24/2020
 *
*/

#ifndef __INVARMASS_ANAPROCESSOR_H__
#define __INVARMASS_ANAPROCESSOR_H__

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

class InvarMassAnaProcessor : public Processor {

public:

    InvarMassAnaProcessor(const std::string& name, Process& process);

    ~InvarMassAnaProcessor();

    virtual bool process(IEvent* ievent);

    virtual void initialize(TTree* tree);

    virtual void finalize();

    virtual void configure(const ParameterSet& parameters);

    virtual void AllNeutrals(double Energy_Threshold, int Max_Layer);

    virtual void SaveHistos();



private:
    //TODO Change this to be held from HPSEvent
    TTree* tree_;

    TBranch *bHeader_{nullptr};
    TBranch *bParts_{nullptr};
    TBranch *bsvtHits_{nullptr};

    EventHeader *evtHead_{nullptr};
    std::vector<Particle*> *fsParts_{nullptr};
    std::vector<RawSvtHit*> *svtHits_{nullptr};


    TH1I *AllEnergy {nullptr};
    TH1I *EnergyThreshold {nullptr};
    TH1I *Neutrals {nullptr};
    TH1I *energy {nullptr};
    TH1I *InvarMass{nullptr};
    TH1I *TotalEBothNeutrals{nullptr};
    TH1I *MassLayer{nullptr};
    TH1I *SeparateY{nullptr};
    TH1I *TimeCut{nullptr};

    //Debug Level
    int debug_{0};

};

#endif //__INVARMASS_ANAPROCESSOR_H__
