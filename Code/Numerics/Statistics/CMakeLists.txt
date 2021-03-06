# Sources for ITKStatisticsNew library

add_library(ITKStatistics
  itkDenseFrequencyContainer2.cxx
  itkSparseFrequencyContainer2.cxx
  itkDecisionRule.cxx
  itkMaximumDecisionRule2.cxx
  itkMaximumRatioDecisionRule2.cxx
  itkMinimumDecisionRule2.cxx
  itkNormalVariateGenerator.cxx

  # Distributions
  itkGaussianDistribution.cxx
  itkTDistribution.cxx
  itkChiSquareDistribution.cxx
 )

target_link_libraries(ITKStatistics ITKCommon itkNetlibSlatec )

if(ITK_LIBRARY_PROPERTIES)
  set_target_properties(ITKStatistics PROPERTIES ${ITK_LIBRARY_PROPERTIES})
endif(ITK_LIBRARY_PROPERTIES)

if(NOT ITK_INSTALL_NO_LIBRARIES)
  install(TARGETS ITKStatistics
    RUNTIME DESTINATION ${ITK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT Development)
endif(NOT ITK_INSTALL_NO_LIBRARIES)

if(ITK_USE_FLAT_DIRECTORY_INSTALL)
  set( ITK_INSTALL_INCLUDE_DIR_CM24_REVIEW_STATISTICS ${ITK_INSTALL_INCLUDE_DIR_CM24})
else(ITK_USE_FLAT_DIRECTORY_INSTALL)
  set( ITK_INSTALL_INCLUDE_DIR_CM24_REVIEW_STATISTICS ${ITK_INSTALL_INCLUDE_DIR_CM24}/Review/Statistics)
endif(ITK_USE_FLAT_DIRECTORY_INSTALL)


if(NOT ITK_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${ITK_INSTALL_INCLUDE_DIR_CM24_REVIEW_STATISTICS}
    COMPONENT Development)
endif(NOT ITK_INSTALL_NO_DEVELOPMENT)
