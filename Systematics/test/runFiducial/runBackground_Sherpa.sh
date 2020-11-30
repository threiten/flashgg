#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_diphoton_sherpa_16/
fggRunJobs.py --load diphoton_jobs_2016_sherpa.json -d ${outDir}_diphoton_sherpa_16 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False ignoreNegR9=True puTarget=`cat lumi/PU2016.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_diphoton_sherpa_16/ -q workday -n 800 --no-copy-proxy --make-light-tarball &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_diphoton_sherpa_17/
fggRunJobs.py --load diphoton_jobs_2017_sherpa.json -d ${outDir}_diphoton_sherpa_17 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False ignoreNegR9=True puTarget=`cat lumi/PU2017.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_diphoton_sherpa_17/ -q workday -n 800 --no-copy-proxy --make-light-tarball &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_diphoton_sherpa_18/
fggRunJobs.py --load diphoton_jobs_2018_sherpa.json -d ${outDir}_diphoton_sherpa_18 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False ignoreNegR9=True puTarget=`cat lumi/PU2018.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_diphoton_sherpa_18/ -q workday -n 800 --no-copy-proxy --make-light-tarball &

wait
