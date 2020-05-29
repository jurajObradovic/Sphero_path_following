#!/usr/bin/env python

import rospy
import math
#from turtlesim.msg import Pose
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
x_robot_pose=0
y_robot_pose=0
def poseCallback(pose_message):
	global x_robot_pose
	global y_robot_pose
	x_robot_pose=pose_message.pose.pose.position.x
	y_robot_pose=pose_message.pose.pose.position.y
	rospy.loginfo("x: %s y: %s", x_robot_pose, y_robot_pose)

#def get_distance(x1, y1, x2, y2):
#	return(math.sqrt(math.pow((x1-x2),2) + math.pow((y1-y2), 2)))

def arrive_velocity(pose_x, pose_y):
	 #definiram publisher(topic tipa Twist)
	 #definiram node anonymous=True da nebi imao vise istih nodova
	rate=rospy.Rate(10) #svako koliko objavljujem topic
	twist=Twist()
	max_speed=1
	radius=2
	while not rospy.is_shutdown():
		target_offset_x=pose_x - x_robot_pose
		target_offset_y=pose_y - y_robot_pose
		ramped_speed_x=max_speed*(target_offset_x/radius)
		ramped_speed_y=max_speed*(target_offset_y/radius)
		twist.linear.x=min(ramped_speed_x, max_speed)
		twist.linear.y=min(ramped_speed_y, max_speed)
		twist.linear.z=0
		twist.angular.x=0
		twist.angular.y=0
		twist.angular.z=0
		pub.publish(twist)
		rate.sleep()



if __name__=='__main__':
	try:

		rospy.init_node('arrive_velocity_node', anonymous=True)
		pub=rospy.Publisher('cmd_vel', Twist, queue_size=10)
		sub=rospy.Subscriber('odom', Odometry, poseCallback)

		pose_x=2
		pose_y=2
		arrive_velocity(pose_x, pose_y)
	except rospy.ROSInterruptException:
		pass
