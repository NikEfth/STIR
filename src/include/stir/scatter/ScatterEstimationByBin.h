#ifndef __stir_scatter_ScatterEstimationByBin_H__
#define __stir_scatter_ScatterEstimationByBin_H__

/*
    Copyright (C) 2004 - 2009 Hammersmith Imanet Ltd
    Copyright (C) 2013 University College London
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
  \ingroup scatter
  \brief Definition of class stir::ScatterEstimationByBin.
  
  \author Nikos Efthimiou
  \author Kris Thielemans
*/

#include "stir/shared_ptr.h"
#include "stir/DiscretisedDensity.h"
#include "stir/ProjData.h"
#include "stir/ParsingObject.h"
#include "stir/numerics/BSplines.h"
#include <vector>
#include "stir/CartesianCoordinate3D.h"
#include "stir/recon_buildblock/Reconstruction.h"

#include "stir/scatter/ScatterSimulation.h"
#include "stir/recon_buildblock/ChainedBinNormalisation.h"

START_NAMESPACE_STIR

class Succeeded;
class ProjDataInfoCylindricalNoArcCorr;
class ViewSegmentNumbers;
class BinNormalisation;

/*!
  \ingroup scatter
  \brief Estimate the scatter probability using a model-based approach
*/

class ScatterEstimationByBin : public ParsingObject
{
public:
    //! upsample coarse scatter estimate and fit it to tails of the emission data
    /*! Current procedure:
    1. interpolate segment 0 of \a scatter_proj_data to size of segment 0 of \a emission_proj_data
    2. inverseSSRB to create oblique segments
    3. find scale factors with get_scale_factors_per_sinogram()
    4. apply thresholds
    5. filter scale-factors in axial direction (independently for every segment)
    6. apply scale factors using scale_sinograms()
  */
    static void
    upsample_and_fit_scatter_estimate(ProjData& scaled_scatter_proj_data,
                                      const  ProjData& emission_proj_data,
                                      const ProjData& scatter_proj_data,
                                      const BinNormalisation& scatter_normalisation,
                                      const ProjData& weights_proj_data,
                                      const float min_scale_factor,
                                      const float max_scale_factor,
                                      const unsigned half_filter_width,
                                      BSpline::BSplineType spline_type,
                                      const bool remove_interleaving = true);


    //! Default constructor (calls set_defaults())
    ScatterEstimationByBin();

    virtual Succeeded process_data();

    virtual Succeeded
    reconstruct_iterative(int,
             shared_ptr<DiscretisedDensity<3, float> >&);

    virtual Succeeded
    reconstruct_analytic();

    //!
    //! \brief set_image_from_file
    //! \param filename
    //! \details This function loads image files from the disk
    inline shared_ptr<DiscretisedDensity<3,float> >
    get_image_from_file(const std::string& filename);



    //! set the image that determines where the scatter points are
    /*! Also calls sample_scatter_points()
   \warning Uses attenuation_threshold member variable
  */


    //void set_output_proj_data_sptr(const shared_ptr<ProjData>& new_sptr);


    /** @}*/

    // TODO write_log can't be const because parameter_info isn't const
    virtual void
    write_log(const double simulation_time,
              const float total_scatter);


protected:
    void set_defaults();
    void initialise_keymap();

    //!
    //! \brief post_processing
    //! \return
    //! \details This function initialised only the reconstruction method
    //! and then calls either set_up_iterative() or set_up_analytic() because
    //! the two reconstruction methods are substancially different.
    bool post_processing();

    //!
    //! \brief set_up_iterative
    //! \return
    bool set_up_iterative();

    //!
    //! \brief set_up_initialise_analytic
    //! \return
    //!
    bool set_up_analytic();

    //!
    //! \brief proj_data_info_2d_ptr
    //! \details The projection data info for the 2D data - after SSRB
    ProjDataInfoCylindricalNoArcCorr * proj_data_info_2d_ptr;

    //!
    //! \brief proj_data_info_2d_sptr
    //! \details shared pointer to projection data info for the 2D data - after SSRB
    shared_ptr < ProjDataInfo > proj_data_info_2d_sptr;

    /**
   *
   * \name Variables related to the initial activity image and the measured emission data.
   * @{
   */

    //!
    //! \brief recompute_initial_estimate
    //! \details If set then the initial activity estimate will be recomputed
    //! and stored if a name is provided.
    bool recompute_initial_activity_image;

    //!
    //! \brief initial_activity_image_filename
    //! \details Filename of the initial activity image.
    std::string initial_activity_image_filename;

    //!
    //! \brief reconstruction_method_sptr
    //! \details The reconsturction which is going to be used for the scatter simulation
    //! and the intial activity image (if recompute set).
    shared_ptr < Reconstruction < DiscretisedDensity < 3, float > > >
    reconstruction_template_sptr;

    //!
    //! \brief reconstruction_template_par_filename
    //! \details The filename for the parameters file of the reconstruction method.
    //! \warning Refere to the samples for a proper example.
    std::string reconstruction_template_par_filename;

