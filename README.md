
## Installation
Simply clone this repository inside ROS workspace and run `catkin_make` in workspace root.

Simulation part uses [stage_ros](http://wiki.ros.org/stage_ros) simulator.


## Usage
**Simulation:**
1. roslaunch sphero_path_following launch1.launch
1. roslaunch sphero_path_following launch_path.launch 
1. roslaunch sphero_path_following launch_reynolds.launch 


Flocking algorithm parameters can be changed during runtime with _rqt_reconfigure_.
