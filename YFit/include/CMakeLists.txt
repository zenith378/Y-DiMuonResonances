add_library(libCorCar SHARED src/Cuts.C src/df_set.C src/SpectrumPlot.C src/fitRoo.C src/optionParse.C)


target_link_libraries(libCorCar ${ROOT_LIBRARIES})
