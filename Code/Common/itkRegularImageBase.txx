/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRegularImageBase.txx
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
#ifndef __itkRegularImageBase_txx
#define __itkRegularImageBase_txx

#include "itkRegularImageBase.h"

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
RegularImageBase< VImageDimension >
::RegularImageBase()
{
  m_Spacing.Fill(1.0);
  m_IndexToPhysicalPoint.SetIdentity();
  m_PhysicalPointToIndex.SetIdentity();
}

/**
 *
 */
template< unsigned int VImageDimension >
RegularImageBase< VImageDimension >
::~RegularImageBase()
{}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RegularImageBase< VImageDimension >
::SetSpacing(const SpacingType & spacing)
{
  itkDebugMacro("setting Spacing to " << spacing);
  if ( this->m_Spacing != spacing )
    {
    this->m_Spacing = spacing;
    this->ComputeIndexToPhysicalPointMatrices();
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RegularImageBase< VImageDimension >
::SetSpacing(const double spacing[VImageDimension])
{
  SpacingType s(spacing);

  this->SetSpacing(s);
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RegularImageBase< VImageDimension >
::SetSpacing(const float spacing[VImageDimension])
{
  Vector< float, VImageDimension > sf(spacing);
  SpacingType                      s;
  s.CastFrom(sf);
  this->SetSpacing(s);
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RegularImageBase< VImageDimension >
::SetDirection(const DirectionType direction)
{
  bool modified = false;

  for ( unsigned int r = 0; r < VImageDimension; r++ )
    {
    for ( unsigned int c = 0; c < VImageDimension; c++ )
      {
      if ( this->m_Direction[r][c] != direction[r][c] )
        {
        this->m_Direction[r][c] = direction[r][c];
        modified = true;
        }
      }
    }

  if ( modified )
    {
    this->m_DirectionInverse = this->m_Direction.GetInverse();
    this->ComputeIndexToPhysicalPointMatrices();
    }
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RegularImageBase< VImageDimension >
::ComputeIndexToPhysicalPointMatrices()
{
  DirectionType scale;

  for ( unsigned int i = 0; i < VImageDimension; i++ )
    {
    if ( this->m_Spacing[i] == 0.0 )
      {
      itkExceptionMacro("A spacing of 0 is not allowed: Spacing is " << this->m_Spacing);
      }
    scale[i][i] = this->m_Spacing[i];
    }

  if ( vnl_determinant( this->m_Direction.GetVnlMatrix() ) == 0.0 )
    {
    itkExceptionMacro(<< "Bad direction, determinant is 0. Direction is " << this->m_Direction);
    }

  this->m_IndexToPhysicalPoint = this->m_Direction * scale;
  this->m_PhysicalPointToIndex = m_IndexToPhysicalPoint.GetInverse();

  this->Modified();
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
RegularImageBase< VImageDimension >
::CopyInformation(const DataObject *data)
{
  // Standard call to the superclass' method
  Superclass::CopyInformation(data);

  if ( data )
    {
    // Attempt to cast data to an RegularImageBase
    const RegularImageBase< VImageDimension > *imgData;

    try
      {
      imgData = dynamic_cast< const RegularImageBase< VImageDimension > * >( data );
      }
    catch ( ... )
      {
      return;
      }

    if ( imgData )
      {
      // Copy the meta data for this data type
      this->SetSpacing( imgData->GetSpacing() );
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::RegularImageBase::CopyInformation() cannot cast "
                         << typeid( data ).name() << " to "
                         << typeid( const RegularImageBase * ).name() );
      }
    }
}

/**
 *
 */
template< unsigned int VImageDimension >
void
RegularImageBase< VImageDimension >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Spacing: " << this->GetSpacing() << std::endl;

  os << indent << "IndexToPointMatrix: " << std::endl;
  os << indent << this->m_IndexToPhysicalPoint << std::endl;

  os << indent << "PointToIndexMatrix: " << std::endl;
  os << indent << this->m_PhysicalPointToIndex << std::endl;
}
} // end namespace itk

#endif
