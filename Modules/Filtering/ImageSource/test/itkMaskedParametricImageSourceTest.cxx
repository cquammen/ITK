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

#include "itkMaskedParametricImageSource.h"

#include "itkGaussianImageSource.h"
#include "itkTestingMacros.h"

int itkMaskedParametricImageSourceTest( int, char *[] )
{
  const unsigned int Dimension = 2;

  typedef float                                         PixelType;
  typedef itk::Image< PixelType, Dimension >            ImageType;
  typedef itk::GaussianImageSource< ImageType >         DelegateSourceType;
  typedef itk::MaskedParametricImageSource< ImageType > MaskedSourceType;

  // Set up a delegate parametric image source
  DelegateSourceType::Pointer delegateSource = DelegateSourceType::New();

  // Set up the masked image source
  MaskedSourceType::Pointer maskedSource = MaskedSourceType::New();
  maskedSource->SetDelegateImageSource( delegateSource );

  MaskedSourceType::SizeType sourceSize = {{ 100, 100 }};
  maskedSource->SetSize( sourceSize );
  TEST_SET_GET_VALUE( maskedSource->GetSize(), sourceSize );

  MaskedSourceType::SpacingType sourceSpacing;
  sourceSpacing[0] = 1.0;
  sourceSpacing[1] = 1.0;
  maskedSource->SetSpacing( sourceSpacing );
  TEST_SET_GET_VALUE( maskedSource->GetSpacing(), sourceSpacing );

  MaskedSourceType::PointType sourceOrigin;
  sourceOrigin[0] = 0.0;
  sourceOrigin[1] = 0.0;
  maskedSource->SetOrigin( sourceOrigin );
  TEST_SET_GET_VALUE( maskedSource->GetOrigin(), sourceOrigin );

  MaskedSourceType::DirectionType sourceDirection;
  sourceDirection.SetIdentity();
  maskedSource->SetDirection( sourceDirection );
  TEST_SET_GET_VALUE( maskedSource->GetDirection(), sourceDirection );

  unsigned int delegateNumberOfParameters = delegateSource->GetNumberOfParameters();
  TEST_SET_GET_VALUE( maskedSource->GetDelegateNumberOfParameters(),
                      delegateNumberOfParameters );

  // Test setting the parameters
  MaskedSourceType::ParametersType parameters( maskedSource->GetNumberOfParameters() );
  parameters[0] = 2.2;
  parameters[1] = 1.3;
  parameters[2] = 5.0;
  parameters[3] = -3.3;
  parameters[4] = 0.345;

  maskedSource->SetParameters( parameters );
  DelegateSourceType::ParametersType delegateParameters =
    delegateSource->GetParameters();
  for ( unsigned int i = 0; i < parameters.Size(); ++i )
    {
    if ( delegateParameters[i] != parameters[i] )
      {
      std::cerr << "ERROR: Delegate parameter " << i
                << " does not match parameter that was set when all "
                << "parameters are enabled." << std::endl;
      std::cerr << "Masked parameters: " << parameters << std::endl
                << "Delegate parameters: " << delegateParameters << std::endl;
      return EXIT_FAILURE;
      }
    }

  // Test parameter enabling/disabling

  // All parameters should be enabled by default
  TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                      delegateNumberOfParameters );

  maskedSource->SetDelegateImageSource( NULL );
  TEST_SET_GET_VALUE( maskedSource->GetDelegateImageSource(), NULL );

  // There should be no parameters if the delegate is not set
  TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(), 0 );

  // All parameters should be enabled by default after the delegate is changed
  maskedSource->SetDelegateImageSource( delegateSource );
  TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                      delegateNumberOfParameters );

  // Now disable parameters one by one from the beginning of the
  // parameter array and check for correctness.
  for ( unsigned int i = 0; i < delegateNumberOfParameters; ++i )
    {
    TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                        (delegateNumberOfParameters - i) );
    maskedSource->SetParameterEnabled( i, false );
    TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                        (delegateNumberOfParameters - 1 - i) );

    // Now set the parameters for the masked source and check that
    // they are set properly in the delegate source
    parameters = MaskedSourceType::ParametersType( delegateNumberOfParameters - 1 - i );

    for ( unsigned int j = 0; j < delegateNumberOfParameters - 1 - i; ++j )
      {
      parameters[j] = static_cast< MaskedSourceType::ParametersValueType >( j );
      }

    maskedSource->SetParameters( parameters );
    delegateParameters = delegateSource->GetParameters();

    for ( unsigned int j = 0; j < delegateNumberOfParameters - 1 - i; ++j )
      {
      if ( parameters[j] != delegateParameters[j + i + 1] )
        {
        std::cerr << "ERROR: Delegate parameter " << j
                  << " does not match parameter that was set when testing"
                  << " parameter disabling from the front. Disabled index " << i
                  << ", delegate index " << j << std::endl;

        std::cerr << "Masked parameters: " << parameters << std::endl
                  << "Delegate parameters: " << delegateParameters << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  // Re-enable all parameters
  for ( unsigned int i = 0; i < delegateNumberOfParameters; ++i )
    {
    maskedSource->SetParameterEnabled( i, true );
    }

  // Now disable parameters one by one from the end of the
  // parameter array and check for correctness.
  for ( unsigned int i = 0; i < delegateNumberOfParameters; ++i )
    {
    TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                        (delegateNumberOfParameters - i) );
    maskedSource->SetParameterEnabled( delegateNumberOfParameters - 1 - i, false );
    TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                        (delegateNumberOfParameters - 1 - i) );

    // Now set the parameters for the masked source and check that
    // they are set properly in the delegate source
    parameters = MaskedSourceType::ParametersType( delegateNumberOfParameters - 1 - i );

    for ( unsigned int j = 0; j < delegateNumberOfParameters - 1 - i; ++j )
      {
      parameters[j] = static_cast< MaskedSourceType::ParametersValueType >( j );
      }

    maskedSource->SetParameters( parameters );
    delegateParameters = delegateSource->GetParameters();

    for ( unsigned int j = 0; j < delegateNumberOfParameters - 1 - i; ++j )
      {
      if ( parameters[j] != delegateParameters[j] )
        {
        std::cerr << "ERROR: Delegate parameter " << j
                  << " does not match parameter that was set when testing"
                  << " parameter disabling from the back. Disabled index " << i
                  << ", delegate index " << j << std::endl;

        std::cerr << "Masked parameters: " << parameters << std::endl
                  << "Delegate parameters: " << delegateParameters << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  // Re-enable all parameters
  for ( unsigned int i = 0; i < delegateNumberOfParameters; ++i )
    {
    maskedSource->SetParameterEnabled( i, true );
    }

  // Now disable one by one from the beginning of the parameter array,
  // test, then re-enable at the end of the iteration
  for ( unsigned int i = 0; i < delegateNumberOfParameters; ++i )
    {
    TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                        delegateNumberOfParameters );
    maskedSource->SetParameterEnabled( i, false );
    TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                        (delegateNumberOfParameters - 1) );

    // Now set the parameters for the masked source and check that
    // they are set properly in the delegate source
    parameters = MaskedSourceType::ParametersType( delegateNumberOfParameters - 1 );

    for ( unsigned int j = 0; j < delegateNumberOfParameters - 1; ++j )
      {
      parameters[j] = static_cast< MaskedSourceType::ParametersValueType >( j );
      }

    maskedSource->SetParameters( parameters );
    delegateParameters = delegateSource->GetParameters();

    for ( unsigned int j = 0; j < delegateNumberOfParameters - 1; ++j )
      {
      unsigned int parameterIndex = j;
      if ( j >= i )
        {
        parameterIndex++;
        }
      if ( delegateParameters[parameterIndex] != parameters[j] )
        {
        std::cerr << "ERROR: Delegate parameter " << j
                  << " does not match parameter that was set when testing"
                  << " parameter disabling one by one. Disabled index " << i
                  << ", delegate index " << j << std::endl;

        std::cerr << "Masked parameters: " << parameters << std::endl
                  << "Delegate parameters: " << delegateParameters << std::endl;
        return EXIT_FAILURE;
        }
      }

    // Now re-enable the parameter
    maskedSource->SetParameterEnabled( i, true );
    TEST_SET_GET_VALUE( maskedSource->GetNumberOfParameters(),
                        delegateNumberOfParameters );
    }

  maskedSource->UpdateLargestPossibleRegion();

  maskedSource->Print( std::cout );

  return EXIT_SUCCESS;
}
