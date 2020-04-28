# Acrobot-DP

C++ implementation of value iteration for:

 - the height task for the acrobot
 - the stand-up task for the inverted pendulum

## Dependencies

CMake, Eigen3, LibYAML, and Boost are required:

```bash
sudo apt-get install build-essential cmake libeigen3-dev libboost-all-dev libyaml-dev
```

## Installation and Usage

For installation:

```bash
git clone --recurse-submodules https://github.com/goromal/acrobot-dp.git
mkdir acrobot-dp/build && cd acrobot-dp/build
cmake ..
make
```

Subsequently, to create and apply the control policy for the inverted pendulum system, run:

```bash
./calculate-inverted-pendulum-policy # WARNING: will take around 40 minutes to run and will generate a log file 27 megabytes in size!
./apply-inverted-pendulum-policy # runs almost instantaneously, generates 400 log files of 2 kilobytes each
```

Results can be visualized with the following Matlab scripts:

 - visualize\_ip\_input_map.m
 - visualize\_ip\_tracking.m

To create and apply the acrobot policy, run:
```bash
./calculate-acrobot-policy # WARNING: will take around 13 HOURS to run and will generate a log file 240 megabytes in size!
./apply-acrobot-policy # runs almost instantaneously, generates 400 log files of 2-8 kilobytes each
```

Results can be visualized with the following Matlab script:

 - visualize\_ab\_tracking.m


