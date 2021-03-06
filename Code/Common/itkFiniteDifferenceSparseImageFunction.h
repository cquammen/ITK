/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFiniteDifferenceSparseImageFunction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFiniteDifferenceSparseImageFunction_h
#define __itkFiniteDifferenceSparseImageFunction_h

#include "itkFiniteDifferenceFunction.h"

namespace itk
{
/**
 * \class FiniteDifferenceSparseImageFunction
 *
 * \brief This is the base class for function classes that can be used with
 * filters derived from FiniteDifferenceSparseImageFilter.
 *
 * \par This class is derived from FiniteDifferenceFunction. It is designed to
 *work with neighborhoods of pointers instead of actual data. Towards this
 *purpose ComputeUpdate method is no longer used and is replaced by the
 *ComputeSparseUpdate method. ComputeSparseUpdate assumes that the pointers are
 *associated with structures that have the member variable m_Data. The pointers
 *would normally originate from a SparseImage object.
 *
 * \par The PrecomputeSparseUpdate method can be defined to do a first pass of
 *computations which then can be used by ComputeSparseUpdate. If
 *PrecomputeSparseUpdate is used then the m_PrecomputeFlag should be set in the
 *FiniteDifferenceSparseImageFilter class. The precomputation step can be used
 *to speed up computation by avoiding repetitions. See the
 *NormalVectorDiffusionFunction for an example.
 */
template< class TSparseImageType >
class ITK_EXPORT FiniteDifferenceSparseImageFunction:
  public FiniteDifferenceFunction< TSparseImageType >
{
public:
  /** Standard class typedef. */
  typedef FiniteDifferenceSparseImageFunction          Self;
  typedef FiniteDifferenceFunction< TSparseImageType > Superclass;
  typedef SmartPointer< Self >                         Pointer;
  typedef SmartPointer< const Self >                   ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(FiniteDifferenceSparseImageFunction, FiniteDifferenceFunction);

  /** The image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Typedefs from the superclass. */
  typedef typename Superclass::PixelType        PixelType;
  typedef typename Superclass::TimeStepType     TimeStepType;
  typedef typename Superclass::RadiusType       RadiusType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  typedef typename Superclass::FloatOffsetType  FloatOffsetType;
  typedef typename Superclass::ImageType        SparseImageType;

  /** The index type for the sparse image. */
  typedef typename SparseImageType::IndexType IndexType;

  /** The actual type of nodes stored the sparse image. */
  typedef typename SparseImageType::NodeType NodeType;

  /** The type for the variables of NodeType. Scalar or vector. */
  typedef typename NodeType::NodeDataType NodeDataType;

  /** This function is not called from the FiniteDifferenceSparseImageFilter
   *  class because we need to work with neighborhoods of pointers to data
   *  variables instead of neighborhoods of data directly. This function is
   *  replaced by the ComputeSparseUpdate function. */
  virtual PixelType ComputeUpdate(const NeighborhoodType &,
                                  void *,
                                  const FloatOffsetType &)
  {
    return static_cast< PixelType >( 0 );
  }

  /** The update called from the FiniteDifferenceSparseImageFilter. This
      function replaces the ComputeUpdate function. */
  virtual NodeDataType
  ComputeSparseUpdate( NeighborhoodType & neighborhood,
                       void *globalData,
                       const FloatOffsetType & offset = FloatOffsetType(0.0) )
  const = 0;

  /** This function provides support for a 2 step update computation that
   *  avoids repetitive computation. FiniteDifferenceSparseImageFilter first
   *  calls this function for all active pixels in the SparseImage class, then
   *  calls ComputeSparseUpdate for the same set of pixels. This is used in
   *  NormalVectorDiffusionFunction to calculate flux variables which are then
   *  used to compute the updates. Intermediate variables such as the flux in
   *  the above examples are stored in the nodes of the SparseImage
   *  itself. Therefore, this function will have to know about the NodeType it
   *  is dealing with. This function does nothing by default. */
  virtual void PrecomputeSparseUpdate(NeighborhoodType &) const {}
protected:
  FiniteDifferenceSparseImageFunction() {}
  ~FiniteDifferenceSparseImageFunction() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  FiniteDifferenceSparseImageFunction(const Self &); //purposely not implemented
  void operator=(const Self &);                      //purposely not implemented
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFiniteDifferenceSparseImageFunction.txx"
#endif

#endif
