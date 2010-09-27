/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRegularImageBase.h
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
#ifndef __itkRegularImageBase_h
#define __itkRegularImageBase_h

#include "itkPhysicalImageBase.h"

#include "itkImageRegion.h"
#include "itkIndex.h"
#include "itkObjectFactory.h"
#include "itkOffset.h"
#include "itkPoint.h"
#include "itkSize.h"
#include "itkFixedArray.h"
#include "itkPoint.h"
#include "itkMatrix.h"
#include "itkContinuousIndex.h"
#include "itkImageHelper.h"
#include <vnl/vnl_matrix_fixed.txx>

#include "itkImageRegion.h"

#ifdef ITK_USE_TEMPLATE_META_PROGRAMMING_LOOP_UNROLLING
#include "itkImageTransformHelper.h"
#endif

namespace itk
{

/** \class RegularImageBase
 * \brief Base class for templated image classes.
 *
 * RegularImageBase is the base class for the templated Image
 * classes. RegularImageBase is templated over the dimension of the image. It
 * provides the API and ivars that depend solely on the dimension of
 * the image.  RegularImageBase does not store any of the image (pixel) data.
 * Storage for the pixels and the pixel access methods are defined in
 * subclasses of RegularImageBase, namely Image and ImageAdaptor.
 *
 * There are three sets of meta-data describing an image. These are "Region"
 * objects that define a portion of an image via a starting index for the
 * image array and a size. The ivar LargestPossibleRegion defines the size
 * and starting index of the image dataset. The entire image dataset, however,
 * may not be resident in memory. The region of the image that is resident in
 * memory is defined by the "BufferedRegion". The Buffer is a contiguous block
 * of memory.  The third set of meta-data defines a region of interest, called
 * the "RequestedRegion". The RequestedRegion is used by the pipeline
 * execution model to define what a filter is requested to produce.
 *
 * [RegionIndex, RegionSize] C [BufferIndex, BufferSize]
 *                           C [ImageIndex, ImageSize]
 *
 * \ingroup ImageObjects
 * \ingroup ITKSystemObjects
 *
 */
template< unsigned int VImageDimension = 2 >
class ITK_EXPORT RegularImageBase:public PhysicalImageBase< VImageDimension >
{
public:
  /** Standard typedefs. */
  typedef RegularImageBase                     Self;
  typedef PhysicalImageBase< VImageDimension > Superclass;
  typedef SmartPointer< Self >                 Pointer;
  typedef SmartPointer< const Self >           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RegularImageBase, PhysicalImageBase);

  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel
   * type and dimension) when they need compile time access to the dimension
   * of the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Index typedef support. An index is used to access pixel values. */
  typedef Index< VImageDimension >           IndexType;
  typedef typename IndexType::IndexValueType IndexValueType;

  /** Offset typedef support. An offset represent relative position
   * between indices. */
  typedef Offset< VImageDimension >            OffsetType;
  typedef typename OffsetType::OffsetValueType OffsetValueType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef Size< VImageDimension >          SizeType;
  typedef typename SizeType::SizeValueType SizeValueType;

  /** Region typedef support. A region is used to specify a subset of an image.
    */
  typedef ImageRegion< VImageDimension > RegionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. ITK only
   * supports positive spacing value: negative values may cause
   * undesirable results.  */
  typedef double                                      SpacingValueType;
  typedef Vector< SpacingValueType, VImageDimension > SpacingType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0,0). */
  typedef double                                   PointValueType;
  typedef Point< PointValueType, VImageDimension > PointType;

  /** Direction typedef support.  The Direction is a matix of
   * direction cosines that specify the direction between samples.
   * */
  typedef typename Superclass::DirectionType DirectionType;

  /** Set the direction cosines of the image. The direction cosines
   * are vectors that point from one pixel to the next.
   *
   * One row of the matrix indicates the direction cosines of the unit vector
   * that is parallel to the lines of the image grid corresponding to that
   * dimension. For example, and image with Direction matrix
   *
   *    0.866   0.500
   *   -0.500   0.866
   *
   * has an image grid were the fastest changing index (dimension[0]) walks
   * over a line that in Physical space is oriented parallel to the vector
   * (0.866,0.5). The second fastest changing index (dimension[1]) walks along
   * a line that in Physical space is oriented parallel to the vector
   * (-0.5,0.866)
   *
   * The vectors whose direction cosines are stored in the Direction matrix,
   * are expected to be orthogonal to each other, and they are expected to form
   * a right handed coordinate system, but this is not checked nor enforced in
   * the itk::RegularImageBase.
   *
   * For details, please see:
   *
   * http://www.itk.org/Wiki/Proposals:Orientation#Some_notes_on_the_DICOM_convention_and_current_ITK_usage
   *
   * \sa GetDirection() */
  virtual void SetDirection(const DirectionType direction);

  /** Get the spacing (size of a pixel) `of the image. The
   * spacing is the geometric distance between image samples.
   * The value returned is a pointer to a double array.
   * For RegularImageBase and Image, the default data spacing is unity. */
  itkGetConstReferenceMacro(Spacing, SpacingType);

  /** Set the spacing (size of a pixel) of the image. The
   * spacing is the geometric distance between image samples.
   * It is stored internally as double, but may be set from
   * float. These methods also pre-compute the Index to Physical
   * point transforms of the image.
   * \sa GetSpacing() */
  virtual void SetSpacing(const SpacingType & spacing);

  virtual void SetSpacing(const double spacing[VImageDimension]);

  virtual void SetSpacing(const float spacing[VImageDimension]);

  /** Get the index (discrete) of a voxel from a physical point.
   * Floating point index results are rounded to integers
   * Returns true if the resulting index is within the image, false otherwise
   * \sa Transform */
