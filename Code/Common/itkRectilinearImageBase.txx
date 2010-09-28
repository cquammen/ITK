/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRectilinearImageBase.txx
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
#ifndef __itkRectilinearImageBase_txx
#define __itkRectilinearImageBase_txx

#include "itkRectilinearImageBase.h"

#include "itkFastMutexLock.h"
#include "itkProcessObject.h"
#include "itkSpatialOrientation.h"
#include <string.h>

namespace itk
{
/**
 *
 */
template< unsigned int VImageDimension >
RectilinearImageBase< VImageDimension >
::RectilinearImageBase()
{
  m_DefaultSpacing.Fill(1.0);
  for ( unsigned int dim; dim < VImageDimension; dim++ )
    {
    m_Spacings[dim].Fill( m_DefaultSpacing[dim] );
    }
}

/**
 *
 */
template< unsigned int VImageDimension >
RectilinearImageBase< VImageDimension >
::~RectilinearImageBase()
{}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::SetDefaultSpacing(const DefaultSpacingType & spacing)
{
  this->m_DefaultSpacing = spacing;
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::SetDefaultSpacing(const double spacing[VImageDimension])
{
  DefaultSpacingType s(spacing);

  this->SetDefaultSpacing(s);
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::SetDefaultSpacing(const float spacing[VImageDimension])
{
  Vector< float, VImageDimension > sf(spacing);
  DefaultSpacingType               s;
  s.CastFrom(sf);
  this->SetDefaultSpacing(s);
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::SetSpacing(unsigned int dimension, unsigned int index,
             const SpacingValueType & spacing)
{
  itkDebugMacro("setting Spacing to " << spacing);
  if ( spacing != this->m_Spacings[dimension][index] )
    {
    this->m_Spacings[dimension][index] = spacing;
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::SetSpacing(unsigned int dimension, unsigned int index, float spacing)
{
  this->SetSpacing(dimension, index, static_cast<SpacingValueType>(spacing));
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::SetSpacing(unsigned int dimension, unsigned int index, double spacing)
{
  this->SetSpacing(dimension, index, static_cast<SpacingValueType>(spacing));
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
typename RectilinearImageBase< VImageDimension >::SpacingValueType
RectilinearImageBase< VImageDimension >
::GetSpacing(unsigned int dimension, unsigned int index)
{
  return m_Spacings[dimension][index];
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::SetLargestPossibleRegion(const RegionType & region)
{
  Superclass::SetLargestPossibleRegion(region);

  // Update the spacings arrays to be of this size
  for ( unsigned int dim = 0; dim < VImageDimension; dim++ )
    {
    if ( this->m_Spacings[dim].Size() != region.GetSize()[dim] )
      {
      this->m_Spacings[dim].SetSize(region.GetSize()[dim]);
      this->m_Spacings[dim].Fill(this->m_DefaultSpacing[dim]);
      }
    }
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::CopyInformation(const DataObject *data)
{
  // Standard call to the superclass' method
  Superclass::CopyInformation(data);

  if ( data )
    {
    // Attempt to cast data to an RectilinearImageBase
    const RectilinearImageBase< VImageDimension > *imgData;

    try
      {
      imgData = dynamic_cast< const RectilinearImageBase< VImageDimension > * >( data );
      }
    catch ( ... )
      {
      return;
      }

    if ( imgData )
      {
      // Copy the meta data for this data type
      for ( unsigned int dim; dim < VImageDimension; dim++ )
        {
        for ( unsigned int index; index < this->m_Spacings[dim].Size(); index++ )
          {
          this->SetSpacing(dim, index, imgData->GetSpacing(dim, index));
          }
        }
      this->SetDefaultSpacing( imgData->GetDefaultSpacing() );
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::RectilinearImageBase::CopyInformation() cannot cast "
                         << typeid( data ).name() << " to "
                         << typeid( const RectilinearImageBase * ).name() );
      }
    }
}

/**
 *
 */
template< unsigned int VImageDimension >
void
RectilinearImageBase< VImageDimension >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  for ( unsigned int dim; dim < VImageDimension; dim++ )
    {
    for ( unsigned int index; index < this->m_Spacings[dim].Size(); index++)
      {
      os << indent << "Dimension spacing: " << this->m_Spacings[dim] << std::endl;
      }
    }

  os << indent << "Default spacing: " << this->m_DefaultSpacing << std::endl;
}
} // end namespace itk

#endif
