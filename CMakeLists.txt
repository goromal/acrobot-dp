cmake_minimum_required (VERSION 2.8.11)
project(acrobot-dp)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)

include_directories(
  include
  ${EIGEN3_INCLUDE_DIRS}
)

add_executable(calculate-inverted-pendulum-policy
    src/calculate_inverted_pendulum_policy.cpp
)
target_compile_definitions(calculate-inverted-pendulum-policy
    PRIVATE
    NUM_THD1=72
    NUM_THD2=72
    THD1_0=-6.19592
    THD2_0=-6.19592
)

add_executable(calculate-acrobot-policy
    src/calculate_acrobot_policy.cpp
)
target_compile_definitions(calculate-acrobot-policy
    PRIVATE
    NUM_THD1=144
    NUM_THD2=324
    THD1_0=-12.4791
    THD2_0=-28.1871
)

add_executable(apply-inverted-pendulum-policy
    src/apply_inverted_pendulum_policy.cpp
)
target_compile_definitions(apply-inverted-pendulum-policy
    PRIVATE
    NUM_THD1=72
    NUM_THD2=72
    THD1_0=-6.19592
    THD2_0=-6.19592
)

add_executable(apply-acrobot-policy
    src/apply_acrobot_policy.cpp
)
target_compile_definitions(apply-acrobot-policy
    PRIVATE
    NUM_THD1=144
    NUM_THD2=324
    THD1_0=-12.4791
    THD2_0=-28.1871
)
