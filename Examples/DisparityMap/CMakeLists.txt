project(DisparityMapExamples)

add_executable(FineRegistrationImageFilterExample FineRegistrationImageFilterExample.cxx)
target_link_libraries(FineRegistrationImageFilterExample ${OTB_LIBRARIES})

add_executable(NCCRegistrationFilterExample NCCRegistrationFilterExample.cxx)
target_link_libraries(NCCRegistrationFilterExample ${OTB_LIBRARIES})

add_executable(SimpleDisparityMapEstimationExample SimpleDisparityMapEstimationExample.cxx)
target_link_libraries(SimpleDisparityMapEstimationExample ${OTB_LIBRARIES})

add_executable(StereoReconstructionExample StereoReconstructionExample.cxx)
target_link_libraries(StereoReconstructionExample ${OTB_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()