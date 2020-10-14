#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi


mkdir -p /eos/cms/store/group/phys_higgs/cmshgg/threiten/TnPProduction/${outdir}_mc_UL17
fggRunJobs.py --load jobs_UL17_MC.json -x cmsRun tagAndProbe_test.py -d ${outDir}_mc_UL17 maxEvents=-1 useAAA=1 doPhoIdInputsCorrections=1 -q tomorrow --make-light-tarball --stage-to=/eos/cms/store/group/phys_higgs/cmshgg/threiten/TnPProduction/${outdir}_mc_UL17/ -n 500 --no-copy-proxy -H &

mkdir -p /eos/cms/store/group/phys_higgs/cmshgg/threiten/TnPProduction/${outdir}_data_UL17
fggRunJobs.py --load jobs_UL17_data.json -x cmsRun tagAndProbe_test.py -d ${outDir}_data_UL17 maxEvents=-1 useAAA=1 doPhoIdInputsCorrections=0 -q tomorrow --make-light-tarball --stage-to=/eos/cms/store/group/phys_higgs/cmshgg/threiten/TnPProduction/${outdir}_data_UL17 -n 500 --no-copy-proxy -H &
