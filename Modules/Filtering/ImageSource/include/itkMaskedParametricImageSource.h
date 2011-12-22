/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkMaskedParametricImageSource_h
#define __itkMaskedParametricImageSource_h

#include "itkParametricImageSource.h"

#include "vector"

namespace itk
{

/** \class MaskedParametricImageSource
 * \brief Adapts a ParametricImageSource to treat some parameters as fixed.
 *
 * The MaskedParametricImageSource class is an adapter class that
 * presents a ParametricImageSource as one with potentially fewer
 * parameters. Individual parameters may be enabled or disabled. This
 * capability is useful for using a ParametricImageSource in a parameter
 * estimation routine where some of the parameters should be treated as
 * fixed (disabled parameters) and the others are desired to be estimated
 * (enabled parameters).
 *
 * \ingroup DataSources
 * \ingroup ITKImageSource
*/
template< class TOutputImage >
class ITK_EXPORT MaskedParametricImageSource :
    public ParametricImageSource< TOutputImage >
{
public:
  /** Standard typedefs. */
  typedef MaskedParametricImageSource            Self;
  typedef ParametricImageSource< TOutputImage >  Superclass;
  typedef SmartPointer< Self >                   Pointer;
  typedef SmartPointer< const Self >             ConstPointer;

  /** Output image typedefs */
  typedef TOutputImage                            OutputImageType;
  typedef typename OutputImageType::Pointer       OutputImagePointer;
  typedef typename OutputImageType::PixelType     OutputImagePixelType;
  typedef typename OutputImageType::PixelType     PixelType;
  typedef typename OutputImageType::RegionType    RegionType;
  typedef typename OutputImageType::SpacingType   SpacingType;
  typedef typename OutputImageType::PointType     PointType;
  typedef typename OutputImageType::DirectionType DirectionType;
  typedef typename OutputImageType::SizeType      SizeType;
  typedef typename OutputImageType::SizeValueType SizeValueType;

  typedef typename Superclass::ParametersValueType  ParametersValueType;
  typedef typename Superclass::ParametersType       ParametersType;
  typedef std::vector< bool >                       EnabledArrayType;
  typedef Superclass                                DelegateImageSourceType;
  typedef typename Superclass::Pointer              DelegateImageSourcePointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

   /** ImageDimension constant */
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MaskedParametricImageSource, ParametricImageSource);

  /** Set/get the delegate image source. This is the source that
   * actually produces the image. */
  void SetDelegateImageSource( DelegateImageSourceType * delegateSource );
  itkGetConstObjectMacro(DelegateImageSource, DelegateImageSourceType);

  /** Set the parameters for this source. Setting the parameters does
   * not mark the image source as modified; subclasses should override
   * this method to forward parameters through setters that call
   * Modified(). */
  virtual void SetParameters( const ParametersType & parameters );

  /** Get the parameters for this source. */
  virtual ParametersType GetParameters() const;

  /** Get the number of parameters. */
  virtual unsigned int GetNumberOfParameters() const;

  /** Get the number of parameters in the delegate source. */
  virtual unsigned int GetDelegateNumberOfParameters() const;

  /** Set/get whether a parameter is enabled. When a parameter is
   * disabled, this source reduces the number of parameters it reports
   * by one, and the parameter value in the delegate source cannot be
   * changed through the SetParameters() method. In other words, the
   * parameter is treated as fixed. Contrariwise, when a parameter is
   * enabled, the number of parameters this source reports increases
   * by one, and the parameter value in the delegate source can be
   * changed through the SetParameters() method. */
  virtual void SetParameterEnabled(unsigned int parameterIndex, bool enabled);
  virtual bool GetParameterEnabled(unsigned int parameterIndex) const;

protected:
  MaskedParametricImageSource();
  virtual ~MaskedParametricImageSource();

  /** This filter delegates the image generation, so we override
  GenerateData() here. */
  void GenerateData();

  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  MaskedParametricImageSource(const Self &); // purposely not implemented
  void operator=(const Self &); // purposely not implemented

  DelegateImageSourcePointer m_DelegateImageSource;

  EnabledArrayType m_EnabledArray;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMaskedParametricImageSource.hxx"
#endif

#endif
