#ifndef FLASHgg_WeightedCompositeCandidate_h
#define FLASHgg_WeightedCompositeCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"


namespace flashgg {

  class WeightedCompositeCandidate : public reco::CompositeCandidate, public WeightedObject
  {
  public:
    WeightedCompositeCandidate(std::string name = "") : reco::CompositeCandidate(), WeightedObject() {}
    WeightedCompositeCandidate(const Candidate& p, const std::string& name = "");
    WeightedCompositeCandidate(const Candidate& p, const std::string& name, role_collection const& roles);
    virtual ~WeightedCompositeCandidate() override;
    WeightedCompositeCandidate* clone() const;
    void addDaughter(const flashgg::WeightedObject&, const std::string& s = "");
  };
}

#endif
