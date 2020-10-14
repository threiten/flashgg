//----------------------------------------------------------------------------------------
// Produce tag and probe pairs out of DiPhotons candidate
//
//----------------------------------------------------------------------------------------

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/TagAndProbeCandidate.h"
#include "flashgg/MicroAOD/interface/CutBasedPhotonViewSelector.h"

using namespace edm;
using namespace std;

namespace flashgg {

    class TagAndProbeProducer : public EDProducer
    {
    public:
        //---typedef
        typedef StringCutObjectSelector<Photon, true> selector_type;
        
        //---ctors
        TagAndProbeProducer( );
        TagAndProbeProducer( const ParameterSet & );
        
    private:
        //---FW methods
        void produce( Event &, const EventSetup & ) override;

        float getProbeEnergy(Ptr<DiPhotonCandidate>, TagAndProbeCandidate &);
        float getProbeSigEOverE(Ptr<DiPhotonCandidate>, TagAndProbeCandidate &);
        float getProbeSigE(Ptr<DiPhotonCandidate>, TagAndProbeCandidate &);

        //---data
        EDGetTokenT<View<DiPhotonCandidate> > diphotonsToken_;
        Handle<View<DiPhotonCandidate> > diphotonsHandle_;
        EDGetTokenT<vector<reco::GenParticle> > genPartToken_;
        Handle<vector<reco::GenParticle> > genPartHandle_;

        //---scale up
        EDGetTokenT<View<DiPhotonCandidate> > diphotonsScaleUpToken_;
        Handle<View<DiPhotonCandidate> > diphotonsScaleUpHandle_;
        EDGetTokenT<vector<reco::GenParticle> > genPartScaleUpToken_;
        Handle<vector<reco::GenParticle> > genPartScaleUpHandle_;

        //---scale down
        EDGetTokenT<View<DiPhotonCandidate> > diphotonsScaleDownToken_;
        Handle<View<DiPhotonCandidate> > diphotonsScaleDownHandle_;
        EDGetTokenT<vector<reco::GenParticle> > genPartScaleDownToken_;
        Handle<vector<reco::GenParticle> > genPartScaleDownHandle_;

        //---smearing up
        EDGetTokenT<View<DiPhotonCandidate> > diphotonsSmearingUpToken_;
        Handle<View<DiPhotonCandidate> > diphotonsSmearingUpHandle_;
        EDGetTokenT<vector<reco::GenParticle> > genPartSmearingUpToken_;
        Handle<vector<reco::GenParticle> > genPartSmearingUpHandle_;

        //---smearing down
        EDGetTokenT<View<DiPhotonCandidate> > diphotonsSmearingDownToken_;
        Handle<View<DiPhotonCandidate> > diphotonsSmearingDownHandle_;
        EDGetTokenT<vector<reco::GenParticle> > genPartSmearingDownToken_;
        Handle<vector<reco::GenParticle> > genPartSmearingDownHandle_;

        //---options
        int maxDiphotons_;
        selector_type tagSelector_;
        selector_type probeSelector_;

        //---ID selector
        ConsumesCollector cc_;
        CutBasedPhotonViewSelector idSelector_;

        //---output TnP collection
        unique_ptr<vector<TagAndProbeCandidate> > tnpColl_;
    };

