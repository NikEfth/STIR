//
//
/*!
  \file
  \ingroup IO SimSET
  \brief Declaration of class stir::InputStreamFromSimSET

  \author Nikos Efthimiou

*/
/*
    Copyright (C) 2019, University of Hull
    This file is part of STIR.

    This file is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This file is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    See STIR/LICENSE.txt for details
*/

#ifndef __stir_IO_InputStreamFromSimSET_H__
#define __stir_IO_InputStreamFromSimSET_H__

#include "stir/shared_ptr.h"
#include "stir/Succeeded.h"
#include "stir/listmode/CListRecordSimSET.h"
#include "stir/RegisteredParsingObject.h"

#include <iostream>
#include <string>
#include <vector>

extern "C" {
#include <bin.phg.h>
}

START_NAMESPACE_STIR


/*! 
 * \brief A helper class to read data from a SimSET history file
 * \author Nikos Efthimiou
 *
 * \ingroup IO SimSET
 *
 * \details This class is responsible for reading photons from a PHG history
 * file generated by SimSET.
 *
 * Large parts of the functions are adaptations from functions already in SimSET.
 *
 * Random and Scattered can be excluded as instructed in the bin params file, see SimSET instructions.
 *
 * \warning Support for old photons and old decays has not been implemented. Probably it will not.
 * \warning SiSET supports multiple history files. In STIR this is not supported, yet.
 * \warning Custom history files are not supported. Some code exists in the set_up_custom_hist_file()
 * function. But, currently I am not sure how these files are organised.
*/
class InputStreamFromSimSET
{
public:

    static const char * const registered_name;

    typedef std::vector<LbUsFourByte>::size_type SavedPosition;
    //! Constructor taking a stream
    /*! Data will be assumed to start at the current position reported by seekg().
      If reset() is used, it will go back to this starting position.*/
    InputStreamFromSimSET();

    ~InputStreamFromSimSET();
    //! gives method information
    std::string method_info() const;
    //! Must be called before calling for the first event.
    //! The function will try to set up first assuming that the history file
    //! is standard. Upon failure it will try with a custom set up.
    Succeeded set_up(const std::string historyFileName,
                     PHG_BinParamsTy * _binParams);
    //! Check if the file is a standard history file. The code
    //! was adapted from bin.phg.c::phgrdhstStandard(char *argv[]).
    Succeeded set_up_standard_hist_file();
    //! Check if the file is a custom history file. The code
    //! was adapted from bin.phg.c::phgrdhstCustom(char *argv[]).
    Succeeded set_up_custom_hist_file();
    //! Get the next available record, depending on conditions.
    inline
    Succeeded get_next_record(CListRecordSimSET& record);

    //! go back to starting position
    inline
    Succeeded reset();

    //! save current "get" position in an internal array
    inline
    SavedPosition save_get_position();
    //! set current "get" position to previously saved value
    inline
    Succeeded set_get_position(const SavedPosition&);
    //! Function that enables the user to store the saved get_positions
    inline
    std::vector<PHG_Decay> get_saved_get_positions() const;
    //! Function that sets the saved get_positions
    inline
    void set_saved_get_positions(const std::vector<PHG_Decay>& );

    inline unsigned long get_total_number_of_events() const;

protected:

    void set_defaults();

private:
    //! The history file we are going to process
    FILE *historyFile;
    //! Hook to header
    LbHdrHkTy headerHk;
    //! Type of current event.
    //! \warning This might vary in custom history files
    EventTy eventType;
    //! Current file index
    LbUsFourByte curFileIndex;
    //! Number of photons processed
    LbUsFourByte numPhotonsProcessed;
    //! Number of decays processed
    LbUsFourByte numDecaysProcessed;
    //! First event in the history file.
    LbUsFourByte startFileIndex;
    //! Total number of photons in the history file.
    LbUsFourByte numPhotons;
    //! The first detected photon
    PHG_DetectedPhoton	cur_detectedPhotonBlue;
    //! The second detected photon
    PHG_DetectedPhoton	cur_detectedPhotonPink;
    //! SimSET cylindrical PET scanner is centered to the z axis.
    //! This variable will be added to move the z position on the
    //! possitive side, as is STIR's convention.
    double min_axial_position;
    //! The next position in the decays.
    PHG_Decay nextDecay;
    //! The first decay, used to reset the file.
    PHG_Decay firstDecay;
    //! Binning parameters. They are used to get energy window,
    //! randoms and scattered exclusion/inclusion.
    PHG_BinParamsTy *binParams;

    std::vector<PHG_Decay> saved_get_positions;
    //! Number of scatters for current blue photon
    LbUsFourByte blueScatters;
    //! Number of scatters for current pink photon
    LbUsFourByte pinkScatters;
    //! set true if the scatter/random param is 3,5,8 or 10
    Boolean	ignoreMaxScatters;
    //! set true if the scatter/random param is 4,5,9 or 10
    Boolean	ignoreMinScatters;
    //! Hook to history file information. Used in the case of custom
    //! history file.
    PhoHFileHkTy histHk;

};

END_NAMESPACE_STIR

#include "stir/IO/InputStreamFromSimSET.inl"

#endif
