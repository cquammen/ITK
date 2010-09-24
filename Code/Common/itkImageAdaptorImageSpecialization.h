#ifndef __itkImageAdaptorImageSpecialization_h
#define __itkImageAdaptorImageSpecialization_h

#include "itkImage.h"

#include "itkImageAdaptor.h"
#include "itkRegularImageAdaptor.h"

namespace itk
{

/** Specialization for Image class. */

/**
 * \class ImageAdaptor
 * \brief Give access to partial aspects of voxels from an Image
 *
 * ImageAdaptors are templated over the ImageType and over a functor
 * that will specify what part of the pixel can be accessed
 *
 * The basic aspects of this class are the types it defines.
 *
 * Image adaptors can be used as intermediate classes that allow
 * the sending of an image to a filter, specifying what part of the
 * image pixels the filter will act on.
 *
 * The TAccessor class should implement the Get and Set methods
 * These two will specify how data can be put
 * and get from parts of each pixel. It should define the types
 * ExternalType and InternalType too.
 *
 * \ingroup ImageAdaptors
 *
 */
template< class TPixelType, unsigned int VDimension, class TAccessor >
class ITK_EXPORT ImageAdaptor< Image< TPixelType, VDimension >, TAccessor >
  : public RegularImageAdaptor< Image< TPixelType, VDimension >, TAccessor >
{
public:
  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel
   * type and dimension) when they need compile time access to the dimension
   * of the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, VDimension);

  /** Standard class typedefs. */
  typedef ImageAdaptor                                                      Self;
  typedef RegularImageAdaptor< Image< TPixelType, VDimension >, TAccessor > Superclass;
  typedef SmartPointer< Self >                                              Pointer;
  typedef SmartPointer< const Self >                                        ConstPointer;
  typedef WeakPointer< const Self >                                         ConstWeakPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageAdaptor, RegularImageAdaptor);

  /** Typedef of unadapted image */
  typedef Image< TPixelType, VDimension > InternalImageType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Pixel typedef support. Used to declare pixel type in filters
   * or other operations. */
  typedef typename TAccessor::ExternalType PixelType;

  /** Pixel typedef support. Used to declare pixel type in filters
   * or other operations. */
  typedef typename TAccessor::InternalType InternalPixelType;

  typedef PixelType IOPixelType;

  /**  Accessor type that convert data between internal and external
   *  representations. */
  typedef   TAccessor AccessorType;

  /** typedef of the functor that chooses the appropriate accessor
   * Image or VectorImage. */
  typedef DefaultPixelAccessorFunctor< Self > AccessorFunctorType;

  /** Index typedef support. An index is used to access pixel values. */
  typedef typename Superclass::IndexType     IndexType;
  typedef typename IndexType::IndexValueType IndexValueType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef typename Superclass::SizeType    SizeType;
  typedef typename SizeType::SizeValueType SizeValueType;

  /** Offset typedef support. */
  typedef typename Superclass::OffsetType      OffsetType;
  typedef typename OffsetType::OffsetValueType OffsetValueType;

  /** Region typedef support. A region is used to specify a subset of
   *  an image. */
  typedef typename Superclass::RegionType RegionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. */
  typedef typename Superclass::SpacingType SpacingType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0,0). */
  typedef typename Superclass::PointType PointType;

  /** Direction typedef support.  The Direction is a matix of
   * direction cosines that specify the direction between samples.
   * */
  typedef typename Superclass::DirectionType DirectionType;

  /** PixelContainer typedef support. Used to construct a container for
   * the pixel data. */
  typedef typename InternalImageType::PixelContainer             PixelContainer;
  typedef typename InternalImageType::PixelContainerPointer      PixelContainerPointer;
  typedef typename InternalImageType::PixelContainerConstPointer PixelContainerConstPointer;

  /** Graft the data and information from one image to another. This
   * is a convenience method to setup a second image with all the meta
   * information of another image and use the same pixel
   * container. Note that this method is different than just using two
   * SmartPointers to the same image since separate DataObjects are
   * still maintained. This method is similar to
   * ImageSource::GraftOutput(). The implementation in ImageBase
   * simply calls CopyInformation() and copies the region ivars.
   * The implementation here refers to the superclass' implementation
   * and then copies over the pixel container. */
  virtual void Graft(const DataObject *data);

protected:

  ImageAdaptor();
  virtual ~ImageAdaptor();
  void PrintSelf(std::ostream & os, Indent indent) const;

private:

  ImageAdaptor(const Self &);   //purposely not implemented
  void operator=(const Self &); //purposely not implemented

};
} // end namespace itk

#include "itkImageAdaptorImageSpecialization.txx"

#endif // __itkImageAdaptorImageSpecialization_h
