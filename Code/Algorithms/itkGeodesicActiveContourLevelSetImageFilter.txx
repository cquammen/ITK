/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGeodesicActiveContourLevelSetImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGeodesicActiveContourLevelSetImageFilter_txx_
#define __itkGeodesicActiveContourLevelSetImageFilter_txx_

#include "itkGeodesicActiveContourLevelSetImageFilter.h"

namespace itk {


template <class TInputImage, class TFeatureImage, class TOutputType>
GeodesicActiveContourLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::GeodesicActiveContourLevelSetImageFilter()
{
  /* Instantiate a geodesic active contour function and set it as the segmentation function. */
  m_GeodesicActiveContourFunction = GeodesicActiveContourFunctionType::New();

  this->SetSegmentationFunction( m_GeodesicActiveContourFunction );

  /* Use negative features by default. */
  this->SetUseNegativeFeatures( true );

  /* Turn off interpolation. */
  this->InterpolateSurfaceLocationOff();
}
 
template <class TInputImage, class TFeatureImage, class TOutputType>
void
GeodesicActiveContourLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "GeodesicActiveContourFunction: " << m_GeodesicActiveContourFunction.GetPointer();
}


}// end namespace itk




#endif
