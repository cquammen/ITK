/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDivideImageFilterTest2.cxx
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




#include "itkVectorImage.h"
#include "itkDivideImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"


int itkDivideImageFilterTest2(int, char* [] ) 
{

  // Define the dimension of the images
  const unsigned int myDimension = 3;
  typedef float      myElementPixelType;

  // Declare the types of the images
  typedef itk::VectorImage<myElementPixelType, myDimension>  myImageType1;
  typedef itk::Image<myElementPixelType, myDimension>        myImageType2;
  typedef itk::VectorImage<myElementPixelType, myDimension>  myImageType3;

  // Declare the type of the index to access images
  typedef itk::Index<myDimension>         myIndexType;

  // Declare the type of the size 
  typedef itk::Size<myDimension>          mySizeType;

  // Declare the type of the Region
  typedef itk::ImageRegion<myDimension>        myRegionType;

  // Declare the type for the Divide filter
  typedef itk::DivideImageFilter<
                                myImageType1,
                                myImageType2,
                                myImageType3  >       myFilterType;
 
  // Declare the pointers to images
  typedef myImageType1::Pointer   myImageType1Pointer;
  typedef myImageType2::Pointer   myImageType2Pointer;
  typedef myImageType3::Pointer   myImageType3Pointer;
  typedef myFilterType::Pointer   myFilterTypePointer;

  // Create two images
  myImageType1Pointer inputImageA  = myImageType1::New();
  myImageType2Pointer inputImageB  = myImageType2::New();
  
  // Define their size, and start index
  mySizeType size;
  size[0] = 2;
  size[1] = 2;
  size[2] = 2;

  myIndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;

  myRegionType region;
  region.SetIndex( start );
  region.SetSize( size );

  // Initialize Image A
  inputImageA->SetLargestPossibleRegion( region );
  inputImageA->SetBufferedRegion( region );
  inputImageA->SetRequestedRegion( region );
  inputImageA->SetNumberOfComponentsPerPixel( 4 );
  inputImageA->Allocate();

  // Initialize Image B
  inputImageB->SetLargestPossibleRegion( region );
  inputImageB->SetBufferedRegion( region );
  inputImageB->SetRequestedRegion( region );
  inputImageB->Allocate();


  // Declare Iterator types apropriated for each image 
  typedef itk::ImageRegionIteratorWithIndex<myImageType1>  myIteratorType1;
  typedef itk::ImageRegionIteratorWithIndex<myImageType2>  myIteratorType2;
  typedef itk::ImageRegionIteratorWithIndex<myImageType3>  myIteratorType3;

  // Create one iterator for Image A (this is a light object)
  myIteratorType1 it1( inputImageA, inputImageA->GetBufferedRegion() );

  myImageType1::PixelType valueA( inputImageA->GetNumberOfComponentsPerPixel() );
  valueA.Fill( 2.0 );

  // Initialize the content of Image A
  std::cout << "First operand " << std::endl;
  while( !it1.IsAtEnd() ) 
  {
    it1.Set( valueA );
    std::cout << it1.Get() << std::endl;
    ++it1;
  }

  // Create one iterator for Image B (this is a light object)
  myIteratorType2 it2( inputImageB, inputImageB->GetBufferedRegion() );

  // Initialize the content of Image B
  std::cout << "Second operand " << std::endl;
  while( !it2.IsAtEnd() ) 
  {
    it2.Set( 3.0 );
    std::cout << it2.Get() << std::endl;
    ++it2;
  }
           

  // Create an Divide Filter                                
  myFilterTypePointer filter = myFilterType::New();


  // Connect the input images
  filter->SetInput1( inputImageA ); 
  filter->SetInput2( inputImageB );

  // Get the Smart Pointer to the Filter Output 
  myImageType3Pointer outputImage = filter->GetOutput();

  
  // Execute the filter
  filter->Update();
  filter->SetFunctor(filter->GetFunctor());

  // Create an iterator for going through the image output
  myIteratorType3 it3(outputImage, outputImage->GetBufferedRegion());
  
  //  Print the content of the result image
  std::cout << " Result " << std::endl;
  while( !it3.IsAtEnd() ) 
  {
    std::cout << it3.Get() << std::endl;
    ++it3;
  }


  // All objects should be automatically destroyed at this point
  return EXIT_SUCCESS;

}




