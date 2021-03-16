#include "flashgg/DataFormats/interface/SigmaMpTTag.h"

using namespace flashgg;

SigmaMpTTag::SigmaMpTTag() : DiPhotonTagBase::DiPhotonTagBase() {}

SigmaMpTTag::~SigmaMpTTag() {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : SigmaMpTTag::SigmaMpTTag( diPho, *mvaRes ) {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :  DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, std::map<std::string, edm::Ptr<flashgg::WeightedCompositeCandidate> > compObjMap )
    : SigmaMpTTag::SigmaMpTTag( diPho, *mvaRes, compObjMap ) {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, std::map<std::string, edm::Ptr<flashgg::WeightedCompositeCandidate> > compObjMap ) :  DiPhotonTagBase::DiPhotonTagBase( dipho,
            mvares ) {
    
    compObjMap_ = compObjMap;

}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, std::map<std::string, edm::Ptr<flashgg::WeightedCompositeCandidate> > compObjMap, std::map<std::string, float> extraWeightsMap )
    : SigmaMpTTag::SigmaMpTTag( diPho, *mvaRes, compObjMap, extraWeightsMap ) {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, std::map<std::string, edm::Ptr<flashgg::WeightedCompositeCandidate> > compObjMap, std::map<std::string, float> extraWeightsMap ) :  DiPhotonTagBase::DiPhotonTagBase( dipho,
            mvares ) {
    
    compObjMap_ = compObjMap;
    extraObjWeightsMap_ = extraWeightsMap;
}

const edm::Ptr<flashgg::WeightedCompositeCandidate > SigmaMpTTag::getCompCand(const std::string &name) const
{
    auto itr = compObjMap_.find(name);
    if(itr==compObjMap_.end()){
        assert(0);
    }
    return itr->second;
}

// void SigmaMpTTag::addExtraObjWeight(const std::string &name, const float &val)
// {
//     extraObjWeightsMap_.insert(std::pair<std::string, float>(name, val));
// }

const float SigmaMpTTag::getExtraObjWeight(const std::string &name) const
{
    auto itr = extraObjWeightsMap_.find(name);
    if(itr==extraObjWeightsMap_.end()){
        return 1.;
    }
    return itr->second;
}

SigmaMpTTag *SigmaMpTTag::clone() const
{
    SigmaMpTTag *result = new SigmaMpTTag( *this );
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

