/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkReviewPrintTest.cxx
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

#include "itkContourExtractor2DImageFilter.h"
#include "itkLabelToRGBImageFilter.h"
#include "itkLabelOverlayImageFilter.h"
#include "itkRGBPixel.h"
#include "itkQuadEdgeMesh.h"

#include "itkValuedRegionalMinimaImageFilter.h"
#include "itkValuedRegionalMaximaImageFilter.h"
#include "itkRegionalMaximaImageFilter.h"
#include "itkRegionalMinimaImageFilter.h"

#include "itkNeuralNetworkFileReader.h"
#include "itkNeuralNetworkFileWriter.h"

#include "itkConformalFlatteningMeshFilter.h"

#include "itkVTKPolyDataReader.h"
#include "itkVTKPolyDataWriter.h"

#include "itkImageKernelOperator.h"

#include "itkHessianToObjectnessMeasureImageFilter.h"
#include "itkMultiScaleHessianBasedMeasureImageFilter.h"

int main(int , char* [])
{
  typedef itk::Image<float,2>           Input2DImageType;
  typedef itk::Image<float,2>           OutputType;
  typedef itk::Image<unsigned char,2>   CharType;
  typedef itk::RGBPixel<unsigned char>  RGBPixelType;
  typedef itk::Image< RGBPixelType, 2 > RGBImageType;

  typedef itk::Vector<float,2>      VectorType;
  typedef itk::Image<VectorType, 2> VectorImageType;

  typedef itk::QuadEdgeMesh< double, 3 > QuadEdgeMeshType;

  typedef itk::Vector<double, 2> MeasurementVectorType;
  typedef itk::Vector<double, 1> TargetVectorType;

  typedef unsigned short         PixelType;
  typedef itk::Mesh< float, 3 >  MeshType;

  typedef itk::NumericTraits< PixelType >::RealType                 RealPixelType;
  typedef itk::SymmetricSecondRankTensor< RealPixelType, 3 >        HessianPixelType;
  typedef itk::Image< HessianPixelType, 3         >                 HessianImageType;
  typedef itk::Image< PixelType, 3 >                                Input3DImageType;
  typedef itk::Image< PixelType, 3 >                                OutputImageType;

  // Dummy variable just to force the full instantiation of the class
  CharType::Pointer dummyImage = CharType::New();

  itk::ContourExtractor2DImageFilter<Input2DImageType>::Pointer
    ContourExtractor2DImageFilterObj =
    itk::ContourExtractor2DImageFilter<Input2DImageType>::New();
  std:: cout << "-------------ContourExtractor2DImageFilter "
             << ContourExtractor2DImageFilterObj;

  itk::LabelToRGBImageFilter<CharType,RGBImageType>::Pointer
    LabelToRGBImageFilterObj =
    itk::LabelToRGBImageFilter<CharType,RGBImageType>::New();
  std:: cout << "-------------LabelToRGBImageFilter "
             << LabelToRGBImageFilterObj;

  itk::LabelOverlayImageFilter<Input2DImageType,CharType,RGBImageType>::Pointer
    LabelOverlayImageFilterObj =
    itk::LabelOverlayImageFilter<Input2DImageType,CharType,RGBImageType>::New();
  std:: cout << "-------------LabelOverlayImageFilter "
             << LabelOverlayImageFilterObj;

  QuadEdgeMeshType::Pointer QuadEdgeMeshObj = QuadEdgeMeshType::New();
  std:: cout << "-------------QuadEdgeMesh "
             << QuadEdgeMeshObj;

  itk::ValuedRegionalMaximaImageFilter<Input2DImageType,Input2DImageType>::Pointer
    ValuedRegionalMaximaImageFilterObj =
    itk::ValuedRegionalMaximaImageFilter<Input2DImageType,Input2DImageType>::New();
  std:: cout << "-------------ValuedRegionalMaximaImageFilterObj "
             << ValuedRegionalMaximaImageFilterObj;

  itk::ValuedRegionalMinimaImageFilter<Input2DImageType,Input2DImageType>::Pointer
    ValuedRegionalMinimaImageFilterObj =
    itk::ValuedRegionalMinimaImageFilter<Input2DImageType,Input2DImageType>::New();
  std:: cout << "-------------ValuedRegionalMinimaImageFilterObj "
             << ValuedRegionalMinimaImageFilterObj;

  itk::RegionalMaximaImageFilter<Input2DImageType,Input2DImageType>::Pointer
    RegionalMaximaImageFilterObj =
    itk::RegionalMaximaImageFilter<Input2DImageType,Input2DImageType>::New();
  std:: cout << "-------------RegionalMaximaImageFilterObj "
             << RegionalMaximaImageFilterObj;

  itk::RegionalMinimaImageFilter<Input2DImageType,Input2DImageType>::Pointer
    RegionalMinimaImageFilterObj =
    itk::RegionalMinimaImageFilter<Input2DImageType,Input2DImageType>::New();
  std:: cout << "-------------RegionalMinimaImageFilterObj "
             << RegionalMinimaImageFilterObj;

  itk::NeuralNetworkFileReader<itk::Statistics::OneHiddenLayerBackPropagationNeuralNetwork<MeasurementVectorType, TargetVectorType> >::Pointer
    NeuralNetworkFileReaderObj =
              itk::NeuralNetworkFileReader<itk::Statistics::OneHiddenLayerBackPropagationNeuralNetwork<MeasurementVectorType, TargetVectorType> >::New();
  std:: cout << "-------------NeuralNetworkFileReaderObj "
             << NeuralNetworkFileReaderObj;

  itk::NeuralNetworkFileWriter<itk::Statistics::OneHiddenLayerBackPropagationNeuralNetwork<MeasurementVectorType, TargetVectorType> >::Pointer
    NeuralNetworkFileWriterObj =
              itk::NeuralNetworkFileWriter<itk::Statistics::OneHiddenLayerBackPropagationNeuralNetwork<MeasurementVectorType, TargetVectorType> >::New();
  std:: cout << "-------------NeuralNetworkFileWriterObj "
             << NeuralNetworkFileWriterObj;

  itk::ConformalFlatteningMeshFilter<MeshType, MeshType>::Pointer
    ConformalFlatteningMeshFilterObj =
    itk::ConformalFlatteningMeshFilter<MeshType, MeshType>::New();
  std::cout << "--------------ConformalFlatteningMeshFilterObj "
            << ConformalFlatteningMeshFilterObj;

  itk::VTKPolyDataReader<MeshType>::Pointer VTKPolyDataReaderObj =
    itk::VTKPolyDataReader<MeshType>::New();
  std::cout << "--------------VTKPolyDataReaderObj "
           << VTKPolyDataReaderObj;

  itk::VTKPolyDataWriter<MeshType>::Pointer VTKPolyDataWriterObj =
    itk::VTKPolyDataWriter<MeshType>::New();
  std::cout << "--------------VTKPolyDataWriterObj "
            << VTKPolyDataWriterObj;

  itk::ImageKernelOperator<float> kernelOperator;
  std::cout << "--------------ImageKernelOperatorObj ";
  kernelOperator.Print(std::cout);

  itk::HessianToObjectnessMeasureImageFilter< HessianImageType,OutputImageType >::Pointer 
              ObjectnessFilterObject = 
        itk::HessianToObjectnessMeasureImageFilter< HessianImageType,OutputImageType >::New(); 

  std::cout << "---------------------------------ObjectnessFilterObject "
                   << ObjectnessFilterObject;
 
  itk::MultiScaleHessianBasedMeasureImageFilter< Input3DImageType ,HessianImageType, OutputImageType >::Pointer
               MultiScaleHessianFilter = 
        itk::MultiScaleHessianBasedMeasureImageFilter< Input3DImageType ,HessianImageType, OutputImageType >::New();

  return EXIT_SUCCESS;
}
