cmake_minimum_required(VERSION 3.8)
project(snake)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(main 
  src/main.cpp
  src/Game.cpp
)

add_executable(DirectionSubscriber 
  src/DirectionSubscriber.cpp
)

target_include_directories(main PUBLIC include)
target_include_directories(DirectionSubscriber PUBLIC include)

ament_target_dependencies(main 
  rclcpp
  std_msgs
)

ament_target_dependencies(DirectionSubscriber 
  rclcpp
  std_msgs
)

install(TARGETS 
  main
  DirectionSubscriber
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()