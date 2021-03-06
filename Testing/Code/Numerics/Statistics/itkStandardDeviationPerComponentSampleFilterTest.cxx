/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkStandardDeviationPerComponentSampleFilterTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkImageToListSampleFilter.h"
#include "itkStandardDeviationPerComponentSampleFilter.h"
#include "itkImageRegionIterator.h"
#include "itkFixedArray.h"
#include "itkVector.h"
#include "itkCovarianceSampleFilter.h"

int itkStandardDeviationPerComponentSampleFilterTest(int, char* [] )
{
  std::cout << "StandardDeviationPerComponentSampleFilter Test \n \n";
  std::string whereFail = "";

  // Now generate an image
  enum { MeasurementVectorSize = 3 };
  typedef float MeasurementType;

  typedef itk::FixedArray< MeasurementType, MeasurementVectorSize > MeasurementVectorType;
  typedef itk::Image< MeasurementVectorType, MeasurementVectorSize > ImageType;
  typedef itk::Image< unsigned char, MeasurementVectorSize >         MaskImageType;

  ImageType::Pointer image = ImageType::New();
  ImageType::RegionType region;
  ImageType::SizeType size;
  ImageType::IndexType index;
  index.Fill(0);
  size.Fill(5);
  region.SetIndex(index);
  region.SetSize(size);


  image->SetBufferedRegion(region);
  image->Allocate();

  typedef itk::ImageRegionIterator< ImageType > ImageIterator;
  ImageIterator iter(image, region);

  unsigned int count = 0;
  MeasurementVectorType temp;
  temp.Fill(0);

  // fill the image
  while (!iter.IsAtEnd())
    {
    temp[0] =     count;
    temp[1] = 2 * count;
    temp[2] = 3 * count;
    iter.Set(temp);
    ++iter;
    ++count;
    }

  // creates an ImageToListAdaptor object
  typedef  itk::Statistics::ImageToListSampleFilter< ImageType, MaskImageType >
                                     ImageToListSampleFilterType;

  ImageToListSampleFilterType::Pointer sampleGeneratingFilter
                            = ImageToListSampleFilterType::New();

  sampleGeneratingFilter->SetInput( image );

  try
    {
    sampleGeneratingFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr<< excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef ImageToListSampleFilterType::ListSampleType                 ListSampleType;
  typedef itk::Statistics::StandardDeviationPerComponentSampleFilter< ListSampleType >         StandardDeviationPerComponentSampleFilterType;

  StandardDeviationPerComponentSampleFilterType::Pointer standardDeviationFilter = StandardDeviationPerComponentSampleFilterType::New();

  std::cout << standardDeviationFilter->GetNameOfClass() << std::endl;

  //Invoke update before adding an input. An exception should be
  try
    {
    standardDeviationFilter->Update();
    std::cerr << "Exception should have been thrown since \
                 Update() is invoked without setting an input " << std::endl;
    return EXIT_FAILURE;
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception caught: " << excp << std::endl;
    }

  standardDeviationFilter->ResetPipeline();

  if ( standardDeviationFilter->GetInput() != NULL )
    {
    std::cerr << "GetInput() should return NULL if the input \
                     has not been set" << std::endl;
    return EXIT_FAILURE;
    }

  standardDeviationFilter->SetInput( sampleGeneratingFilter->GetOutput() );
  try
    {
    standardDeviationFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr<< excp << std::endl;
    return EXIT_FAILURE;
    }

  standardDeviationFilter->Print( std::cout );

  const double   epsilon = 1e-6;

  // CHECK THE RESULTS
  typedef StandardDeviationPerComponentSampleFilterType::MeasurementVectorRealType  MeasurementVectorRealType;
  typedef StandardDeviationPerComponentSampleFilterType::MeasurementVectorRealDecoratedType  MeasurementVectorRealDecoratedType;

  const MeasurementVectorRealDecoratedType * standardDeviationDecorator =
    standardDeviationFilter->GetStandardDeviationPerComponentOutput();

  const MeasurementVectorRealDecoratedType * meanDecorator =
    standardDeviationFilter->GetMeanPerComponentOutput();

  MeasurementVectorRealType    standardDeviation  = standardDeviationDecorator->Get();
  std::cout << "Standard deviation:   " << standardDeviation << std::endl;

  MeasurementVectorRealType    mean  = meanDecorator->Get();
  std::cout << "Mean :   " << mean << std::endl;

  MeasurementVectorRealType    standardDeviation2 = standardDeviationFilter->GetStandardDeviationPerComponent();

  if ( ( vnl_math_abs( standardDeviation[0] - standardDeviation2[0]) > epsilon )  ||
       ( vnl_math_abs( standardDeviation[1] - standardDeviation2[1]) > epsilon )  ||
       ( vnl_math_abs( standardDeviation[2] - standardDeviation2[2]) > epsilon ) )
    {
    std::cerr << "Standard Deviation value retrieved using Get() and the decorator\
                  are not the same:: " <<  standardDeviation << "," << standardDeviation2 << std::endl;
    return EXIT_FAILURE;
    }


  typedef itk::Statistics::CovarianceSampleFilter< ListSampleType > CovarianceSampleFilterType;
  CovarianceSampleFilterType::Pointer covarianceFilter = CovarianceSampleFilterType::New();
  covarianceFilter->SetInput( sampleGeneratingFilter->GetOutput());

  try
    {
    covarianceFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception caught: " << excp << std::endl;
    }

  CovarianceSampleFilterType::MeasurementVectorType meanCalculatedUsingCovarianceSampleFilter = covarianceFilter->GetMean();

  if ( ( vnl_math_abs( meanCalculatedUsingCovarianceSampleFilter[0] - mean[0]) > epsilon )  ||
       ( vnl_math_abs( meanCalculatedUsingCovarianceSampleFilter[1] - mean[1]) > epsilon )  ||
       ( vnl_math_abs( meanCalculatedUsingCovarianceSampleFilter[2] - mean[2]) > epsilon ) )
    {
    std::cerr << "Mean calculated using the CovarianceSampleFilter is different from\
                 the one calculated using the StandardDeviationPerComponentSampleFilter " << std::endl;
    return EXIT_FAILURE;
    }

  CovarianceSampleFilterType::MatrixType covarianceCalculatedUsingCovarianceSampleFilter = covarianceFilter->GetCovarianceMatrix();

  for( unsigned int k = 0; k < MeasurementVectorSize; k++ )
    {
    const double variance = covarianceCalculatedUsingCovarianceSampleFilter(k,k);
    const double standardDeviationValue = vcl_sqrt( variance );

    if ( ( vnl_math_abs( standardDeviationValue - standardDeviation[k] ) > epsilon ) )
      {
      std::cerr << "Standard deviation calculated using the CovarianceSampleFilter";
      std::cerr << " (as the square root of the diagonal) is different from ";
      std::cerr << " the one calculated using the StandardDeviationPerComponentSampleFilter " << std::endl;
      return EXIT_FAILURE;
      }
    }

  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;
}
