/*
    Copyright (C) 2019 University of Hull
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
/*!
  \file
  \ingroup listmode
  \brief Declaration of class stir::CListModeDataSimSET

  \author Nikos Efthimiou
*/

#ifndef __stir_listmode_CListModeDataSimSET_H__
#define __stir_listmode_CListModeDataSimSET_H__

#include "stir/listmode/CListModeData.h"
#include "stir/listmode/CListRecordSimSET.h"
#include "stir/IO/InputStreamFromSimSET.h"
#include "stir/KeyParser.h"

extern "C" {
//#include "SystemDependent.h"

//#include "LbTypes.h"
//#include "LbError.h"
//#include "LbDebug.h"
//#include "LbEnvironment.h"
//#include "LbFile.h"
//#include "LbMemory.h"
//#include "LbParamFile.h"
//#include "LbInterface.h"
//#include "LbHeader.h"

//#include "Photon.h"
//#include "PhgParams.h"
//#include "ColTypes.h"
//#include "ColParams.h"
//#include "DetTypes.h"
//#include "DetParams.h"
//#include "CylPos.h"
//#include "PhgMath.h"
//#include "PhoHFile.h"
//#include "PhgHdr.h"
//#include "ProdTbl.h"
//#include "PhoTrk.h"
//#include "SubObj.h"
//#include "EmisList.h"
//#include "Collimator.h"
//#include "Detector.h"
//#include "phg.h"
//#include "PhgBin.h"
}

START_NAMESPACE_STIR


class CListModeDataSimSET : public CListModeData
{
public:
    //! construct from the filename of the Interfile header
    CListModeDataSimSET(const std::string& _history_filename);

    //! returns the header filename
    virtual std::string
    get_name() const;
    //! Set private members default values;
    void set_defaults();

    virtual
    shared_ptr <CListRecord> get_empty_record_sptr() const;

    virtual
    Succeeded get_next_record(CListRecord& record) const;

    virtual ~CListModeDataSimSET();

    virtual
    Succeeded reset();

    virtual
    SavedPosition save_get_position();

    virtual
    Succeeded set_get_position(const SavedPosition&);

    virtual
    bool has_delayeds() const { return true; }

    virtual inline
    unsigned long int
    get_total_number_of_events() const ;

private:
    //! Check if the hroot contains a full scanner description.
    Succeeded check_scanner_definition(std::string& ret);
    //! Check if the scanner_sptr matches the geometry in root_file_sptr.
    Succeeded check_scanner_match_geometry(std::string& ret, const shared_ptr<Scanner>& scanner_sptr);

    //! Pointer to the listmode data
    shared_ptr<InputStreamFromSimSET > history_file_sptr;
    //! Name of the PHG file.
    const std::string phg_filename;
    //! Name of history file as string.
    std::string phgrdhstHistName_str;
    //! Name of history file as char.
    char *phgrdhstHistName;
    //! Name of history parameters file as string.
    std::string phgrdhstHistParamsName_str;
    //! Name of history parameters file.
    char *phgrdhstHistParamsName;
    //! The name of the originating scanner
    std::string originating_system;

    Succeeded open_lm_file();
};


END_NAMESPACE_STIR

#endif
