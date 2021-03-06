/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkNumericTraitsRGBPixel.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNumericTraitsRGBPixel_h
#define __itkNumericTraitsRGBPixel_h

#include "itkNumericTraits.h"
#include "itkRGBPixel.h"

namespace itk
{
/** \class NumericTraits<RGBPixel< T > >
 * \brief Define numeric traits for RGBPixel.
 *
 * We provide here a generic implementation based on creating types of
 * RGBPixel whose components are the types of the NumericTraits from
 * the original RGBPixel components. This implementation require
 * support for partial specializations, since it is based on the
 * concept that:
 *   NumericTraits<RGBPixel< T > >  is defined piecewise by
 *   RGBPixel< NumericTraits< T > >
 *
 * \sa NumericTraits
 * \ingroup DataRepresentation
 */
template< typename T >
class NumericTraits< RGBPixel< T > >
{
private:

  typedef typename NumericTraits< T >::AbsType        ElementAbsType;
  typedef typename NumericTraits< T >::AccumulateType ElementAccumulateType;
  typedef typename NumericTraits< T >::FloatType      ElementFloatType;
  typedef typename NumericTraits< T >::PrintType      ElementPrintType;
  typedef typename NumericTraits< T >::RealType       ElementRealType;
public:

  /** Return the type of the native component type. */
  typedef T ValueType;

  typedef RGBPixel< T > Self;

  /** Unsigned component type */
  typedef RGBPixel< ElementAbsType > AbsType;

  /** Accumulation of addition and multiplication. */
  typedef RGBPixel< ElementAccumulateType > AccumulateType;

  /** Typedef for operations that use floating point instead of real precision
    */
  typedef RGBPixel< ElementFloatType > FloatType;

  /** Return the type that can be printed. */
  typedef RGBPixel< ElementPrintType > PrintType;

  /** Type for real-valued scalar operations. */
  typedef RGBPixel< ElementRealType > RealType;

  /** Type for real-valued scalar operations. */
  typedef ElementRealType ScalarRealType;

  /** Component wise defined element
   *
   * \note minimum value for floating pointer types is defined as
   * minimum positive normalize value.
   */
  static const Self max(const Self &)
  {
    return Self( NumericTraits< T >::max() );
  }

  static const Self min(const Self &)
  {
    return Self( NumericTraits< T >::min() );
  }

  static const Self max()
  {
    return Self( NumericTraits< T >::max() );
  }

  static const Self min()
  {
    return Self( NumericTraits< T >::min() );
  }

  static const Self NonpositiveMin()
  {
    return Self( NumericTraits< ValueType >::NonpositiveMin() );
  }

  static const Self ZeroValue()
  {
    return Self(NumericTraits< T >::Zero);
  }

  static const Self OneValue()
  {
    return Self(NumericTraits< T >::One);
  }

  /** \note: the functions are prefered over the member variables as
   * they are defined for all partial specialization
   */
  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;
};
} // end namespace itk

#endif // __itkNumericTraitsRGBPixel_h
