/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRectilinearImageTest.cxx
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

#include <iostream>
#include "itkRectilinearImage.h"

int itkRectilinearImageTest(int, char* [] )
{

  typedef itk::RectilinearImage<float,2> Image;
  Image::Pointer image = Image::New();
  image->DebugOn();
  image->GetSource();
  image->DisconnectPipeline();

  Image::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10);
  region.SetSize(1, 10);
  image->SetRegions(region);

  Image::DefaultSpacingType spacing; spacing.Fill(1.0);
  Image::PointType origin; origin.Fill(1.0);
  Image::DirectionType direction;
  direction[0][0] = .5;
  direction[0][1] = .7;
  direction[1][0] = .7;
  direction[1][1] = .5;
  image->SetDefaultSpacing (spacing);
  image->SetOrigin (origin);
  image->SetDirection (direction);

  double dspacing[Image::ImageDimension] = {2.0, 2.0};
  double dorigin[Image::ImageDimension] = {2.0, 2.0};
  image->SetDefaultSpacing (dspacing);
  image->SetOrigin (dorigin);

  float fspacing[Image::ImageDimension] = {3.0, 3.0};
  float forigin[Image::ImageDimension] = {3.0, 3.0};
  image->SetDefaultSpacing (fspacing);
  image->SetOrigin (forigin);

  for ( unsigned int dim = 0; dim < Image::ImageDimension; dim++ )
    {
    for ( unsigned int j = 0; j < image->GetLargestPossibleRegion().GetSize()[dim]; j++ )
      {
      image->SetSpacing(dim, j, static_cast<double>(j)+1.0);
      }
    }

  image->Print(std::cout);

  direction.SetIdentity();
  image->SetDirection(direction);
  origin.Fill(0.0);
  image->SetOrigin(origin);
  spacing.Fill(1.0);
  image->SetDefaultSpacing(spacing);
  //image->InitializeWithDefaultSpacings();

  image->Print(std::cout);

  double coords[] = {-2.8, -1.8, -0.8, -0.2, -0.4, 0.5, 0.51, 1.5, 2.4, 2.5,
                     2.6, 4.0, 5.4, 5.5, 5.6, 40.0, 50.0, 52.5, 53.5, 54.5,
                     55.5, 56.5, 57.5, 58.5, 59.5, 60.5, 61.5, 62.5, 63.5,
                     64.5, 65.5, 66.5, 67.5, 68.5, 69.5, 70.5, 71.5};

  itk::Point< double, Image::ImageDimension > point;
  Image::IndexType index;
  itk::ContinuousIndex< double, Image::ImageDimension > cindex;

  for ( unsigned int i = 0; i < sizeof(coords) / sizeof(double); i++ )
    {

    point.Fill(coords[i]);
    image->TransformPhysicalPointToIndex(point, index);
    std::cout << "Point " << point << " maps to index " << index << std::endl;

    point.Fill(coords[i]);
    image->TransformPhysicalPointToContinuousIndex(point, cindex);
    std::cout << "Point " << point << " maps to continuous index " << cindex << std::endl;
    }

  return (EXIT_SUCCESS);
}
