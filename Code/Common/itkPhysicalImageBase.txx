/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkPhysicalImageBase.txx
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
#ifndef __itkPhysicalImageBase_txx
#define __itkPhysicalImageBase_txx

#include "itkPhysicalImageBase.h"

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
PhysicalImageBase< VImageDimension >
::PhysicalImageBase()
{
  m_Origin.Fill(0.0);
  m_Direction.SetIdentity();
}

/**
 *
 */
template< unsigned int VImageDimension >
PhysicalImageBase< VImageDimension >
::~PhysicalImageBase()
{}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
PhysicalImageBase< VImageDimension >
::SetOrigin(const double origin[VImageDimension])
{
  PointType p(origin);

  this->SetOrigin(p);
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
PhysicalImageBase< VImageDimension >
::SetOrigin(const float origin[VImageDimension])
{
  Point< float, VImageDimension > of(origin);
  PointType                       p;
  p.CastFrom(of);
  this->SetOrigin(p);
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
PhysicalImageBase< VImageDimension >
::SetDirection(const DirectionType direction)
{
  bool modified = false;

  for ( unsigned int r = 0; r < VImageDimension; r++ )
    {
    for ( unsigned int c = 0; c < VImageDimension; c++ )
      {
      if ( m_Direction[r][c] != direction[r][c] )
        {
        m_Direction[r][c] = direction[r][c];
        modified = true;
        }
      }
    }

  if ( modified )
    {
    m_DirectionInverse = m_Direction.GetInverse();
    }
}

//----------------------------------------------------------------------------
template< unsigned int VImageDimension >
void
PhysicalImageBase< VImageDimension >
::CopyInformation(const DataObject *data)
{
  // Standard call to the superclass' method
  Superclass::CopyInformation(data);

  if ( data )
    {
    // Attempt to cast data to an PhysicalImageBase
    const PhysicalImageBase< VImageDimension > *imgData;

    try
      {
      imgData = dynamic_cast< const PhysicalImageBase< VImageDimension > * >( data );
      }
    catch ( ... )
      {
      return;
      }

    if ( imgData )
      {
      // Copy the meta data for this data type
      this->SetLargestPossibleRegion( imgData->GetLargestPossibleRegion() );
      this->SetOrigin( imgData->GetOrigin() );
      this->SetDirection( imgData->GetDirection() );
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::PhysicalImageBase::CopyInformation() cannot cast "
                         << typeid( data ).name() << " to "
                         << typeid( const PhysicalImageBase * ).name() );
      }
    }
}

/**
 *
 */
template< unsigned int VImageDimension >
void
PhysicalImageBase< VImageDimension >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Origin: " << this->GetOrigin() << std::endl; \

  os << indent << "Direction: " << std::endl << this->GetDirection() << std::endl;

}
} // end namespace itk

#endif
