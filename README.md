# mobile-robot
An autonomous robot which can estimate it's position(abscissa and ordinate), and orientation(angle) in a 2-D space.
Data received from encoders is processed on raspberry-pi using ROS. Complete processes is executed via three nodes
position-estimation-node, encoder-node, teleop-motor-control-node.
positon-estimation-node subscribes to encoder-node and teleop-motor-control-node and prints localization data.
teleop-motor-control-node control node controls robot using realtime keyboard control.
encoder-node reads sensor values and publishes message when counters increment.
 
