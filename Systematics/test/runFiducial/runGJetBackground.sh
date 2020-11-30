#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_GJets_16/
fggRunJobs.py --load GJets_jobs_2016.json -d ${outDir}_GJets_16 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False ignoreNegR9=True puTarget=`cat lumi/PU2016.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_GJets_16/ -q workday -n 800 -H --no-copy-proxy --make-light-tarball &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_GJets_17/
fggRunJobs.py --load GJets_jobs_2017.json -d ${outDir}_GJets_17 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False ignoreNegR9=True puTarget=`cat lumi/PU2017.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_GJets_17/ -q workday -n 800 -H --no-copy-proxy --make-light-tarball &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_GJets_18/
fggRunJobs.py --load GJets_jobs_2018.json -d ${outDir}_GJets_18 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False ignoreNegR9=True puTarget=`cat lumi/PU2018.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_GJets_18/ -q workday -n 800 -H --no-copy-proxy --make-light-tarball &

wait
