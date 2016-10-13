#! /bin/sh
# Shell script for automatic running of the tests
# see README.txt
#
#  Copyright (C) 2016, UCL
#  This file is part of STIR.
#
#  This file is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.

#  This file is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  See STIR/LICENSE.txt for details
#      
# Author Nikos Efthimiou

# Scripts should exit with error code when a test fails:
if [ -n "$TRAVIS" ]; then
    # The code runs inside Travis
    set -e
fi

echo This script should work with STIR version ">"3.0. If you have
echo a later version, you might have to update your test pack.
echo Please check the web site.
echo

#
# Parse option arguments (--)
# Note that the -- is required to suppress interpretation of $1 as options 
# to expr
#
while test `expr -- "$1" : "--.*"` -gt 0
do

if test "$1" = "--help"
  then
    echo "Usage: run_root_GATE.sh [install_dir]"
    echo "See README.txt for more info."
    exit 1
  else
    echo Warning: Unknown option "$1"
    echo rerun with --help for more info.
    exit 1
  fi

  shift 1

done 

if ! ${INSTALL_DIR}lm_to_projdata --input-formats 2>&1 | grep ROOT; then
echo GATE support has not been installed in this system. Aborting.
exit 1;
fi



echo Executing tests on ROOT files generated by GATE simulations, with cylindrical PET scanners





# first delete any files remaining from a previous run
rm -f my_*v my_*s my_*S

INSTALL_DIR=$1
ThereWereErrors=0
export INPUT_ROOT_FILE=test_PET_GATE.root
export INPUT=root_header.hroot
export TEMPLATE=template_for_ROOT_scanner.hs

echo ------------- Converting ROOT files to ProjData file -------------
echo Making ProjData for all events
echo Running ${INSTALL_DIR}lm_to_projdata for all events

export OUT_PROJDATA_FILE=my_proj_from_lm_all_events
export EXCLUDE_SCATTERED=0
export EXCLUDE_RANDOM=0

all_events=$(${INSTALL_DIR}lm_to_projdata ./lm_to_projdata_from_ROOT.par 2>&1 | grep "Number of prompts stored in this time period" | grep -o -E '[0-9]+')

echo Number of prompts stored in this time period: ${all_events}
echo
echo Reading all values from ROOT file

all_root_num=$(root -l ${INPUT_ROOT_FILE} << EOF | grep "Number of prompts stored in this time period" | grep -o -E '[0-9]+'
Coincidences->Draw(">>eventlist","","goff");
Int_t N = eventlist->GetN();
cout<<endl<<"Number of prompts stored in this time period:"<< N<<endl;
EOF)

echo All events in ROOT file : ${all_root_num}
if [ "$all_events" != "$all_root_num" ];then
ThereWereErrors=1
fi
echo
echo Making ProjData for true events only
echo Running ${INSTALL_DIR}lm_to_projdata *ONLY* for true events

export OUT_PROJDATA_FILE=my_proj_from_lm_true_events
export EXCLUDE_SCATTERED=1
export EXCLUDE_RANDOM=1

true_events=$(${INSTALL_DIR}lm_to_projdata ./lm_to_projdata_from_ROOT.par 2>&1 | grep "Number of prompts stored in this time period" | grep -o -E '[0-9]+')

echo Number of prompts stored in this time period: ${true_events}
echo
echo Reading true values from ROOT file ...

true_root_num=$(root -l ${INPUT_ROOT_FILE} << EOF | grep "Number of trues stored in this time period" | grep -o -E '[0-9]+'
Coincidences->Draw(">>eventlist","eventID1 == eventID2 && comptonPhantom1 == 0 && comptonPhantom2 == 0","goff");
Int_t N = eventlist->GetN();
cout<<endl<<"Number of trues stored in this time period:"<< N<<endl;
EOF)

echo True events in ROOT file : ${true_root_num}

if [ "$true_events" != "$true_root_num" ]; then
ThereWereErrors=1
fi

echo
echo '--------------- End of tests -------------'
echo
if test ${ThereWereErrors} = 1  ; 
then
echo "Check what went wrong. The *.log files might help you."
else
echo "Everything seems to be fine !"
echo 'You could remove all generated files using "rm -f my_* *.log"'
fi



