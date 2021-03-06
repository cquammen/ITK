/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMIRegistrationFunctionTest.cxx
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
#include "itkImage.h"
#include "itkMIRegistrationFunction.h"
#include "itkGaussianImageSource.h"

#include <iostream>

/**
 *  This test uses two 2D-Gaussians (standard deviation RegionSize/2)
 *  One is shifted by 5 pixels from the other.
 *
 *  This test exercises the MIRegistrationFunction 
 *
 */

int itkMIRegistrationFunctionTest(int, char* [] )
{

//------------------------------------------------------------
// Create two simple images
//------------------------------------------------------------

  const unsigned int ImageDimension = 2;

  typedef double                                PixelType;
  typedef itk::Vector<double,ImageDimension>    DeformationPixelType;

  typedef double                   CoordinateRepresentationType;

  //Allocate Images
  typedef itk::Image<PixelType,ImageDimension>         MovingImageType;
  typedef itk::Image<PixelType,ImageDimension>         FixedImageType;
  typedef itk::Image<DeformationPixelType,
                               ImageDimension>         DeformationFieldType;

  // Declare Gaussian Sources
  typedef itk::GaussianImageSource< MovingImageType >  MovingImageSourceType;
  typedef itk::GaussianImageSource< FixedImageType  >  FixedImageSourceType;
  typedef MovingImageSourceType::Pointer               MovingImageSourcePointer;
  typedef FixedImageSourceType::Pointer                FixedImageSourcePointer;

  // Note: the following declarations are classical arrays
  FixedImageType::SizeValueType fixedImageSize[]     = {  100,  100 };
  MovingImageType::SizeValueType movingImageSize[]    = {  100,  100 }; 

  FixedImageType::SpacingValueType fixedImageSpacing[]  = { 1.0f, 1.0f }; 
  MovingImageType::SpacingValueType movingImageSpacing[] = { 1.0f, 1.0f }; 

  FixedImageType::PointValueType fixedImageOrigin[]   = { 0.0f, 0.0f }; 
  MovingImageType::PointValueType movingImageOrigin[]  = { 0.0f, 0.0f }; 

  MovingImageSourceType::Pointer movingImageSource = MovingImageSourceType::New();
  FixedImageSourceType::Pointer  fixedImageSource  = FixedImageSourceType::New();

  fixedImageSource->SetSize(    fixedImageSize    );
  fixedImageSource->SetOrigin(  fixedImageOrigin  );
  fixedImageSource->SetSpacing( fixedImageSpacing );
  fixedImageSource->SetNormalized( false );
  fixedImageSource->SetScale( 250.0f );

  movingImageSource->SetSize(    movingImageSize    );
  movingImageSource->SetOrigin(  movingImageOrigin  );
  movingImageSource->SetSpacing( movingImageSpacing );
  movingImageSource->SetNormalized( false );
  movingImageSource->SetScale( 250.0f );

  movingImageSource->Update();  // Force the filter to run
  fixedImageSource->Update();   // Force the filter to run

  MovingImageType::Pointer movingImage = movingImageSource->GetOutput();
  FixedImageType::Pointer  fixedImage  = fixedImageSource->GetOutput();


//-----------------------------------------------------------
// Set up  the Metric
//-----------------------------------------------------------
  typedef itk::MIRegistrationFunction<  
                                       FixedImageType, 
                                       MovingImageType,
                                       DeformationFieldType >   
                                                MetricFunctionType;

  MetricFunctionType::Pointer  metricFunction = MetricFunctionType::New();


//-----------------------------------------------------------
// Plug the Images into the metricFunction
//-----------------------------------------------------------
  metricFunction->SetFixedImage( fixedImage );
  metricFunction->SetMovingImage( movingImage );


 
  
  std::cout << metricFunction << std::endl;

  return EXIT_SUCCESS;

}

