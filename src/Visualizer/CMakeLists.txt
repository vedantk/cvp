PROJECT(CVIP)

#----------------------
# Find VTK
#----------------------
INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
IF (USE_VTK_FILE)
  INCLUDE(${USE_VTK_FILE})
ENDIF (USE_VTK_FILE)

#-------------------------------------------------
# Generate Execuatble
#-------------------------------------------------
ADD_EXECUTABLE(CVIP main.cpp visualize.cpp visualize.h)
TARGET_LINK_LIBRARIES(CVIP vtkRendering vtkGraphics vtkIO vtkCommon vtkFiltering vtkHybrid vtkImaging vtkPatented)
