/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageToVectorImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToVectorImageFilter_h
#define __itkImageToVectorImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkVectorContainer.h"
#include "itkVectorImage.h"

namespace itk
{
/** \class ImageToVectorImageFilter
 * \brief This class takes as input 'n' itk::Image's and composes them into
 * a single itk::VectorImage.
 *
 * \par Inputs and Usage
 * \code
 *    filter->SetInput( 0, image0 );
 *    filter->SetInput( 1, image1 );
 *    ...
 *    filter->Update();
 *    itk::VectorImage< PixelType, dimension >::Pointer = filter->GetOutput();
 * \endcode
 * All input images are expected to have the same template parameters and have
 * the same size and origin.
 *
 * \sa VectorImage
 */

template< class TInputImage >
class ITK_EXPORT ImageToVectorImageFilter:
  public ImageToImageFilter< TInputImage,
                             VectorImage< ITK_TYPENAME TInputImage::InternalPixelType,
                                          ::itk::GetImageDimension< TInputImage >::ImageDimension > >
{
public:

  typedef ImageToVectorImageFilter   Self;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(ImageToVectorImageFilter, ImageToImageFilter);

  itkStaticConstMacro(Dimension, unsigned int, TInputImage::ImageDimension);

  typedef typename TInputImage::InternalPixelType PixelType;
  typedef VectorImage< PixelType,
                       itkGetStaticConstMacro(Dimension) >   OutputImageType;
  typedef ImageToImageFilter< TInputImage,
                              OutputImageType >     Superclass;

  typedef typename Superclass::InputImageType InputImageType;

  typedef typename Superclass::InputImageRegionType RegionType;

  virtual void SetNthInput(unsigned int idx, const InputImageType *inputImage)
  { this->SetInput(idx, inputImage); }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  // Check if the pixeltype is a scalar, (native pixel type).
  /** End concept checking */
#endif
protected:
  ImageToVectorImageFilter();

  virtual void GenerateOutputInformation(void);

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, int);

  virtual void SetNthInput(unsigned int num, DataObject *input)
  {
    Superclass::SetNthInput(num, input);
  }

private:
  ImageToVectorImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);           //purposely not implemented
};
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToVectorImageFilter.txx"
#endif

#endif
