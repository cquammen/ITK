/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDirectFourierReconstructionImageToImageFilterTest.cxx
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
#include "itkImageFileReader.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"

#include "itkDirectFourierReconstructionImageToImageFilter.h"

typedef double    InternalPixelType;
typedef short int OutputPixelType;

typedef itk::Image< OutputPixelType, 3 > OutputImageType;
typedef itk::DirectFourierReconstructionImageToImageFilter< InternalPixelType, InternalPixelType > ReconstructionFilterType;
typedef ReconstructionFilterType::OutputImageType InternalImageType;

typedef itk::RecursiveGaussianImageFilter< InternalImageType, InternalImageType > SmootherType;
typedef itk::RescaleIntensityImageFilter< InternalImageType, OutputImageType >    RescalerType;
typedef itk::RegionOfInterestImageFilter< OutputImageType, OutputImageType >      ROIFilterType;
typedef itk::ImageFileReader< InternalImageType >                                 ReaderType;
typedef itk::ImageFileWriter< OutputImageType >                                   WriterType;


class CommandProgressUpdate : public itk::Command
{
public:
  typedef CommandProgressUpdate Self;
  typedef itk::Command          Superclass;
  
  typedef itk::SmartPointer< Self > Pointer;
  
  itkNewMacro( Self );
  
protected:
  CommandProgressUpdate() {}; // purposely not implemented
 
  typedef const ReconstructionFilterType * ReconstructionFilterPointer;
 
  void Execute(itk::Object * caller, const itk::EventObject & event )
    {
    Execute( ( const itk::Object * )caller, event);
    }
  
  void Execute( const itk::Object * caller, const itk::EventObject & event )
    {
    ReconstructionFilterPointer reconstructor = dynamic_cast< ReconstructionFilterPointer >( caller );
   
    if ( ! itk::ProgressEvent().CheckEvent( &event ) )
      {
      return;
      }
   
    std::cout << (int)( 100 * reconstructor->GetProgress() ) << "%" << std::endl;
    }
};


int itkDirectFourierReconstructionImageToImageFilterTest (int argc, char * argv[] )
{

  if ( argc != 18)
    {
    std::cerr << "Wrong number of input arguments" << std::endl;
    std::cerr << "Usage : " << std::endl << "\t";
    std::cerr << argv[0] << " input output r_dir z_dir alpha_dir nz ng fc nb alpha_range x y z sx sy sz sigma" << std::endl;
    return 1;
    }

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
 
 
  SmootherType::Pointer smoother = SmootherType::New();
  smoother->SetInput( reader->GetOutput() );
  smoother->SetSigma( atof( argv[17] ) );
  smoother->SetDirection( atoi( argv[3] ) );
 
 
  ReconstructionFilterType::Pointer reconstruct = ReconstructionFilterType::New();
  if ( atof( argv[17] ) == 0 ) 
    {
    reconstruct->SetInput( reader->GetOutput() );
    }
  else 
    {
    reconstruct->SetInput( smoother->GetOutput() );
    }
  reconstruct->SetRDirection( atoi( argv[3] ) );
  reconstruct->SetZDirection( atoi( argv[4] ) );
  reconstruct->SetAlphaDirection( atoi( argv[5] ) );
  reconstruct->SetZeroPadding( atoi( argv[6] ) );
  reconstruct->SetOverSampling( atoi( argv[7] ) );
  reconstruct->SetCutoff( atof( argv[8] ) );
  reconstruct->SetRadialSplineOrder( atoi( argv[9] ) );
  reconstruct->SetAlphaRange( atoi( argv[10] ) );

  CommandProgressUpdate::Pointer observer = CommandProgressUpdate::New();
  reconstruct->AddObserver( itk::ProgressEvent(), observer ); 
 
 
 
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetInput( reconstruct->GetOutput() );
  rescaler->SetOutputMinimum( itk::NumericTraits< OutputPixelType >::min() );
  rescaler->SetOutputMaximum( itk::NumericTraits< OutputPixelType >::max() );
 
 
 
  ROIFilterType::Pointer ROIFilter = ROIFilterType::New();
  ROIFilter->SetInput( rescaler->GetOutput() );
 
  ROIFilterType::IndexType start;
  ROIFilterType::SizeType size;
 
  start[0] = atoi( argv[11] );
  start[1] = atoi( argv[12] );
  start[2] = atoi( argv[13] );
 
  size[0] = atoi( argv[14] );
  size[1] = atoi( argv[15] );
  size[2] = atoi( argv[16] );

  ROIFilterType::RegionType requestedRegion;  
  requestedRegion.SetIndex( start );
  requestedRegion.SetSize( size );
 
  ROIFilter->SetRegionOfInterest( requestedRegion );
 
 
 
 
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->UseCompressionOn(  );
  writer->SetInput( ROIFilter->GetOutput() );

 
 
 
 
 
  try 
    {
    writer->Update(); 
    } 
  catch ( itk::ExceptionObject err )
    {
    std::cerr << "An error occurred somewhere:" << std::endl;
    std::cerr << err << std::endl;
    return 2;
    }

  std::cout << "Done" << std::endl;
 
  std::cout << reconstruct << std::endl;

  return 0;

} // main
