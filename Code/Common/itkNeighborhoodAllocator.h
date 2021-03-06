/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkNeighborhoodAllocator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNeighborhoodAllocator_h
#define __itkNeighborhoodAllocator_h
#include <iostream>

namespace itk
{
/** \class NeighborhoodAllocator
 *
 * This is a memory allocator for use as the default allocator type in
 * Neighborhood.  The API is designed to mimic that of vnl_vector so that
 * vnl_vector can also be used as an allocator for Neighborhood.
 *
 * The decision to create this allocator with the vnl_vector api (versus
 * using an STL allocator and wrapping the vnl_vector API) was made because
 * the STL allocator API is not guaranteed stable at this time.
 *
 * \ingroup Operators
 */
template< class TPixel >
class NeighborhoodAllocator
{
public:
  /** Standard class typedefs. */
  typedef NeighborhoodAllocator Self;

  /** Iterator support. Note that the naming of the typedefs is on purpose.
  * itk::Neighborhood makes reference to the allocator, which because it may
  * be vnl or other type, uses the lower case/underscore forms iterator and
  * const_iterator. */
  typedef TPixel *      iterator;
  typedef const TPixel *const_iterator;

  /** Default constructor */
  NeighborhoodAllocator():m_ElementCount(0), m_Data(0)  {}

  /** Default destructor */
  ~NeighborhoodAllocator()
  { this->Deallocate(); }

  /** Allocates memory using new() */
  void Allocate(unsigned int n)
  {
    m_Data = new TPixel[n];
    m_ElementCount = n;
  }

  /** Deallocates memory using delete[](). */
  void Deallocate()
  {
    if ( m_Data ) { delete[] m_Data; }
    m_ElementCount = 0;
  }

  /** Copy constructor. */
  NeighborhoodAllocator(const Self & other):m_ElementCount(0), m_Data(0)
  {
    this->set_size(other.m_ElementCount);
    for ( unsigned int i = 0; i < other.m_ElementCount; ++i )
      {
      this->operator[](i) = other[i];
      }
    m_ElementCount = other.m_ElementCount;
  }

  /** Assignment operator. */
  const Self & operator=(const Self & other)
  {
    this->set_size(other.m_ElementCount);
    for ( unsigned int i = 0; i < other.m_ElementCount; ++i )
      {
      this->operator[](i) = other[i];
      }
    m_ElementCount = other.m_ElementCount;
    return *this;
  }

  /** Comparison operator. */
  bool operator==(const Self & other) const
  {
    return ( m_Data == other.m_Data );
  }

  /** Not Equal operator. */
  bool operator!=(const Self & other) const
  {
    return ( m_Data != other.m_Data );
  }

  /** STL-style iterator support for the memory buffer. */
  iterator begin()
  { return m_Data; }
  const_iterator begin() const
  { return m_Data; }
  iterator end()
  { return ( m_Data + m_ElementCount ); }
  const_iterator end() const
  { return ( m_Data + m_ElementCount ); }
  unsigned int size() const
  { return m_ElementCount; }

  /** Data access methods */
  const TPixel & operator[](unsigned int i) const
  { return m_Data[i]; }
  TPixel & operator[](unsigned int i)
  { return m_Data[i]; }

  /** Allocates or Reallocates a buffer of size n */
  void set_size(unsigned int n)
  {
    if ( m_Data ) { Deallocate(); }
    this->Allocate(n);
  }

protected:
  unsigned int m_ElementCount;
  TPixel *     m_Data;
};

template< class TPixel >
inline std::ostream & operator<<(
  std::ostream & o, const NeighborhoodAllocator< TPixel >
  & a)
{
  o << "NeighborhoodAllocator { this = " << &a << ", begin = "
  << static_cast< const void * >( a.begin() )
  << ", size=" << a.size()
  << " }";
  return o;
}
} // end namespace itk
#endif
