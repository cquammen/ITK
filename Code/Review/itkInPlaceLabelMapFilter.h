/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkInPlaceLabelMapFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkInPlaceLabelMapFilter_h
#define __itkInPlaceLabelMapFilter_h

#include "itkLabelMapFilter.h"

namespace itk
{
/** \class InPlaceLabelMapFilter
 * \brief Base class for filters that takes an image as input and overwrites
 * that image as the output
 *
 * InPlaceLabelMapFilter is the base class for all process objects whose
 * output image data is constructed by overwriting the input image
 * data. In other words, the output bulk data is the same block of
 * memory as the input bulk data.  This filter provides the mechanisms
 * for in place image processing while maintaining general pipeline
 * mechanics. InPlaceLabelMapFilters use less memory than standard
 * ImageToImageFilters because the input buffer is reused as the
 * output buffer.  However, this benefit does not come without a cost.
 * Since the filter overwrites its input, the ownership of the bulk
 * data is transitioned from the input data object to the output data
 * object.  When a data object has multiple consumers with one
 * of the consumers being an in place filter, the in place filter
 * effectively destroys the bulk data for the data object. Upstream
 * filters will then have to re-execute to regenerate the data object's
 * bulk data for the remaining consumers.
 *
 * Since an InPlaceLabelMapFilter reuses the input bulk data memory for the
 * output bulk data memory, the input image type must match the output
 * image type.  If the input and output image types are not identical,
 * the filter reverts to a traditional ImageToImageFilter behaviour
 * where an output image is allocated.  In place operation can also be
 * controlled (when the input and output image type match) via the
 * methods InPlaceOn() and InPlaceOff().
 *
 * Subclasses of InPlaceLabelMapFilter must take extra care in how they
 * manage memory using (and perhaps overriding) the implementations of
 * ReleaseInputs() and AllocateOutputs() provided here.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction,
 *  INRA de Jouy-en-Josas, France.
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapToLabelImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template< class TInputImage >
class ITK_EXPORT InPlaceLabelMapFilter:public LabelMapFilter< TInputImage, TInputImage >
{
public:
  /** Standard class typedefs. */
  typedef InPlaceLabelMapFilter                      Self;
  typedef LabelMapFilter< TInputImage, TInputImage > Superclass;
  typedef SmartPointer< Self >                       Pointer;
  typedef SmartPointer< const Self >                 ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(InPlaceLabelMapFilter, LabelMapFilter);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;

  typedef typename InputImageType::PixelType  PixelType;
  typedef typename InputImageType::IndexType  IndexType;
  typedef typename InputImageType::RegionType RegionType;

  typedef TInputImage TOutputImage;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** In place operation can be turned on and off. This only has an
   * effect when the input and output image type match. */
  itkSetMacro(InPlace, bool);
  itkGetMacro(InPlace, bool);
  itkBooleanMacro(InPlace);

  /** Can the filter run in place? To do so, the filter's first input
   * and output must have the same dimension and pixel type. This
   * method can be used in conjunction with the InPlace ivar to
   * determine whether a particular use of the filter is really
   * running in place. Some filters may be able to optimize their
   * operation if the InPlace is true and CanRunInPlace is true. */
  bool CanRunInPlace() const
  {
    return ( typeid( TInputImage ) == typeid( TOutputImage ) );
  }

protected:
  InPlaceLabelMapFilter();
  ~InPlaceLabelMapFilter();

  virtual void PrintSelf(std::ostream & os, Indent indent) const;

  /** The GenerateData method normally allocates the buffers for all
   * of the outputs of a filter. Since InPlaceLabelMapFilter's can use an
   * overwritten version of the input for its output, the output
   * buffer should not be allocated. When possible, we graft the input
   * to the filter to the output.  If an InPlaceFilter has multiple
   * outputs, then it would need to override this method to graft one
   * of its outputs and allocate the remaining. If a filter is
   * threaded (i.e. it provides an implementation of
   * ThreadedGenerateData()), this method is called automatically. If
   * an InPlaceFilter is not threaded (i.e. it provides an
   * implementation of GenerateData()), then this method (or
   * equivalent) must be called in GenerateData(). */
  virtual void AllocateOutputs();

  /** InPlaceLabelMapFilter may transfer ownership of the input bulk data
   * to the output object.  Once the output object owns the bulk data
   * (done in AllocateOutputs()), the input object must release its
   * hold on the bulk data.  ProcessObject::ReleaseInputs() only
   * releases the input bulk data when the user has set the
   * ReleaseDataFlag.  InPlaceLabelMapFilter::ReleaseInputs() also
   * releases the input that it has overwritten.
   *
   * \sa ProcessObject::ReleaseInputs() */
  virtual void ReleaseInputs();

  /**
   * Return the output label collection image, instead of the input as in the default
   * implementation
   */
  virtual InputImageType * GetLabelMap()
  {
    return this->GetOutput();
  }

  typedef typename Superclass::LabelObjectContainerType          LabelObjectContainerType;
  typedef typename Superclass::LabelObjectContainerConstIterator LabelObjectContainerConstIterator;
private:
  InPlaceLabelMapFilter(const Self &); //purposely not implemented
  void operator=(const Self &);        //purposely not implemented

  bool m_InPlace;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInPlaceLabelMapFilter.txx"
#endif

#endif
