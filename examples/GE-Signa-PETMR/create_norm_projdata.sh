#! /bin/sh -e
# Example script to reconstruct GE-Signa PET/MR data with randoms, norm and scatter.
# Currently supposes you have randoms estimated already.
# Default filenames are for output of unlist_and_randoms.sh, and for the NEMA demo files
# Author: Kris Thielemans

# directory with some standard .par files
: ${pardir:=~/devel/STIR/examples/GE-Signa-PETMR}

if [ $# -ne 3 ]; then
  echo "Usage: `basename $0` output_filename_prefix RDF_norm_filename template_filename"
  echo "This creates a projdata with the normalisation factors"
  exit 1
fi

norm_sino_prefix=$1
# note: variable name is used in correct_projdata_for_norm.par
RDFNORM=$2
export RDFNORM
template_filename=$3

if [ -r ${norm_sino_prefix}.hs ]; then
  echo "Reusing existing ${norm_sino_prefix}.hs"
else
  echo "Creating ${norm_sino_prefix}.hs"
  OUTPUT=${norm_sino_prefix} INPUT=${template_filename} correct_projdata ${pardir}/correct_projdata_for_norm.par > ${norm_sino_prefix}.log 2>&1
fi
