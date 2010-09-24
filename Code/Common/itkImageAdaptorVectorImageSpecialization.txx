#ifndef __itkImageAdaptorVectorImageSpecialization_txx
#define __itkImageAdaptorVectorImageSpecialization_txx

#include "itkImageAdaptorImageSpecialization.h"


namespace itk
{

/** Specialization for Image type. */

/**
 * Constructor
 */
template< class TPixelType, unsigned int VDimension, class TAccessor >
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::ImageAdaptor()
{
  // Allocate an internal image.  A process object might try to allocate an
  // temporary image that is the same type as its input or output.  If that
  // image type is an adaptor, we need to make sure that an internal image is
  // available because the process object will not know to call SetImage on
  // the adaptor.
  m_Image = InternalImageType::New();
}

/**
 * Destructor
 */
template< class TPixelType, unsigned int VDimension, class TAccessor >
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::~ImageAdaptor()
{}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::Initialize()
{
  // call the superclass' method first; then delegate
  Superclass::Initialize();

  // delegation to internal image
  m_Image->Initialize();
}

template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetPixelContainer(PixelContainer *container)
{
  if ( this->GetPixelContainer() != container )
    {
    m_Image->SetPixelContainer(container);
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
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
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
const typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::OffsetValueType *
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetOffsetTable(void) const
{
  return m_Image->GetOffsetTable();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::IndexType
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::ComputeIndex(OffsetValueType offset) const
{
  return m_Image->ComputeIndex(offset);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::Update()
{
  Superclass::Update();

  m_Image->Update();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::UpdateOutputInformation()
{
  // call the superclass' method first, then delegate
  Superclass::UpdateOutputInformation();

  // delegation to internal image
  m_Image->UpdateOutputInformation();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::UpdateOutputData()
{
  // call the superclass' method first, then delegate
  Superclass::UpdateOutputData();

  // delegation to internal image
  m_Image->UpdateOutputData();
  SetBufferedRegion( m_Image->GetBufferedRegion() );
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::PropagateRequestedRegion()
throw ( InvalidRequestedRegionError )
{
  // call the superclass' method first, then delegate
  Superclass::PropagateRequestedRegion();

  // delegation to internal image
  m_Image->PropagateRequestedRegion();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetRequestedRegionToLargestPossibleRegion()
{
  // call the superclass' method first, then delegate
  Superclass::SetRequestedRegionToLargestPossibleRegion();

  // delegation to internal image
  this->m_Image->SetRequestedRegionToLargestPossibleRegion();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::CopyInformation(const DataObject *data)
{
  // call the superclass' method first, then delegate
  Superclass::CopyInformation(data);

  // delegation to internal image
  this->m_Image->CopyInformation(data);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
const typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::SpacingType &
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetSpacing(void) const
{
  return this->m_Image->GetSpacing();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetSpacing(const SpacingType & spacing)
{
  // delegation to internal image
  this->m_Image->SetSpacing(spacing);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetSpacing(const double *spacing /*[Self::ImageDimension]*/)
{
  // delegation to internal image
  this->m_Image->SetSpacing(spacing);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetSpacing(const float *spacing /*[Self::ImageDimension]*/)
{
  // delegation to internal image
  this->m_Image->SetSpacing(spacing);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetOrigin(const PointType origin)
{
  // delegation to internal image
  this->m_Image->SetOrigin(origin);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetOrigin(const double *origin /*[Self::ImageDimension]*/)
{
  // delegation to internal image
  this->m_Image->SetOrigin(origin);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetOrigin(const float *origin /*[Self::ImageDimension]*/)
{
  // delegation to internal image
  this->m_Image->SetOrigin(origin);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
const typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::PointType &
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetOrigin(void) const
{
  return this->m_Image->GetOrigin();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetDirection(const DirectionType direction)
{
  // delegation to internal image
  this->m_Image->SetDirection(direction);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
const typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::DirectionType &
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetDirection(void) const
{
  return this->m_Image->GetDirection();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetImage(InternalImageType *image)
{
  this->m_Image = image;
  Superclass::SetLargestPossibleRegion( this->m_Image->GetLargestPossibleRegion() );
  Superclass::SetBufferedRegion( this->m_Image->GetBufferedRegion() );
  Superclass::SetRequestedRegion( this->m_Image->GetRequestedRegion() );
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
const typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::InternalPixelType  *
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetBufferPointer() const
{
  return this->m_Image->GetBufferPointer();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::InternalPixelType  *
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetBufferPointer()
{
  return this->m_Image->GetBufferPointer();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::Modified() const
{
  Superclass::Modified();

  this->m_Image->Modified();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
unsigned long
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetMTime() const
{
  unsigned long mtime1, mtime2;

  mtime1 = Superclass::GetMTime();
  mtime2 = this->m_Image->GetMTime();

  return ( mtime1 >= mtime2 ? mtime1 : mtime2 );
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetBufferedRegion(const RegionType & region)
{
  // call the superclass' method first, then delegate
  Superclass::SetBufferedRegion(region);

  // delegation to internal image
  this->m_Image->SetBufferedRegion(region);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
const typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::RegionType &
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetBufferedRegion(void) const
{
  // delegation to internal image
  return this->m_Image->GetBufferedRegion();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetLargestPossibleRegion(const RegionType & region)
{
  // call the superclass' method first, then delegate
  Superclass::SetLargestPossibleRegion(region);

  // delegation to internal image
  this->m_Image->SetLargestPossibleRegion(region);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
const typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::RegionType &
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetLargestPossibleRegion(void) const
{
  // delegation to internal image
  return this->m_Image->GetLargestPossibleRegion();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetRequestedRegion(const RegionType & region)
{
  // call the superclass' method first, then delegate
  Superclass::SetRequestedRegion(region);

  // delegation to internal image
  this->m_Image->SetRequestedRegion(region);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
void
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::SetRequestedRegion(DataObject *data)
{
  // call the superclass' method first, then delegate
  Superclass::SetRequestedRegion(data);

  // delegation to internal image
  this->m_Image->SetRequestedRegion(data);
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
bool
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::VerifyRequestedRegion()
{
  // call the superclass' method first, then delegate
  Superclass::VerifyRequestedRegion();

  // delegation to internal image
  return this->m_Image->VerifyRequestedRegion();
}

//----------------------------------------------------------------------------
template< class TPixelType, unsigned int VDimension, class TAccessor >
const typename ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >::RegionType &
ImageAdaptor< VectorImage< TPixelType, VDimension >, TAccessor >
::GetRequestedRegion(void) const
{
  // delegation to internal image
  return this->m_Image->GetRequestedRegion();
}
} // end namespace itk


#endif // __itkImageAdaptorVectorImageSpecialization_txx