    //---constructors
    //---dummy
    TagAndProbeProducer::TagAndProbeProducer( ):
        diphotonsToken_(),
        maxDiphotons_(-1),
        tagSelector_("1"),
        probeSelector_("1"),
        cc_( consumesCollector() ),
        idSelector_( ParameterSet(), cc_ )
    {}
    //---standard
    TagAndProbeProducer::TagAndProbeProducer( const ParameterSet & pSet):
        diphotonsToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "diphotonsSrc" ) ) ),
        genPartToken_( consumes<vector<reco::GenParticle> >( pSet.getParameter<InputTag> ( "genParticlesSrc" ) ) ),
        diphotonsScaleUpToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "diphotonsScaleUpSrc" ) ) ),
        diphotonsScaleDownToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "diphotonsScaleDownSrc" ) ) ),
        diphotonsSmearingUpToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "diphotonsSmearingUpSrc" ) ) ),
        diphotonsSmearingDownToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "diphotonsSmearingDownSrc" ) ) ),
        maxDiphotons_( pSet.getParameter<int> ( "maxDiphotons" ) ),        
        tagSelector_( pSet.getParameter<string> ( "tagSelection" ) ),
        probeSelector_( pSet.getParameter<string> ( "probeSelection" ) ),
        cc_( consumesCollector() ),
        idSelector_( pSet.getParameter<ParameterSet> ( "idSelection" ), cc_ )
    {
        produces<vector<TagAndProbeCandidate> >();
    }

    float TagAndProbeProducer::getProbeEnergy( Ptr<DiPhotonCandidate> variedDiphoton, TagAndProbeCandidate & cand )
    {
        auto lead = variedDiphoton->leadingView();
        auto sublead = variedDiphoton->subLeadingView();
        auto leadingPhoton = *lead->photon();
        auto subLeadingPhoton = *sublead->photon();
        
        if( reco::deltaR(*cand.getProbe(), leadingPhoton) < 0.3)
            {
                return leadingPhoton.energy();
            }
        if( reco::deltaR(*cand.getProbe(), subLeadingPhoton) < 0.3)
            {
                return subLeadingPhoton.energy();
            }

        return -999.;
    }

    float TagAndProbeProducer::getProbeSigEOverE( Ptr<DiPhotonCandidate> variedDiphoton, TagAndProbeCandidate & cand )
    {
        auto lead = variedDiphoton->leadingView();
        auto sublead = variedDiphoton->subLeadingView();
        auto leadingPhoton = *lead->photon();
        auto subLeadingPhoton = *sublead->photon();

        if( reco::deltaR(*cand.getProbe(), leadingPhoton) < 0.3)
            {
                return leadingPhoton.sigEOverE();
            }
        if( reco::deltaR(*cand.getProbe(), subLeadingPhoton) < 0.3)
            {
                return subLeadingPhoton.sigEOverE();
            }

        return -999.;
    }

    float TagAndProbeProducer::getProbeSigE( Ptr<DiPhotonCandidate> variedDiphoton, TagAndProbeCandidate & cand)
    {
        auto lead = variedDiphoton->leadingView();
        auto sublead = variedDiphoton->subLeadingView();
        auto leadingPhoton = *lead->photon();
        auto subLeadingPhoton = *sublead->photon();

        if( reco::deltaR(*cand.getProbe(), leadingPhoton) < 0.3)
            {
                return leadingPhoton.getCorrectedEnergyError(leadingPhoton.getCandidateP4type());
            }
        if( reco::deltaR(*cand.getProbe(), subLeadingPhoton) < 0.3)
            {
                return subLeadingPhoton.getCorrectedEnergyError(subLeadingPhoton.getCandidateP4type());
            }

        return -999.;
    }

    //---FW produce method
    void TagAndProbeProducer::produce( Event & event, const EventSetup & setup )
    {
        //---input 
        event.getByToken( diphotonsToken_, diphotonsHandle_ );
        auto diphotons = *diphotonsHandle_.product();

        View<DiPhotonCandidate> diphotonsScaleUp;
        View<DiPhotonCandidate> diphotonsScaleDown;
        View<DiPhotonCandidate> diphotonsSmearingUp;
        View<DiPhotonCandidate> diphotonsSmearingDown;
        
        vector<reco::GenParticle> genParticles;
        if( ! event.isRealData() )
        {
            event.getByToken( genPartToken_, genPartHandle_ );
            genParticles = *genPartHandle_.product();
            event.getByToken( diphotonsScaleUpToken_, diphotonsScaleUpHandle_ );
            event.getByToken( diphotonsScaleDownToken_, diphotonsScaleDownHandle_ );
            event.getByToken( diphotonsSmearingUpToken_, diphotonsSmearingUpHandle_ );
            event.getByToken( diphotonsSmearingDownToken_, diphotonsSmearingDownHandle_ );
            if( diphotonsScaleUpHandle_.isValid())
                {
                    diphotonsScaleUp = *diphotonsScaleUpHandle_.product();
                }
            else{
                    std::cout << "Handle for diphotonScaleUp not valid !!" << std::endl;
                }
            if( diphotonsScaleDownHandle_.isValid())
                {
                    diphotonsScaleDown = *diphotonsScaleDownHandle_.product();
                }
            else{
                    std::cout << "Handle for diphotonScaleDown not valid !!" << std::endl;
                }
            if( diphotonsSmearingUpHandle_.isValid())
                {
                    diphotonsSmearingUp = *diphotonsSmearingUpHandle_.product();
                }
            else{
                    std::cout << "Handle for diphotonSmearingUp not valid !!" << std::endl;
                }
            if( diphotonsSmearingDownHandle_.isValid())
                {
                    diphotonsSmearingDown = *diphotonsSmearingDownHandle_.product();
                }
            else{
                    std::cout << "Handle for diphotonSmearingDown not valid !!" << std::endl;
                }
        }

        //---output collection
        std::unique_ptr<vector<TagAndProbeCandidate> > tnpColl_( new vector<TagAndProbeCandidate> );

        //---search for gen electron and positron
        bool genEleFound = false;
        bool genPosFound = false;
        reco::GenParticle genEle;
        reco::GenParticle genPos;
        if( ! event.isRealData() ) {
            for( auto& gen : genParticles ) {
                int status = gen.status();
                int pdgid  = gen.pdgId();
                if ( abs(pdgid)==11 && status==23 ) {
                    if ( gen.mother(0) &&
                         gen.mother(0)->pdgId()==23) {
                        if (pdgid==11)  {
                            genPos = gen;
                            genPosFound = true;
                        }
                        if (pdgid==-11) {
                            genEle = gen;
                            genEleFound = true;
                        }
                    }
                }
            }
        }

        //---loop over diphoton candidates (max number specified from config)
        int nDP = maxDiphotons_ == -1 ? diphotons.size() : std::min(int(diphotons.size()), maxDiphotons_);
        for(int iDP=0; iDP<nDP; ++iDP)
        {
            auto diphoPtr = diphotons.ptrAt(iDP);
            auto lead = diphoPtr->leadingView();
            auto sublead = diphoPtr->subLeadingView();

            //---gen match (simulation only)
            float minDR=999;
            int leadGenMatch=0, subleadGenMatch=0;
            if(genPosFound && reco::deltaR(genPos, *lead->photon()) < 0.3)
            {
                minDR = reco::deltaR(genPos, *lead->photon());
                leadGenMatch = 1;
            }
            if(genEleFound && reco::deltaR(genEle, *lead->photon()) < 0.3 && reco::deltaR(genEle, *lead->photon()) < minDR)
                leadGenMatch = -1;
            minDR = 999;
            if(genPosFound && reco::deltaR(genPos, *lead->photon()) < 0.3)
            {
                minDR = reco::deltaR(genPos, *sublead->photon());
                subleadGenMatch = 1;
            }
            if(genEleFound && reco::deltaR(genEle, *sublead->photon()) < 0.3 && reco::deltaR(genEle, *sublead->photon()) < minDR)
                subleadGenMatch = -1;            
            
            //---check which photon is tag/probe (both combination are allowed)
            //   - TagAndProbeCandidate(diphoPtr, bool leadIsTag)
            //   - Compute id-selection
            //   - Single id variable passed/failed status is set by the selector as a UserInt
            if(tagSelector_(*lead->photon()) && probeSelector_(*sublead->photon()))
            {
                TagAndProbeCandidate cand(diphoPtr, true);
                auto idResults = idSelector_.computeSelections(*sublead->photon(), event);
                for(auto& sel : idResults)
                    cand.addUserInt("probe_pass_"+sel.first, sel.second);
                cand.addUserInt("tagGenMatch", leadGenMatch);
                cand.addUserInt("probeGenMatch", subleadGenMatch);
                cand.addUserFloat("probeSigE", cand.getProbe()->getCorrectedEnergyError(cand.getProbe()->getCandidateP4type()));
                if( diphotonsScaleUpHandle_.isValid() )
                    {
                        cand.addUserFloat("probeEnergyScaleUp01Sigma", getProbeEnergy(diphotonsScaleUp.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEOverEScaleUp01Sigma", getProbeSigEOverE(diphotonsScaleUp.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEScaleUp01Sigma", getProbeSigE(diphotonsScaleUp.ptrAt(iDP), cand));
                    }
                if( diphotonsScaleDownHandle_.isValid() )
                    {
                        cand.addUserFloat("probeEnergyScaleDown01Sigma", getProbeEnergy(diphotonsScaleDown.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEOverEScaleDown01Sigma", getProbeSigEOverE(diphotonsScaleDown.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEScaleDown01Sigma", getProbeSigE(diphotonsScaleDown.ptrAt(iDP), cand));
                    }
                if( diphotonsSmearingUpHandle_.isValid() )
                    {
                        cand.addUserFloat("probeEnergySmearingUp01Sigma", getProbeEnergy(diphotonsSmearingUp.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEOverESmearingUp01Sigma", getProbeSigEOverE(diphotonsSmearingUp.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigESmearingUp01Sigma", getProbeSigE(diphotonsSmearingUp.ptrAt(iDP), cand));
                    }
                if( diphotonsSmearingDownHandle_.isValid() )
                    {
                        cand.addUserFloat("probeEnergySmearingDown01Sigma", getProbeEnergy(diphotonsSmearingDown.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEOverESmearingDown01Sigma", getProbeSigEOverE(diphotonsSmearingDown.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigESmearingDown01Sigma", getProbeSigE(diphotonsSmearingDown.ptrAt(iDP), cand));
                    }
                tnpColl_->push_back(cand);
            }
            if(tagSelector_(*sublead->photon()) && probeSelector_(*lead->photon()))
            {
                TagAndProbeCandidate cand(diphoPtr, false);
                auto idResults = idSelector_.computeSelections(*lead->photon(), event);
                for(auto& sel : idResults)
                    cand.addUserInt("probe_pass_"+sel.first, sel.second);                
                cand.addUserInt("tagGenMatch", subleadGenMatch);
                cand.addUserInt("probeGenMatch", leadGenMatch);
                cand.addUserFloat("probeSigE", cand.getProbe()->getCorrectedEnergyError(cand.getProbe()->getCandidateP4type()));
                if( diphotonsScaleUpHandle_.isValid() )
                    {
                        cand.addUserFloat("probeEnergyScaleUp01Sigma", getProbeEnergy(diphotonsScaleUp.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEOverEScaleUp01Sigma", getProbeSigEOverE(diphotonsScaleUp.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEScaleUp01Sigma", getProbeSigE(diphotonsScaleUp.ptrAt(iDP), cand));
                    }
                if( diphotonsScaleDownHandle_.isValid() )
                    {
                        cand.addUserFloat("probeEnergyScaleDown01Sigma", getProbeEnergy(diphotonsScaleDown.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEOverEScaleDown01Sigma", getProbeSigEOverE(diphotonsScaleDown.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEScaleDown01Sigma", getProbeSigE(diphotonsScaleDown.ptrAt(iDP), cand));
                    }
                if( diphotonsSmearingUpHandle_.isValid() )
                    {
                        cand.addUserFloat("probeEnergySmearingUp01Sigma", getProbeEnergy(diphotonsSmearingUp.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEOverESmearingUp01Sigma", getProbeSigEOverE(diphotonsSmearingUp.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigESmearingUp01Sigma", getProbeSigE(diphotonsSmearingUp.ptrAt(iDP), cand));
                    }
                if( diphotonsSmearingDownHandle_.isValid() )
                    {
                        cand.addUserFloat("probeEnergySmearingDown01Sigma", getProbeEnergy(diphotonsSmearingDown.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigEOverESmearingDown01Sigma", getProbeSigEOverE(diphotonsSmearingDown.ptrAt(iDP), cand));
                        cand.addUserFloat("probeSigESmearingDown01Sigma", getProbeSigE(diphotonsSmearingDown.ptrAt(iDP), cand));
                    }
                tnpColl_->push_back(cand);
            }
        }
        //---put the colletion in the Event
        event.put( std::move(tnpColl_) );
    }
}

typedef flashgg::TagAndProbeProducer FlashggTagAndProbeProducer;
DEFINE_FWK_MODULE( FlashggTagAndProbeProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
