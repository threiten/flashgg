#include "flashgg/DataFormats/interface/WeightedCompositeCandidate.h"

namespace flashgg {
  
  template<class T>
  void WeightedCompositeCandidate::addDaughter(const edm::Ptr<T> cand){
    CompositeCandidate::addDaughter(*cand);
    includeWeights(*cand);
  }
  
  template<>
  void WeightedCompositeCandidate::addDaughter(const edm::Ptr<flashgg::GenJetExtra> cand){
    CompositeCandidate::addDaughter(*cand);
  }
  
  template<>
  void WeightedCompositeCandidate::addDaughter(const edm::Ptr<reco::GenJet> cand){
    CompositeCandidate::addDaughter(*cand);
  }
  
  template<>
  void WeightedCompositeCandidate::addDaughter(const edm::Ptr<flashgg::GenLeptonExtra> cand){
    CompositeCandidate::addDaughter(*cand);
  }
}
