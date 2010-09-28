/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkPhysicalImageBase.h
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
#ifndef __itkPhysicalImageBase_h
#define __itkPhysicalImageBase_h

#include "itkImageBase.h"

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
/** \class PhysicalImageBase
 * \brief Base class for templated image classes.
 *
 * PhysicalImageBase is the base class for the templated Image
 * classes. PhysicalImageBase is templated over the dimension of the image. It
 * provides the API and ivars that depend solely on the dimension of
 * the image.  PhysicalImageBase does not store any of the image (pixel) data.
 * Storage for the pixels and the pixel access methods are defined in
 * subclasses of PhysicalImageBase, namely Image and ImageAdaptor.
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
class ITK_EXPORT PhysicalImageBase:public ImageBase< VImageDimension >
{
public:
  /** Standard typedefs. */
  typedef PhysicalImageBase            Self;
  typedef ImageBase< VImageDimension > Superclass;
  typedef SmartPointer< Self >         Pointer;
  typedef SmartPointer< const Self >   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PhysicalImageBase, ImageBase);

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
  typedef Matrix< double, VImageDimension, VImageDimension > DirectionType;

  /** Set the origin of the image. The origin is the geometric
   * coordinates of the image origin.  It is stored internally
   * as double but may be set from float.
   * \sa GetOrigin() */
  itkSetMacro(Origin, PointType);
  virtual void SetOrigin(const double origin[VImageDimension]);

  virtual void SetOrigin(const float origin[VImageDimension]);

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
   * the itk::PhysicalImageBase.
   *
   * For details, please see:
   *
   * http://www.itk.org/Wiki/Proposals:Orientation#Some_notes_on_the_DICOM_convention_and_current_ITK_usage
   *
   * \sa GetDirection() */
  virtual void SetDirection(const DirectionType direction);

  /** Get the direction cosines of the image. The direction cosines
   * are vectors that point from one pixel to the next.
   * For PhysicalImageBase and Image, the default direction is identity. */
  itkGetConstReferenceMacro(Direction, DirectionType);

  /** Get the origin of the image. The origin is the geometric
   * coordinates of the index (0,0).  The value returned is a pointer
   * to a double array.  For PhysicalImageBase and Image, the default origin is
   * 0. */
  itkGetConstReferenceMacro(Origin, PointType);

  /** Copy information from the specified data set.  This method is
   * part of the pipeline execution model. By default, a ProcessObject
   * will copy meta-data from the first input to all of its
   * outputs. See ProcessObject::GenerateOutputInformation().  Each
   * subclass of DataObject is responsible for being able to copy
   * whatever meta-data it needs from from another DataObject.
   * PhysicalImageBase has more meta-data than its DataObject.  Thus, it must
   * provide its own version of CopyInformation() in order to copy the
   * LargestPossibleRegion from the input parameter. */
  virtual void CopyInformation(const DataObject *data);

protected:
  PhysicalImageBase();
  ~PhysicalImageBase();
  virtual void PrintSelf(std::ostream & os, Indent indent) const;

protected:
  /** Origin of physical coordinates. This variables are
   * protected for efficiency.  They are referenced frequently by
   * inner loop calculations. */
  PointType m_Origin;

  /** Direction cosine matrix that rotates image sample points. */
  DirectionType m_Direction;

  /** Inverse of the direction cosine matrix. */
  DirectionType m_DirectionInverse;

private:
  PhysicalImageBase(const Self &);      //purposely not implemented
  void operator=(const Self &); //purposely not implemented

};
} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_PhysicalImageBase(_, EXPORT, TypeX, TypeY)         \
  namespace itk                                                 \
  {                                                             \
  _( 1 ( class EXPORT PhysicalImageBase< ITK_TEMPLATE_1 TypeX > ) )     \
  namespace Templates                                           \
  {                                                             \
  typedef PhysicalImageBase< ITK_TEMPLATE_1 TypeX > PhysicalImageBase##TypeY; \
  }                                                             \
  }

#if ITK_TEMPLATE_EXPLICIT
//template <unsigned int VImageDimension> const unsigned int
// itk::PhysicalImageBase<VImageDimension>::ImageDimension;
#include "Templates/itkPhysicalImageBase+-.h"
#endif

#if ITK_TEMPLATE_TXX
#include "itkPhysicalImageBase.txx"
#endif

#endif
