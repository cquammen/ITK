/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRegularImage.txx
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
#ifndef __itkRegularImage_txx
#define __itkRegularImage_txx

#include "itkRegularImage.h"
#include "itkProcessObject.h"

namespace itk
{
/**
 *
 */
template< class TPixel, unsigned int VImageDimension >
RegularImage< TPixel, VImageDimension >
::RegularImage()
{
  m_Buffer = PixelContainer::New();
}

//----------------------------------------------------------------------------
template< class TPixel, unsigned int VImageDimension >
void
RegularImage< TPixel, VImageDimension >
::Allocate()
{
  unsigned long num;

  this->ComputeOffsetTable();
  num = this->GetOffsetTable()[VImageDimension];

  m_Buffer->Reserve(num);
}

template< class TPixel, unsigned int VImageDimension >
void
RegularImage< TPixel, VImageDimension >
::Initialize()
{
  //
  // We don't modify ourselves because the "ReleaseData" methods depend upon
  // no modification when initialized.
  //

  // Call the superclass which should initialize the BufferedRegion ivar.
  Superclass::Initialize();

  // Replace the handle to the buffer. This is the safest thing to do,
  // since the same container can be shared by multiple images (e.g.
  // Grafted outputs and in place filters).
  m_Buffer = PixelContainer::New();
}

template< class TPixel, unsigned int VImageDimension >
void
RegularImage< TPixel, VImageDimension >
::FillBuffer(const TPixel & value)
{
  const SizeValueType numberOfPixels =
    this->GetBufferedRegion().GetNumberOfPixels();

  for ( SizeValueType i = 0; i < numberOfPixels; i++ )
    {
    ( *m_Buffer )[i] = value;
    }
}

template< class TPixel, unsigned int VImageDimension >
void
RegularImage< TPixel, VImageDimension >
::SetPixelContainer(PixelContainer *container)
{
  if ( m_Buffer != container )
    {
    m_Buffer = container;
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template< class TPixel, unsigned int VImageDimension >
void
RegularImage< TPixel, VImageDimension >
::Graft(const DataObject *data)
{
  // call the superclass' implementation
  Superclass::Graft(data);

  if ( data )
    {
    // Attempt to cast data to an RegularImage
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
      this->SetPixelContainer( const_cast< PixelContainer * >
                               ( imgData->GetPixelContainer() ) );
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::RegularImage::Graft() cannot cast "
                         << typeid( data ).name() << " to "
                         << typeid( const Self * ).name() );
      }
    }
}

//----------------------------------------------------------------------------
template< class TPixel, unsigned int VImageDimension >
void
RegularImage< TPixel, VImageDimension >
::ComputeIndexToPhysicalPointMatrices()
{
  this->Superclass::ComputeIndexToPhysicalPointMatrices();
}

/**
 *
 */
template< class TPixel, unsigned int VImageDimension >
void
RegularImage< TPixel, VImageDimension >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "PixelContainer: " << std::endl;
  m_Buffer->Print( os, indent.GetNextIndent() );

  // m_Origin and m_Spacing are printed in the Superclass
}
} // end namespace itk

#endif