#ifdef ITK_USE_TEMPLATE_META_PROGRAMMING_LOOP_UNROLLING
  template< class TCoordRep >
  bool TransformPhysicalPointToIndex(
    const Point< TCoordRep, VImageDimension > & point,
    IndexType & index) const
  {
    ImageTransformHelper< VImageDimension, VImageDimension - 1, VImageDimension - 1 >::TransformPhysicalPointToIndex(
      this->m_PhysicalPointToIndex, this->m_Origin, point, index);

    // Now, check to see if the index is within allowed bounds
    const bool isInside = this->GetLargestPossibleRegion().IsInside(index);
    return isInside;
  }

#else
  template< class TCoordRep >
  bool TransformPhysicalPointToIndex(
    const Point< TCoordRep, VImageDimension > & point,
    IndexType & index) const
  {
    for ( unsigned int i = 0; i < VImageDimension; i++ )
      {
      TCoordRep sum = NumericTraits< TCoordRep >::Zero;
      for ( unsigned int j = 0; j < VImageDimension; j++ )
        {
        sum += this->m_PhysicalPointToIndex[i][j] * ( point[j] - this->m_Origin[j] );
        }
      index[i] = Math::RoundHalfIntegerUp< IndexValueType >(sum);
      }

    // Now, check to see if the index is within allowed bounds
    const bool isInside = this->GetLargestPossibleRegion().IsInside(index);

    return isInside;
  }

#endif

  /** \brief Get the continuous index from a physical point
   *
   * Returns true if the resulting index is within the image, false otherwise.
   * \sa Transform */
  template< class TCoordRep >
  bool TransformPhysicalPointToContinuousIndex(
    const Point< TCoordRep, VImageDimension > & point,
    ContinuousIndex< TCoordRep, VImageDimension > & index) const
  {
    Vector< double, VImageDimension > cvector;

    for ( unsigned int k = 0; k < VImageDimension; k++ )
      {
      cvector[k] = point[k] - this->m_Origin[k];
      }
    cvector = m_PhysicalPointToIndex * cvector;
    for ( unsigned int i = 0; i < VImageDimension; i++ )
      {
      index[i] = static_cast< TCoordRep >( cvector[i] );
      }

    // Now, check to see if the index is within allowed bounds
    const bool isInside = this->GetLargestPossibleRegion().IsInside(index);

    return isInside;
  }

  /** Get a physical point (in the space which
   * the origin and spacing infomation comes from)
   * from a continuous index (in the index space)
   * \sa Transform */
  template< class TCoordRep >
  void TransformContinuousIndexToPhysicalPoint(
    const ContinuousIndex< TCoordRep, VImageDimension > & index,
    Point< TCoordRep, VImageDimension > & point) const
  {
    for ( unsigned int r = 0; r < VImageDimension; r++ )
      {
      TCoordRep sum = NumericTraits< TCoordRep >::Zero;
      for ( unsigned int c = 0; c < VImageDimension; c++ )
        {
        sum += this->m_IndexToPhysicalPoint(r, c) * index[c];
        }
      point[r] = sum + this->m_Origin[r];
      }
  }

  /** Get a physical point (in the space which
   * the origin and spacing infomation comes from)
   * from a discrete index (in the index space)
   *
   * \sa Transform */
