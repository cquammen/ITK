#ifndef __itkImageAdaptorRectilinearImageSpecialization_txx
#define __itkImageAdaptorRectilinearImageSpecialization_txx

#include "itkImageAdaptorRectilinearImageSpecialization.h"


namespace itk
{

/** Specialization for Image type. */

/**
 * Constructor
 */
template< class TPixelType, unsigned int VDimension, class TAccessor >
ImageAdaptor< RectilinearImage< TPixelType, VDimension >, TAccessor >
::ImageAdaptor()
{
}

/**
 * Destructor
 */
template< class TPixelType, unsigned int VDimension, class TAccessor >
ImageAdaptor< RectilinearImage< TPixelType, VDimension >, TAccessor >
::~ImageAdaptor()
{}


//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< RectilinearImage< TPixelType, VDimension >, TAccessor >
::Graft(const DataObject *data)
{
  // call the superclass' implementation
  Superclass::Graft(data);

  if ( data )
    {
    // Attempt to cast data to an ImageAdaptor
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
      itkExceptionMacro( << "itk::ImageAdaptor::Graft() cannot cast "
                         << typeid( data ).name() << " to "
                         << typeid( const Self * ).name() );
      }
    }
}

/**
 *
 */
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< RectilinearImage< TPixelType, VDimension >, TAccessor >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk


#endif
