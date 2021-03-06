/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkReconstructionByDilationImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkReconstructionByDilationImageFilter_h
#define __itkReconstructionByDilationImageFilter_h

#include "itkReconstructionImageFilter.h"

#include "itkNumericTraits.h"

namespace itk
{
/** \class ReconstructionByDilationImageFilter
 * \brief grayscale reconstruction by dilation of an image
 *
 * Reconstruction by dilation operates on a "marker" image and a "mask"
 * image, and is defined as the dilation of the marker image with
 * respect to the mask image iterated until stability.
 *
 * The marker image must be less than or equal to the mask image
 * (on a pixel by pixel basis).
 *
 * Geodesic morphology is described in Chapter 6.2 of Pierre Soille's
 * book "Morphological Image Analysis: Principles and Applications",
 * Second Edition, Springer, 2003.
 *
 * Algorithm implemented in this filter is based on algorithm described
 * by Kevin Robinson and  Paul F. Whelan in "Efficient Morphological
 * Reconstruction: A Downhill Filter", Pattern Recognition Letters, Volume
 * 25, Issue 15, November 2004, Pages 1759-1767.
 *
 * The algorithm, a description of the transform and some applications
 * can be found in "Morphological Grayscale Reconstruction in Image Analysis:
 * Applications and Efficient Algorithms", Luc Vincent, IEEE Transactions on
 * image processing, Vol. 2, April 1993.
 *
 * \author Richard Beare. Department of Medicine, Monash University,
 * Melbourne, Australia.
 *
 * \sa MorphologyImageFilter, GrayscaleDilateImageFilter, GrayscaleFunctionDilateImageFilter, BinaryDilateImageFilter, ReconstructionByErosionImageFilter,
OpeningByReconstructionImageFilter, ClosingByReconstructionImageFilter, ReconstructionImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 * \ingroup MathematicalMorphologyImageFilters
 */

template< class TInputImage, class TOutputImage >
class ITK_EXPORT ReconstructionByDilationImageFilter:
  public ReconstructionImageFilter< TInputImage, TOutputImage, std::greater< typename TOutputImage::PixelType > >
{
public:
  typedef ReconstructionByDilationImageFilter Self;
  typedef ReconstructionImageFilter<
    TInputImage, TOutputImage, std::greater< typename TOutputImage::PixelType > >
  Superclass;

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                            MarkerImageType;
  typedef typename MarkerImageType::Pointer      MarkerImagePointer;
  typedef typename MarkerImageType::ConstPointer MarkerImageConstPointer;
  typedef typename MarkerImageType::RegionType   MarkerImageRegionType;
  typedef typename MarkerImageType::PixelType    MarkerImagePixelType;
  typedef TInputImage                            MaskImageType;
  typedef typename MaskImageType::Pointer        MaskImagePointer;
  typedef typename MaskImageType::ConstPointer   MaskImageConstPointer;
  typedef typename MaskImageType::RegionType     MaskImageRegionType;
  typedef typename MaskImageType::PixelType      MaskImagePixelType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(MarkerImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(MaskImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ReconstructionByDilationImageFilter,
               ReconstructionImageFilter);
protected:
  ReconstructionByDilationImageFilter()
  {
    this->m_MarkerValue = NumericTraits< ITK_TYPENAME TOutputImage::PixelType >::NonpositiveMin();
  }

  virtual ~ReconstructionByDilationImageFilter() {}
private:
  //purposely not implemented
  ReconstructionByDilationImageFilter(const Self &);
  void operator=(const Self &);
};
// end ReconstructionByDilationImageFilter
}

#endif
