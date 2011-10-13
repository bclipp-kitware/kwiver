/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PROCESSES_HELPER_IMAGE_FORMAT_H
#define VISTK_PROCESSES_HELPER_IMAGE_FORMAT_H

#include "pixtypes.h"

#include <vistk/pipeline/process.h>

/**
 * \file format.h
 *
 * \brief Types and functions to help manage image formats in the pipeline.
 */

namespace vistk
{

/**
 * \brief Port types for images with different parameters.
 *
 * \param pixtype The type for pixels.
 * \param grayscale True if the images are grayscale, false otherwise.
 * \param alpha True if the images have an alpha channel, false otherwise.
 *
 * \returns The port type for images of the given information.
 */
process::port_type_t port_type_for_pixtype(pixtype_t const& pixtype, bool grayscale, bool alpha = false);

}

#endif // VISTK_PROCESSES_HELPER_IMAGE_FORMAT_H
