#ifndef __BHTOYS_HISTOPROCESSOR_H__
#define __BHTOYS_HISTOPROCESSOR_H__

//HPSTR
#include "BumpHunter.h"
#include "FlatTupleMaker.h"
#include "HpsFitResult.h"

//ROOT
#include "Processor.h"
#include "TFile.h"
#include "TH1.h"

class TTree;


class BhToysHistoProcessor : public Processor {

    public:

        BhToysHistoProcessor(const std::string& name, Process& process);

        ~BhToysHistoProcessor();

        virtual void configure(const ParameterSet& parameters);

        virtual void initialize(std::string inFilename, std::string outFilename);

        virtual bool process();

        virtual void initialize(TTree* tree) {};

        virtual bool process(IEvent* event) { return true;};

        virtual void finalize();

    private:

        TFile* inF_{nullptr};

        // The bump hunter manager
        BumpHunter* bump_hunter_{nullptr};

        // The flat tuple manager
        FlatTupleMaker* flat_tuple_{nullptr};

        // The name of the mass spectrum to fit.
        std::string massSpectrum_{"testSpectrum_h"};

        // The mass spectrum to fit
        TH1* mass_spec_h{nullptr};

        // The signal shape histogram name to use, if defined.
        //std::string* signal_shape_h_name_{"bhTight/bhTight_vtx_InvM_h"};
        std::string signal_shape_h_name_{""};

        // The signal shpae histogram file path, if defined.
        //std::string* signal_shape_h_file_{"/volatile/hallb/hps/mccarty/anaBhAp100.root"};
        std::string signal_shape_h_file_{""};

        // The signal shape histogram to use.
        TH1* signal_shape_h_{nullptr};

        // The signal hypothesis to use in the fit.
        double mass_hypo_{100.0};
        
        // Order of polynomial used to model the background.
        int poly_order_{3};

        // Order of polynomial used to create the toy generator function.
        int toy_poly_order_{-1};

        // The factor that determines the size of the mass window as
        //      window_size = (mass_resolution*win_factor)
        int win_factor_{10};

        // The seed used in generating random numbers.  The default of 0 causes
        // the generator to use the system time.
        int seed_{10};

        // Number of toys to throw and fit
        int nToys_{50};

        // Number of samples for signal to employ in toy model generation. Defaults to zero.
        int toy_sig_samples_{0};

        // Toy background events are by default equal in number to the integral
        // of the invariant mass distribution. The number of events may be modified
        // by a multiplicative factor.
        int bkg_mult_{1};

        // The factor by which to scale the mass resolution function.
        double res_scale_{1.00};
        
        // Whether to use the asymptotic upper limit or the power constrained. Defaults to asymptotic.
        bool asymptotic_limit_{true};

        // What background model type to use.
        int bkg_model_{1};

        //Debug Level
        int debug_{0};

};

#endif
