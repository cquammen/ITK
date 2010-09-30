/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRectilinearImageAdaptor.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRectilinearImageAdaptor_txx
#define __itkRectilinearImageAdaptor_txx
#include "itkRectilinearImageAdaptor.h"
#include "itkProcessObject.h"

namespace itk
{
/**
 * Constructor
 */
template< class TImage, class TAccessor >
RectilinearImageAdaptor< TImage, TAccessor >
::RectilinearImageAdaptor()
{
  // Allocate an internal image.  A process object might try to allocate an
  // temporary image that is the same type as its input or output.  If that
  // image type is an adaptor, we need to make sure that an internal image is
  // available because the process object will not know to call SetImage on
  // the adaptor.
  m_Image = TImage::New();
}

/**
 * Destructor
 */
template< class TImage, class TAccessor >
RectilinearImageAdaptor< TImage, TAccessor >
::~RectilinearImageAdaptor()
{}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::Initialize()
{
  // call the superclass' method first; then delegate
  Superclass::Initialize();

  // delegation to internal image
  m_Image->Initialize();
}

template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetPixelContainer(PixelContainer *container)
{
  if ( this->GetPixelContainer() != container )
    {
    m_Image->SetPixelContainer(container);
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::Graft(const DataObject *data)
{
  // call the superclass' implementation
  Superclass::Graft(data);

  if ( data )
    {
    // Attempt to cast data to an RectilinearImageAdaptor
    const Self *imgData;

    try
      {
      imgData = dynamic_cast< const Self * >( data );
      }
    catch ( ... )
      {
      return;
      }

    if ( imgData )
      {
      // Now copy anything remaining that is needed
      this->SetPixelContainer(
        const_cast< Self * >( imgData )->GetPixelContainer() );
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::RectilinearImageAdaptor::Graft() cannot cast "
                         << typeid( data ).name() << " to "
                         << typeid( const Self * ).name() );
      }
    }
}

/**
 *
 */
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
const typename RectilinearImageAdaptor< TImage, TAccessor >::OffsetValueType *
RectilinearImageAdaptor< TImage, TAccessor >
::GetOffsetTable(void) const
{
  return m_Image->GetOffsetTable();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
typename RectilinearImageAdaptor< TImage, TAccessor >::IndexType
RectilinearImageAdaptor< TImage, TAccessor >
::ComputeIndex(OffsetValueType offset) const
{
  return m_Image->ComputeIndex(offset);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::Update()
{
  Superclass::Update();

  m_Image->Update();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::UpdateOutputInformation()
{
  // call the superclass' method first, then delegate
  Superclass::UpdateOutputInformation();

  // delegation to internal image
  m_Image->UpdateOutputInformation();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::UpdateOutputData()
{
  // call the superclass' method first, then delegate
  Superclass::UpdateOutputData();

  // delegation to internal image
  m_Image->UpdateOutputData();
  SetBufferedRegion( m_Image->GetBufferedRegion() );
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::PropagateRequestedRegion()
throw ( InvalidRequestedRegionError )
{
  // call the superclass' method first, then delegate
  Superclass::PropagateRequestedRegion();

  // delegation to internal image
  m_Image->PropagateRequestedRegion();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetRequestedRegionToLargestPossibleRegion()
{
  // call the superclass' method first, then delegate
  Superclass::SetRequestedRegionToLargestPossibleRegion();

  // delegation to internal image
  m_Image->SetRequestedRegionToLargestPossibleRegion();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::CopyInformation(const DataObject *data)
{
  // call the superclass' method first, then delegate
  Superclass::CopyInformation(data);

  // delegation to internal image
  m_Image->CopyInformation(data);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetDefaultSpacing(const DefaultSpacingType & spacing)
{
  // delegation to internal image
  m_Image->SetDefaultSpacing(spacing);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetDefaultSpacing(const double *spacing /*[Self::ImageDimension]*/)
{
  // delegation to internal image
  m_Image->SetDefaultSpacing(spacing);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetDefaultSpacing(const float *spacing /*[Self::ImageDimension]*/)
{
  // delegation to internal image
  m_Image->SetDefaultSpacing(spacing);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
const typename RectilinearImageAdaptor< TImage, TAccessor >::DefaultSpacingType &
RectilinearImageAdaptor< TImage, TAccessor >
::GetDefaultSpacing(void) const
{
  return m_Image->GetDefaultSpacing();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetSpacing(unsigned int dimension, unsigned int index, const SpacingValueType & spacing)
{
  m_Image->SetSpacing(dimension, index, spacing);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetSpacing(unsigned int dimension, unsigned int index, double spacing)
{
  m_Image->SetSpacing(dimension, index, spacing);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetSpacing(unsigned int dimension, unsigned int index, float spacing)
{
  m_Image->SetSpacing(dimension, index, spacing);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
typename RectilinearImageAdaptor< TImage, TAccessor >::SpacingValueType
RectilinearImageAdaptor< TImage, TAccessor >
::GetSpacing(unsigned int dimension, unsigned int index)
{
  return m_Image->GetSpacing(dimension, index);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetOrigin(const PointType origin)
{
  // delegation to internal image
  m_Image->SetOrigin(origin);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetOrigin(const double *origin /*[Self::ImageDimension]*/)
{
  // delegation to internal image
  m_Image->SetOrigin(origin);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetOrigin(const float *origin /*[Self::ImageDimension]*/)
{
  // delegation to internal image
  m_Image->SetOrigin(origin);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
const typename RectilinearImageAdaptor< TImage, TAccessor >::PointType &
RectilinearImageAdaptor< TImage, TAccessor >
::GetOrigin(void) const
{
  return m_Image->GetOrigin();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetDirection(const DirectionType direction)
{
  // delegation to internal image
  m_Image->SetDirection(direction);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
const typename RectilinearImageAdaptor< TImage, TAccessor >::DirectionType &
RectilinearImageAdaptor< TImage, TAccessor >
::GetDirection(void) const
{
  return m_Image->GetDirection();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetImage(TImage *image)
{
  m_Image = image;
  Superclass::SetLargestPossibleRegion( m_Image->GetLargestPossibleRegion() );
  Superclass::SetBufferedRegion( m_Image->GetBufferedRegion() );
  Superclass::SetRequestedRegion( m_Image->GetRequestedRegion() );
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
const typename RectilinearImageAdaptor< TImage, TAccessor >::InternalPixelType  *
RectilinearImageAdaptor< TImage, TAccessor >
::GetBufferPointer() const
{
  return m_Image->GetBufferPointer();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
typename RectilinearImageAdaptor< TImage, TAccessor >::InternalPixelType  *
RectilinearImageAdaptor< TImage, TAccessor >
::GetBufferPointer()
{
  return m_Image->GetBufferPointer();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::Modified() const
{
  Superclass::Modified();

  m_Image->Modified();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
unsigned long
RectilinearImageAdaptor< TImage, TAccessor >
::GetMTime() const
{
  unsigned long mtime1, mtime2;

  mtime1 = Superclass::GetMTime();
  mtime2 = m_Image->GetMTime();

  return ( mtime1 >= mtime2 ? mtime1 : mtime2 );
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetBufferedRegion(const RegionType & region)
{
  // call the superclass' method first, then delegate
  Superclass::SetBufferedRegion(region);

  // delegation to internal image
  m_Image->SetBufferedRegion(region);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
const typename RectilinearImageAdaptor< TImage, TAccessor >::RegionType &
RectilinearImageAdaptor< TImage, TAccessor >
::GetBufferedRegion(void) const
{
  // delegation to internal image
  return m_Image->GetBufferedRegion();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetLargestPossibleRegion(const RegionType & region)
{
  // call the superclass' method first, then delegate
  Superclass::SetLargestPossibleRegion(region);

  // delegation to internal image
  m_Image->SetLargestPossibleRegion(region);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
const typename RectilinearImageAdaptor< TImage, TAccessor >::RegionType &
RectilinearImageAdaptor< TImage, TAccessor >
::GetLargestPossibleRegion(void) const
{
  // delegation to internal image
  return m_Image->GetLargestPossibleRegion();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetRequestedRegion(const RegionType & region)
{
  // call the superclass' method first, then delegate
  Superclass::SetRequestedRegion(region);

  // delegation to internal image
  m_Image->SetRequestedRegion(region);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
void
RectilinearImageAdaptor< TImage, TAccessor >
::SetRequestedRegion(DataObject *data)
{
  // call the superclass' method first, then delegate
  Superclass::SetRequestedRegion(data);

  // delegation to internal image
  m_Image->SetRequestedRegion(data);
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
bool
RectilinearImageAdaptor< TImage, TAccessor >
::VerifyRequestedRegion()
{
  // call the superclass' method first, then delegate
  Superclass::VerifyRequestedRegion();

  // delegation to internal image
  return m_Image->VerifyRequestedRegion();
}

//----------------------------------------------------------------------------
template< class TImage, class TAccessor >
const typename RectilinearImageAdaptor< TImage, TAccessor >::RegionType &
RectilinearImageAdaptor< TImage, TAccessor >
::GetRequestedRegion(void) const
{
  // delegation to internal image
  return m_Image->GetRequestedRegion();
}
} // end namespace itk

#endif
