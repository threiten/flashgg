#ifndef __HadronicActivityProducer_h__
#define __HadronicActivityProducer_h__

#include <string>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/libminifloat.h"

#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositePtrCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "flashgg/DataFormats/interface/WeightedCompositeCandidate.h"

namespace flashgg { 
	template<class T> struct TrivialVetex { size_t operator()(const T& obj) { return 0; } };
	
    template <class T, class V=TrivialVetex<typename T::value_type> >
    class HadronicActivityProducer : public edm::global::EDProducer<> {
        
    public:
        explicit HadronicActivityProducer(const edm::ParameterSet&);
        ~HadronicActivityProducer();
        
        virtual void produce(edm::StreamID, edm::Event&, const edm::EventSetup&) const;
        
    private:
        std::vector<edm::EDGetTokenT<edm::View<flashgg::WeightedObject> > > srcTokens_;
        int max_;
        bool veto_;
        double vetocone_;
        edm::EDGetTokenT<T> vetoToken_;
        std::vector<edm::InputTag> srcTags;
    };
    
    template <class T, class V>
    HadronicActivityProducer<T,V>::HadronicActivityProducer(const edm::ParameterSet& iConfig) : 
        max_(-1),
        veto_( iConfig.exists("veto") ),
        vetocone_( 0.4 )
    {
        if( iConfig.exists("maxCand") ) {
            max_ = iConfig.getParameter<int>("maxCand");
        }

        if( iConfig.existsAs<edm::InputTag>("src") ) { 
            srcTags.push_back(iConfig.getParameter<edm::InputTag> ( "src" ) ); 
        } else { 
            srcTags = iConfig.getParameter<std::vector<edm::InputTag> > ( "src" ); 
        }
        for( auto & tag : srcTags ) { srcTokens_.push_back( consumes<edm::View<flashgg::WeightedObject> >( tag ) );}

        if( veto_ ) { vetoToken_ = consumes<T>( iConfig.getParameter<edm::InputTag> ( "veto" ) ); }
        if( iConfig.exists("vetocone") ) { vetocone_ = iConfig.getParameter<double>("vetocone"); }

        produces<std::vector<reco::CompositeCandidate> >();	
    }
    
    template <class T, class V>
    HadronicActivityProducer<T,V>::~HadronicActivityProducer() {}
    
    template <class T, class V>
    void HadronicActivityProducer<T,V>::produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const {
        
        std::unique_ptr<std::vector<reco::CompositeCandidate> > outPtr(new std::vector<reco::CompositeCandidate>);
       
        edm::Handle<T> veto;
        if( veto_ )  {
            iEvent.getByToken( vetoToken_,  veto);
        }
        size_t N_dipho = veto_ ? veto->size() : 1;
        for(size_t idipho=0;  idipho< N_dipho; idipho++){
            flashgg::WeightedCompositeCandidate out; 
            size_t index = 0;
            if( veto_ )  {
                //    iEvent.getByToken( vetoToken_,  veto);
                if( veto->size() > 0 ) index = V()(veto->at(0));
            }
            if( index > srcTokens_.size()-1 ) { index = 0; }
            
            edm::Handle<edm::View<flashgg::WeightedObject> > src;
            iEvent.getByToken( srcTokens_[index],  src);
            auto & collection = *src;
            
            int count = ( max_ > 0 ? max_ : collection.size() );
            bool isReco = false;
            std::cout << "-------------------InputTags-----------------" << std::endl;
            for ( auto &tag : srcTags) {
                if (tag.label().find("reco") != std::string::npos || tag.label().find("Reco") != std::string::npos){
                    isReco = true;
                }
                std::cout << "Source Tag: "<< tag << "isReco: " << isReco <<std::endl;
            }
            std::cout << "-------------------InputTags-----------------" << std::endl;
            for( size_t iob = 0; iob<collection.size() && count > 0; ++iob ) {
                auto & cand = collection.at(iob);
                bool add = true;
                // if( ( veto_ && veto->size() > 0 ) &&
                //     ( reco::deltaR(*(veto->at(idipho).leadingPhoton()),cand) < vetocone_ || reco::deltaR(*(veto->at(idipho).subLeadingPhoton()),cand) < vetocone_ ) ) { add=false; }
                if( add ) {
                    // const flashgg::WeightedObject *wObj = dynamic_cast<const flashgg::WeightedObject *>(&cand);
                    out.addDaughter(cand);
                    if ( isReco ){
                        if (out.hasWeight("JetBTagCutWeight")){
                            std::cout << "JetBTagCutWeight HadronicActivityProducer: " << out.weight("JetBTagCutWeight") << std::endl;
                        } else {
                            std::cout << "JetBTagCutWeight HadronicActivityProducer not found!" << std::endl;
                        }
                        if (out.hasWeight("JetBTagCutWeightCentral")){
                            std::cout << "JetBTagCutWeightCentral HadronicActivityProducer: " << out.weight("JetBTagCutWeightCentral") << std::endl;
                        } else {
                            std::cout << "JetBTagCutWeightCentral HadronicActivityProducer not found!" << std::endl;
                        }
                    }
                    --count;
                }
            }
            
            AddFourMomenta addP4;
            addP4.set(out);
            outPtr->push_back(out);
        }
        
        iEvent.put(std::move(outPtr));
    }
}

/// #include "FWCore/Framework/interface/MakerMacros.h"
/// DEFINE_FWK_MODULE(HadronicActivityProducer);

#endif // __HadronicActivityProducer_h__
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

