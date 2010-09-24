/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageAdaptor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageAdaptor_h
#define __itkImageAdaptor_h

#include "itkImage.h"
#include "itkRegularImageAdaptor.h"

#include "itkDefaultPixelAccessorFunctor.h"

namespace itk
{

/**
 * \class ImageAdaptor
 * \brief Give access to partial aspects of voxels from an Image
 *
 * ImageAdaptors are templated over the ImageType and over a functor
 * that will specify what part of the pixel can be accessed
 *
 * The basic aspects of this class are the types it defines.
 *
 * Image adaptors can be used as intermediate classes that allow
 * the sending of an image to a filter, specifying what part of the
 * image pixels the filter will act on.
 *
 * The TAccessor class should implement the Get and Set methods
 * These two will specify how data can be put
 * and get from parts of each pixel. It should define the types
 * ExternalType and InternalType too.
 *
 * \ingroup ImageAdaptors
 *
 */

/** Default template. Should give an error for images for which no
specialization is defined. */
template< class TImage, class TAccessor >
class ITK_EXPORT ImageAdaptor : public ImageBase< ::itk::GetImageDimension< TImage >::ImageDimension >
{
};
} // end namespace itk


/** Include header files for specializations, one for each image type. */
#include "itkImageAdaptorImageSpecialization.h"
#include "itkImageAdaptorVectorImageSpecialization.h"

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageAdaptor.txx"
#endif

#endif
