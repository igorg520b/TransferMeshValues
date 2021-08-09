cmake_minimum_required(VERSION 3.5)

project(tmv LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#find_package(Eigen3 REQUIRED NO_MODULE)
#find_package(HDF5 REQUIRED COMPONENTS CXX)
#find_package(Boost REQUIRED)
find_package(VTK 9.0 REQUIRED COMPONENTS
  CommonColor
  CommonCore
  FiltersSources
  FiltersGeometry
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  ViewsQt
  IOGeometry
  RenderingAnnotation
  RenderingLabel
  IOImage
)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(tmv
    gui/main.cpp
    gui/windows/mainwindow.cpp
    gui/windows/mainwindow.h
    gui/windows/mainwindow.ui

#    geometry/node.cpp
#    geometry/node.h
#    geometry/element.cpp
#    geometry/element.h
#    geometry/meshfragment.cpp
#    geometry/meshfragment.h
#    geometry/meshview.cpp
#    geometry/meshview.h

    gui/interactor/SpecialSelector2D.cpp
    gui/interactor/SpecialSelector2D.h
)

#target_include_directories(tmv PUBLIC ${HDF5_INCLUDE_DIRS})
#target_include_directories(tmv PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(tmv PUBLIC gui gui/windows gui/interactor geometry)

#target_link_libraries(tmv PUBLIC m dl)
target_link_libraries(tmv PRIVATE Qt5::Widgets Qt5::Core)
target_link_libraries(tmv PRIVATE ${VTK_LIBRARIES})
#target_link_libraries(tmv PUBLIC ${HDF5_LIBRARIES})
#target_link_libraries(tmv PUBLIC Eigen3::Eigen)
#target_link_libraries(tmv PUBLIC ${Boost_LIBRARIES})
