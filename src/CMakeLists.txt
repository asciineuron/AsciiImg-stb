qt_add_executable(asciimg 
  main.cpp 
  stb_image.h 
  stb_image_write.h 
  stb_truetype.h
  MyApp.cpp
  MyApp.hpp
  mylib.hpp
  mylib.cpp)

target_link_libraries(asciimg PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

set_target_properties(asciimg PROPERTIES MACOSX_BUNDLE ON)
