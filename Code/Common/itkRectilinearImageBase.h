/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRectilinearImageBase.h
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
#ifndef __itkRectilinearImageBase_h
#define __itkRectilinearImageBase_h

#include "itkPhysicalImageBase.h"

#include "itkArray.h"
#include "itkFixedArray.h"
#include "itkImageRegion.h"
#include "itkIndex.h"
#include "itkObjectFactory.h"
#include "itkOffset.h"
#include "itkPoint.h"
#include "itkSize.h"
#include "itkPoint.h"
#include "itkMatrix.h"
#include "itkContinuousIndex.h"
#include "itkImageHelper.h"
#include <vnl/vnl_matrix_fixed.txx>

#include "itkImageRegion.h"

namespace itk
{

/** \class RectilinearImageBase
 * \brief Base class for templated image classes.
 *
 * RectilinearImageBase is the base class for the templated Image
 * classes. RectilinearImageBase is templated over the dimension of the image. It
 * provides the API and ivars that depend solely on the dimension of
 * the image.  RectilinearImageBase does not store any of the image (pixel) data.
 * Storage for the pixels and the pixel access methods are defined in
 * subclasses of RectilinearImageBase, namely Image and ImageAdaptor.
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
class ITK_EXPORT RectilinearImageBase:public PhysicalImageBase< VImageDimension >
{
public:
  /** Standard typedefs. */
  typedef RectilinearImageBase                 Self;
  typedef PhysicalImageBase< VImageDimension > Superclass;
  typedef SmartPointer< Self >                 Pointer;
  typedef SmartPointer< const Self >           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RectilinearImageBase, PhysicalImageBase);

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

  /** Spacing type. */
  typedef double                                      SpacingValueType;
  typedef Vector< SpacingValueType, VImageDimension > DefaultSpacingType;
  typedef Array< SpacingValueType >                   SpacingArrayType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0,0). */
  typedef double                                   PointValueType;
  typedef Point< PointValueType, VImageDimension > PointType;

  /** Direction typedef support.  The Direction is a matix of
   * direction cosines that specify the direction between samples.
   * */
  typedef typename Superclass::DirectionType DirectionType;

  /** Set/get the default spacing (size of a pixel) for the image.
   * When first initialized or when the LargestPossibleRegion is
   * changed, the spacing for each index of a particular dimension is
   * set to the default spacing for that dimension. The default
   * spacing is one. */
  virtual void SetDefaultSpacing(const DefaultSpacingType & spacing);

  virtual void SetDefaultSpacing(const double spacing[VImageDimension]);

  virtual void SetDefaultSpacing(const float spacing[VImageDimension]);

  /** Get the spacing (size of a pixel) of the image. The
   * spacing is the geometric distance between image samples.
   * The value returned is a pointer to a double array.
   * For RectilinearImageBase and Image, the default data spacing is unity. */
  itkGetConstReferenceMacro(DefaultSpacing, DefaultSpacingType);

  /** Set/get the spacing (size of a pixel) for the given dimension at
   * the given index. In a rectilinear image, the geometric distance
   * between image samples may vary at each index position. */
  virtual void SetSpacing(unsigned int dimension, unsigned int index,
                          const SpacingValueType & spacing);

  virtual void SetSpacing(unsigned int dimension, unsigned int index,
                          double spacing);

  virtual void SetSpacing(unsigned int dimension, unsigned int index,
                          float spacing);

  virtual SpacingValueType GetSpacing(unsigned int dimension, unsigned int index);

  /** Set the region object that defines the size and starting index
   * for the largest possible region this image could represent.  This
   * is used in determining how much memory would be needed to load an
   * entire dataset.  It is also used to determine boundary
   * conditions.
   * \sa ImageRegion, SetBufferedRegion(), SetRequestedRegion() */
  virtual void SetLargestPossibleRegion(const RegionType & region);

  /** Get the index (discrete) of a voxel from a physical point.
   * Floating point index results are rounded to integers
   * Returns true if the resulting index is within the image, false otherwise
   * \sa Transform */
  template< class TCoordRep >
  bool TransformPhysicalPointToIndex(
    const Point< TCoordRep, VImageDimension > & point,
    IndexType & index) const
  {
    for ( unsigned int dim = 0; dim < VImageDimension; dim++ )
      {
      TCoordRep sum = NumericTraits< TCoordRep >::Zero;
      for ( unsigned int j = 0; j < VImageDimension; j++ )
        {
        sum += this->m_DirectionInverse[dim][j] *
          ( point[j] - this->m_SpacingsPrefixSum[j][0] );
        }

      for ( unsigned int i = 0; i < this->m_Spacings[dim].Size(); i++ )
        {
        if ( this->m_SpacingsPrefixSum[dim][i] > sum )
          {
          index[dim] = i;
          break;
          }
        }
      }

    // TODO - need to properly handle image boundary cases

    // Now, check to see if the index is within allowed bounds
    const bool isInside = this->GetLargestPossibleRegion().IsInside(index);

    return isInside;
  }

  /** \brief Get the continuous index from a physical point
   *
   * Returns true if the resulting index is within the image, false otherwise.
   * \sa Transform */
  template< class TCoordRep >
  bool TransformPhysicalPointToContinuousIndex(
    const Point< TCoordRep, VImageDimension > & point,
    ContinuousIndex< TCoordRep, VImageDimension > & index) const
  {
    for ( unsigned int dim = 0; dim < VImageDimension; dim++ )
      {
      TCoordRep sum = NumericTraits< TCoordRep >::Zero;
      unsigned int uintIndex = 0;
      for ( unsigned int j = 0; j < VImageDimension; j++ )
        {
        sum += this->m_DirectionInverse[dim][j] *
          ( point[j] - this->m_SpacingsPrefixSum[j][0] );
        }

      for ( unsigned int i = 0; i < this->m_Spacings[dim].Size(); i++ )
        {
        if ( this->m_SpacingsPrefixSum[dim][i] > sum )
          {
          uintIndex = i;
          break;
          }
        }

      // Add the fractional component and integer part to get the
      // continuous index.
      index[dim] = ( sum - uintIndex ) / this->m_Spacings[dim][uintIndex];
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
    Point< TCoordRep, VImageDimension > opoint;
    for ( unsigned int dim = 0; dim < VImageDimension; dim++ )
      {
      // Integer part of the index.
      unsigned int i = static_cast<unsigned int>(index[dim]);

      // Fractional part of the index.
      double fraction = index[dim] - i;
      opoint[dim] = this->m_SpacingsPrefixSum[dim][i] +
        fraction * this->m_Spacings[dim][i];

      }

    for ( unsigned int c = 0; c < VImageDimension; c++ )
      {
      point[c] = NumericTraits< TCoordRep >::Zero;
      for ( unsigned int r = 0; r < VImageDimension; r++ )
        {
        point[c] += this->m_DirectionInverse[r][c] * opoint[c];
        }
      point[c] += this->m_Origin[c];
      }
  }

  /** Get a physical point (in the space which
   * the origin and spacing infomation comes from)
   * from a discrete index (in the index space)
   *
   * \sa Transform */
  template< class TCoordRep >
  void TransformIndexToPhysicalPoint(
    const IndexType & index,
    Point< TCoordRep, VImageDimension > & point) const
  {
    ContinuousIndex< TCoordRep, VImageDimension > cindex( index );
    TransformContinuousIndexToPhysicalPoint( cindex, point );
  }

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
   * RectilinearImageBase has more meta-data than its DataObject.  Thus, it must
   * provide its own version of CopyInformation() in order to copy the
   * LargestPossibleRegion from the input parameter. */
  virtual void CopyInformation(const DataObject *data);

protected:
  RectilinearImageBase();
  ~RectilinearImageBase();
  virtual void PrintSelf(std::ostream & os, Indent indent) const;

  void ComputeSpacingPrefixSum();

private:
  RectilinearImageBase(const Self &);      //purposely not implemented
  void operator=(const Self &); //purposely not implemented

  /** Image plane spacing in each dimension. Each image plane may have
   * a different spacing. To accommodate this, we store an array for
   * each dimension such that the size of the array is the number of
   * voxels in that dimension. */
  SpacingArrayType m_Spacings[VImageDimension];

  /** Prefix sum (exclusive) of spacings. Used as an acceleration data
   * structure in the transforms from physical points to indices. */
  SpacingArrayType m_SpacingsPrefixSum[VImageDimension];

  /** Default spacing. */
  DefaultSpacingType m_DefaultSpacing;
};
} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_RectilinearImageBase(_, EXPORT, TypeX, TypeY)         \
  namespace itk                                                 \
  {                                                             \
  _( 1 ( class EXPORT RectilinearImageBase< ITK_TEMPLATE_1 TypeX > ) )     \
  namespace Templates                                           \
  {                                                             \
  typedef RectilinearImageBase< ITK_TEMPLATE_1 TypeX > RectilinearImageBase##TypeY; \
  }                                                             \
  }

#if ITK_TEMPLATE_EXPLICIT
//template <unsigned int VImageDimension> const unsigned int
// itk::RectilinearImageBase<VImageDimension>::ImageDimension;
#include "Templates/itkRectilinearImageBase+-.h"
#endif

#if ITK_TEMPLATE_TXX
#include "itkRectilinearImageBase.txx"
#endif

#endif
