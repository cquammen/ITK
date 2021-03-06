/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorConfidenceConnectedImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorConfidenceConnectedImageFilter_h
#define __itkVectorConfidenceConnectedImageFilter_h

#include "itkImage.h"
#include "itkImageToImageFilter.h"
#include "itkMahalanobisDistanceThresholdImageFunction.h"

namespace itk
{
/** \class VectorConfidenceConnectedImageFilter
 * \brief Segment pixels with similar statistics using connectivity
 *
 * This filter extracts a connected set of pixels whose pixel
 * intensities are consistent with the pixel statistics of a seed
 * point. The mean and variance across a neighborhood (8-connected,
 * 26-connected, etc.) are calculated for a seed point.  Then
 * pixels connected to this seed point whose values are within
 * the confidence interval for the seed point are grouped. The
 * width of the confidence interval is controlled by the "Multiplier"
 * variable (the confidence interval is the mean plus or minus
 * the "Multiplier" times the standard deviation). If the intensity
 * variations across a segment were gaussian, a "Multiplier" setting
 * of 2.5 would define a confidence interval wide enough to capture
 * 99% of samples in the segment.
 *
 * After this initial segmentation is calculated, the mean and
 * variance are re-calculated. All the pixels in the previous
 * segmentation are used to calculate the mean the standard deviation
 * (as opposed to using the pixels in the neighborhood of the seed
 * point).  The segmentation is then recalculted using these refined
 * estimates for the mean and variance of the pixel values.  This
 * process is repeated for the specified number of iterations.
 * Setting the "NumberOfIterations" to zero stops the algorithm
 * after the initial segmentation from the seed point.
 *
 * NOTE: the lower and upper threshold are restricted to lie within the
 * valid numeric limits of the input data pixel type. Also, the limits
 * may be adjusted to contain the seed point's intensity.
 * \ingroup RegionGrowingSegmentation
 */
template< class TInputImage, class TOutputImage >
class ITK_EXPORT VectorConfidenceConnectedImageFilter:
  public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef VectorConfidenceConnectedImageFilter            Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods).  */
  itkTypeMacro(VectorConfidenceConnectedImageFilter,
               ImageToImageFilter);

  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::PixelType  InputImagePixelType;
  typedef typename InputImageType::IndexType  IndexType;
  typedef typename InputImageType::SizeType   SizeType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType  OutputImagePixelType;

  typedef std::vector< IndexType > SeedsContainerType;

  typedef MahalanobisDistanceThresholdImageFunction<
    InputImageType >
  DistanceThresholdFunctionType;

  typedef typename DistanceThresholdFunctionType::CovarianceMatrixType CovarianceMatrixType;
  typedef typename DistanceThresholdFunctionType::MeanVectorType       MeanVectorType;

  typedef  typename DistanceThresholdFunctionType::Pointer DistanceThresholdFunctionPointer;

  void PrintSelf(std::ostream & os, Indent indent) const;

  /** Set seed point. This method is deprecated, please use AddSeed() */
  void SetSeed(const IndexType & seed)
  {
    m_Seeds.clear();
    this->AddSeed(seed);
  }

  /** Add seed point. */
  void AddSeed(const IndexType & seed)
  {
    m_Seeds.push_back(seed);
    this->Modified();
  }

  /** Set/Get the multiplier to define the confidence interval.  Multiplier
   * can be anything greater than zero. A typical value is 2.5 */
  itkSetMacro(Multiplier, double);
  itkGetConstMacro(Multiplier, double);

  /** Set/Get the number of iterations */
  itkSetMacro(NumberOfIterations, unsigned int);
  itkGetConstMacro(NumberOfIterations, unsigned int);

  /** Set/Get value to replace thresholded pixels */
  itkSetMacro(ReplaceValue, OutputImagePixelType);
  itkGetConstMacro(ReplaceValue, OutputImagePixelType);

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro(InitialNeighborhoodRadius, unsigned int);
  itkGetConstReferenceMacro(InitialNeighborhoodRadius, unsigned int);

  /** Get the Mean Vector computed during the segmentation */
  const MeanVectorType & GetMean() const;

  /** Get the Covariance matrix computed during the segmentation */
  const CovarianceMatrixType & GetCovariance() const;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( OutputEqualityComparableCheck,
                   ( Concept::EqualityComparable< OutputImagePixelType > ) );
  itkConceptMacro( InputHasNumericTraitsCheck,
                   ( Concept::HasNumericTraits< typename InputImagePixelType::ValueType > ) );
  itkConceptMacro( OutputOStreamWritableCheck,
                   ( Concept::OStreamWritable< OutputImagePixelType > ) );
  /** End concept checking */
#endif
protected:
  VectorConfidenceConnectedImageFilter();

  // Override since the filter needs all the data for the algorithm
  void GenerateInputRequestedRegion();

  // Override since the filter produces the entire dataset
  void EnlargeOutputRequestedRegion(DataObject *output);

  void GenerateData();

private:
  VectorConfidenceConnectedImageFilter(const Self &); //purposely not
                                                      // implemented
  void operator=(const Self &);                       //purposely not

  // implemented

  SeedsContainerType   m_Seeds;
  double               m_Multiplier;
  unsigned int         m_NumberOfIterations;
  OutputImagePixelType m_ReplaceValue;
  unsigned int         m_InitialNeighborhoodRadius;

  DistanceThresholdFunctionPointer m_ThresholdFunction;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorConfidenceConnectedImageFilter.txx"
#endif

#endif
