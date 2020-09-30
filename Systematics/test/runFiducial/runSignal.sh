#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_signal_IA_16/
fggRunJobs.py --load sig_jobs_2016_syst.json -d ${outDir}_signal_IA_16 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=True dumpGenWeight=True doJets=True doBJetsAndMET=True ignoreNegR9=True --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True doGranularJEC=True puTarget=`cat lumi/PU2016.txt`  maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_signal_IA_16/ -q tomorrow -n 200 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_signal_IA_17/
fggRunJobs.py --load sig_jobs_2017_syst.json -d ${outDir}_signal_IA_17 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=True dumpGenWeight=True doJets=True doBJetsAndMET=True ignoreNegR9=True --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True doGranularJEC=True recalculatePDFWeights=True puTarget=`cat lumi/PU2017.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_signal_IA_17/ -q tomorrow -n 200 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_signal_IA_18/
fggRunJobs.py --load sig_jobs_2018_syst.json -d ${outDir}_signal_IA_18 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=True dumpGenWeight=True doJets=True doBJetsAndMET=True ignoreNegR9=True --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True doGranularJEC=True recalculatePDFWeights=True puTarget=`cat lumi/PU2018.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_signal_IA_18/ -q tomorrow -n 200 -H --no-copy-proxy &

wait
