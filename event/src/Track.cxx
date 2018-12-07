/**
 * @file Track.h
 * @brief Class used to encapsulate track information.
 * @author Omar Moreno, SLAC National Accelerator Laboratory
 */

#include "Track.h"

ClassImp(Track)

Track::Track()
    : TObject() { 
}

Track::~Track() {
    Clear();
    delete tracker_hits_;
}

void Track::Clear(Option_t* /* option */) {
    TObject::Clear(); 
    tracker_hits_->Delete();
    memset(isolation_, 0, sizeof(isolation_)); 
    n_hits_ = 0; 
}

void Track::setTrackParameters(double d0, double phi0, double omega,
                                  double tan_lambda, double z0) {
    d0_         = d0;
    phi0_       = phi0;
    omega_      = omega;
    tan_lambda_ = tan_lambda;
    z0_         = z0;
}

std::vector<double> Track::getTrackParameters() { return { d0_, phi0_, omega_, tan_lambda_, z0_ }; }

void Track::setPositionAtEcal(const double* position) { 
    x_at_ecal_ = position[0]; 
    y_at_ecal_ = position[1];
    z_at_ecal_ = position[2];
}

std::vector<double> Track::getPositionAtEcal() { return { x_at_ecal_, y_at_ecal_, z_at_ecal_ }; }

//int Track::getCharge() { 
//    if (fs_particle == NULL) return 9999;
//    return ((HpsParticle*) this->fs_particle.GetObject())->getCharge();
//}

//std::vector<double> Track::getMomentum() {
//    if (fs_particle == NULL) return {0, 0, 0}; 
//    return ((HpsParticle*) this->fs_particle.GetObject())->getMomentum();
//}

void Track::addHit(TrackerHit* hit) {
    ++n_hits_; 
    tracker_hits_->Add(static_cast<TObject*>(hit)); 
}