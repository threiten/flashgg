#include "flashgg/DataFormats/interface/WeightedCompositeCandidate.h"

namespace flashgg
{
  
  WeightedCompositeCandidate::~WeightedCompositeCandidate() {}

  WeightedCompositeCandidate::WeightedCompositeCandidate(const Candidate& p, const std::string& name) :
    CompositeCandidate(p, name),
    WeightedObject()
  {}

  WeightedCompositeCandidate::WeightedCompositeCandidate(const Candidate& p, const std::string& name, role_collection const& roles) :
    CompositeCandidate(p, name, roles),
    WeightedObject()
  {}

  WeightedCompositeCandidate* WeightedCompositeCandidate::clone() const { return new WeightedCompositeCandidate(*this); }

  void WeightedCompositeCandidate::addDaughter(const flashgg::WeightedObject& wCand, const std::string& s) {
    includeWeights(wCand);
    const reco::Candidate *cand = dynamic_cast<const reco::Candidate *>(&wCand);
    CompositeCandidate::addDaughter(*cand, s);
  }
}