    //!
    //! \brief activity_image_sptr
    //! \details Initially with is the reconstructed activity image, but during the scatter
    //! estimation it with actually hold the iterative estimates.
    //! Therefore the nane might change later.
    shared_ptr<DiscretisedDensity < 3, float > > activity_image_sptr;

    //!
    //! \brief input_projdata_filename
    //! \details Filename of the measured emission data.
    std::string input_projdata_filename;

    //!
    //! \brief input_data
    //! \details This memnbers holds the measured emission data.
    //    shared_ptr<ProjData> input_projdata_sptr;

    shared_ptr<ProjData> input_projdata_2d_sptr;
    /** }@*/

    /**
   *
   * \name Variables related to the attenuation image and coefficients
   * @{
   */

    //!
    //! \brief atten_image_filename
    //! \details This is the image file name with the anatomic information.
    //! \warning Previously named density_image.
    std::string atten_image_filename;

    //!
    //! \brief recompute_atten_coeff
    //! \details If set to 1 the attenuation coefficients are going to
    //! be recalculated.
    bool recompute_atten_coeff;

    //!
    //! \brief atten_coeff_filename
    //! \details The file name for the attenuation coefficients.
    //! If they are to be recalculated they will be stored here, if set.
    std::string atten_coeff_filename;

    //!
    //! \brief atten_image_sptr
    //!
    shared_ptr<DiscretisedDensity < 3, float > > atten_image_sptr;

    //!
    //! \brief atten_projdata_2d_sptr
    //!
    shared_ptr<ProjData> atten_projdata_2d_sptr;

    /** @}*/

    //!
    //! \brief _multiplicative_data
    //! \details The multiplicative component of the reconsrtuction process.
    shared_ptr<BinNormalisation> _multiplicative_data;

    /**
    * \name Varianbles realted to the background proj data
    * @{
    *
    */

    //!
    //! \brief back_projdata_filename
    //!
    std::string back_projdata_filename;

    //!
    //! \brief back_projdata_sptr
    //! \details In this context the background data are only
    //! randoms.
    //  shared_ptr<ProjData> back_projdata_sptr;

    //!
    //! \brief background_projdata_2d_sptr
    //! \details Background projection data after SSRB
    shared_ptr<ProjData> back_projdata_2d_sptr;

    /** @}*/

    /**
   *
   *  \name Variables related to normalisation factors
   *  @{
   */

    //!
    //! \brief normalisation_coeffs_filename
    //! \details File with normalisation factors
    std::string normalisation_projdata_filename;

    //!
    //! \brief normalisation_factors_2d_sptr
    //! \details normalisation projdata after SSRB
    shared_ptr<ProjData> norm_projdata_2d_sptr;
    /** @}*/


    /**
   * \name Members for the mask image.
   */

    //!
    //! \brief mask_image_filename
    //!
    std::string mask_image_filename;

    //!
    //! \brief mask_postfilter_filename
    //!
    std::string mask_postfilter_filename;

    //!
    //! \brief recompute_mask_image
    //! \details recompute or load the mask image.
    bool recompute_mask_image;

    //!
    //! \brief mask_image_sptr
    //!
    shared_ptr < DiscretisedDensity < 3, float >  > mask_image_sptr;

    //!
    //! \brief mask_max_threshold
    //!
    float mask_max_threshold;

    //!
    //! \brief mask_add_scalar
    //!
    float mask_add_scalar;

    //!
    //! \brief mask_min_threshold
    //!
    float mask_min_threshold;

    //!
    //! \brief mask_times_scalar
    //!
    float mask_times_scalar;

    //!
    //! \brief recompute_mask_projdata
    //! \details If set the mask projdata will be recomputed
    bool recompute_mask_projdata;

    //!
    //! \brief mask_projdata_filename
    //!
    std::string mask_projdata_filename;

    //!
    //! \brief tail_mask_filename
    //! \details Paraameter file for the tail fitting.
    std::string tail_mask_par_filename;

    //!
    //! \brief mask_projdata_sptr
    //!
    shared_ptr<ProjData> mask_projdata_sptr;

    /** @}*/

    std::string output_projdata_filename;
    shared_ptr<ProjData> output_projdata_sptr;

private:

    //!
    //! \brief appy_mask
    //! \details Mask a DiscretisedDensity
    void
    apply_mask_in_place(shared_ptr<DiscretisedDensity<3, float> >&);

    //!
    //! \brief num_scatter_iterations
    //! \details The number of iterations the scatter estimation will perform.
    //! Default = 5.
    int num_scatter_iterations;

    //!
    //! \brief scatter_sim_filename
    //!
    std::string scatter_sim_par_filename;

    //!
    //! \brief _scatter_simulation
    //! \details Class which will implement the scatter simulation.
    shared_ptr < ScatterSimulation > scatter_simulation_sptr;

    //!
    //! \brief iterative_method
    //! \details Used for convinience. It is initialised on post_processing.
    bool iterative_method;
};


END_NAMESPACE_STIR

#include "stir/scatter/ScatterEstimationByBin.inl"

#endif