#ifdef ITK_USE_TEMPLATE_META_PROGRAMMING_LOOP_UNROLLING
  template< class TCoordRep >
  void TransformIndexToPhysicalPoint(
    const IndexType & index,
    Point< TCoordRep, VImageDimension > & point) const
  {
    ImageTransformHelper< VImageDimension, VImageDimension - 1, VImageDimension - 1 >::TransformIndexToPhysicalPoint(
      this->m_IndexToPhysicalPoint, this->m_Origin, index, point);
  }

#else
  template< class TCoordRep >
  void TransformIndexToPhysicalPoint(
    const IndexType & index,
    Point< TCoordRep, VImageDimension > & point) const
  {
    for ( unsigned int i = 0; i < VImageDimension; i++ )
      {
      point[i] = this->m_Origin[i];
      for ( unsigned int j = 0; j < VImageDimension; j++ )
        {
        point[i] += m_IndexToPhysicalPoint[i][j] * index[j];
        }
      }
  }

#endif

  /** Get a physical point (in the space which
   * the origin and spacing infomation comes from)
   * from a discrete index (in the index space)
   *
   * \sa Transform */

  /** Take a vector or covariant vector that has been computed in the
   * coordinate system parallel to the image grid and rotate it by the
   * direction cosines in order to get it in terms of the coordinate system of
   * the image acquisition device.  This implementation in the Image
   * multiply the array (vector or covariant vector) by the matrix of Direction
   * Cosines. The arguments of the method are of type FixedArray to make
   * possible to use this method with both Vector and CovariantVector.
   * The Method is implemented differently in the itk::Image.
   *
   * \sa Image
   */
  template< class TCoordRep >
  void TransformLocalVectorToPhysicalVector(
    const FixedArray< TCoordRep, VImageDimension > & inputGradient,
    FixedArray< TCoordRep, VImageDimension > & outputGradient) const
  {
    //
    //TODO: This temporary implementation should be replaced with Template
    // MetaProgramming.
    //
    const DirectionType & direction = this->GetDirection();

    for ( unsigned int i = 0; i < VImageDimension; i++ )
      {
      typedef typename NumericTraits< TCoordRep >::AccumulateType CoordSumType;
      CoordSumType sum = NumericTraits< CoordSumType >::Zero;
      for ( unsigned int j = 0; j < VImageDimension; j++ )
        {
        sum += direction[i][j] * inputGradient[j];
        }
      outputGradient[i] = static_cast< TCoordRep >( sum );
      }
  }

  /** Copy information from the specified data set.  This method is
   * part of the pipeline execution model. By default, a ProcessObject
   * will copy meta-data from the first input to all of its
   * outputs. See ProcessObject::GenerateOutputInformation().  Each
   * subclass of DataObject is responsible for being able to copy
   * whatever meta-data it needs from from another DataObject.
   * RegularImageBase has more meta-data than its DataObject.  Thus, it must
   * provide its own version of CopyInformation() in order to copy the
   * LargestPossibleRegion from the input parameter. */
  virtual void CopyInformation(const DataObject *data);

protected:
  RegularImageBase();
  ~RegularImageBase();
  virtual void PrintSelf(std::ostream & os, Indent indent) const;

  /** Compute helper matrices used to transform Index coordinates to
   * PhysicalPoint coordinates and back. This method is virtual and will be
   * overloaded in derived classes in order to provide backward compatibility
   * behavior in classes that did not used to take image orientation into
   * account.  */
  virtual void ComputeIndexToPhysicalPointMatrices();

protected:
  /** Origin and spacing of physical coordinates. This variables are
   * protected for efficiency.  They are referenced frequently by
   * inner loop calculations. */
  SpacingType m_Spacing;

  /** Matrices intended to help with the conversion of Index coordinates
   *  to PhysicalPoint coordinates */
  DirectionType m_IndexToPhysicalPoint;
  DirectionType m_PhysicalPointToIndex;

private:
  RegularImageBase(const Self &);      //purposely not implemented
  void operator=(const Self &); //purposely not implemented
};
} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_RegularImageBase(_, EXPORT, TypeX, TypeY)         \
  namespace itk                                                 \
  {                                                             \
  _( 1 ( class EXPORT RegularImageBase< ITK_TEMPLATE_1 TypeX > ) )     \
  namespace Templates                                           \
  {                                                             \
  typedef RegularImageBase< ITK_TEMPLATE_1 TypeX > RegularImageBase##TypeY; \
  }                                                             \
  }

#if ITK_TEMPLATE_EXPLICIT
//template <unsigned int VImageDimension> const unsigned int
// itk::RegularImageBase<VImageDimension>::ImageDimension;
#include "Templates/itkRegularImageBase+-.h"
#endif

#if ITK_TEMPLATE_TXX
#include "itkRegularImageBase.txx"
#endif

#endif
