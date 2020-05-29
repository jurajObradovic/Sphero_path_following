#include "ros/ros.h"  //početak svakog c++ programa
#include "sphero_path_following/pose.h"  //ubaciti i geometry_msgs buduću da je to tip poruke koji koristimo
#include "nav_msgs/Odometry.h"
#include "dynamic_reconfigure/Config.h"
#define PI 3.1415926
//from nav_msgs.msg import Odometry
ros::Publisher velocity_publisher; 
ros::Subscriber param_subscriber;//stvaram globalnu varijablu za  publisher
using namespace std;  //nešto što mora uvijek isto
#include <math.h>      /* sin */

double MAX_brzina1=0;




void param_Callback(const dynamic_reconfigure::Config brzina_max);
void circle(double r);
void elipse(double a, double b);
void num_eight(double a);
void spiral();

void param_Callback(const dynamic_reconfigure::Config brzina_max)
{
       	ros::param::get("/dyn_reconf/max_speed", MAX_brzina1);
        
}


int main(int argc, char **argv)
{

	ros::init(argc, argv, "follow_path_virtual_node"); //stvaranje noda(korak 1)
	ros::NodeHandle n;
	velocity_publisher= n.advertise<nav_msgs::Odometry>("virtual_robot/odom", 10);

	param_subscriber=n.subscribe("/dyn_reconf/parameter_updates", 10, param_Callback);




    std::string path1;
    double circle_radius1, eight_length1, elipse_a1, elipse_b1; 														//path
	n.getParam("/path1", path1);
	n.getParam("/circle_radius1", circle_radius1);
	n.getParam("/eight_length1", eight_length1);
	n.getParam("/elipse_a1", elipse_a1);
	n.getParam("/elipse_b1", elipse_b1);
	cout<<path1;
	cout<<circle_radius1;

	if (path1=="circle"){
		circle(circle_radius1);
	}
	if (path1=="elipse"){
		elipse(elipse_a1, elipse_b1);
	}
	if (path1=="eight"){
		num_eight(eight_length1/2);
	}
	if (path1=="spiral"){
		spiral();
	}
}
void circle(double r){ 	
	nav_msgs::Odometry vel_msgs;
	vel_msgs.header.frame_id="virtual_odom";
	ros::Rate loop_rate(100);
	double t=0;
	double w;
	double posex;
	double posey;
	double t0= ros::Time::now().toSec(); 
	do{
	t = ros::Time::now().toSec()-t0;
	w=MAX_brzina1/r;
	posex=r*cos(w*t);
	posey=r*sin(w*t);
	vel_msgs.pose.pose.position.x=posex;
	vel_msgs.pose.pose.position.y=posey;
	printf("posex=%f\n posey=%f\n", posex, posey);
	
	velocity_publisher.publish(vel_msgs);
	ros::spinOnce(); 						
	loop_rate.sleep();

	}while ((ros::ok()));
	vel_msgs.pose.pose.position.x=0;
	vel_msgs.pose.pose.position.y=0;
	velocity_publisher.publish(vel_msgs);	
}

void elipse(double a, double b){ 	
	nav_msgs::Odometry vel_msgs;
	vel_msgs.header.frame_id="virtual_odom";
	ros::Rate loop_rate(100);
	double t=0;
	double w;
	double posex;
	double posey;
	double t0= ros::Time::now().toSec(); 
	do{
	t = ros::Time::now().toSec()-t0;
	w=2*MAX_brzina1/(a+b);
	posex=a*cos(w*t);
	posey=b*sin(w*t);
	vel_msgs.pose.pose.position.x=posex;
	vel_msgs.pose.pose.position.y=posey;
	printf("posex=%f\n posey=%f\n", posex, posey);
	
	velocity_publisher.publish(vel_msgs);
	ros::spinOnce(); 						
	loop_rate.sleep();

	}while ((ros::ok()));
	vel_msgs.pose.pose.position.x=0;
	vel_msgs.pose.pose.position.y=0;
	velocity_publisher.publish(vel_msgs);	

}

void num_eight(double a)
{
	nav_msgs::Odometry vel_msgs;
	//sphero_path_following::pose vel_msgs; 
	vel_msgs.header.frame_id="virtual_odom";
	double t0 = ros::Time::now().toSec();  		// računam trenutno vrijeme
	ros::Rate loop_rate(10);
	double t=0;

	do{				
	double w=MAX_brzina1/a*1.2;		//empirijski postavljen odnos kutne brzine i Maksimalne brzine koji daje dobro vladanje
	t = ros::Time::now().toSec()-t0;
	vel_msgs.pose.pose.position.x=(a*cos(w*t))/((pow(sin(w*t), 2)+1));
	vel_msgs.pose.pose.position.y=(a*cos(w*t)*sin(w*t))/((pow(sin(w*t), 2)+1));


	velocity_publisher.publish(vel_msgs);
	ros::spinOnce(); 						
	loop_rate.sleep();

	}while (ros::ok());
	vel_msgs.pose.pose.position.x=0;
	vel_msgs.pose.pose.position.y=0;
	velocity_publisher.publish(vel_msgs);	
}


void spiral(){ 	//realizacija metode(korak3)  //obična logaritamska spirala, radi super
	nav_msgs::Odometry vel_msgs;
	//sphero_path_following::pose vel_msgs; 
	vel_msgs.header.frame_id="virtual_odom2";
	double t0 = ros::Time::now().toSec();   		// računam trenutno vrijeme
	ros::Rate loop_rate(10);
	double t=0;
	double rs=0;
	double l_vel=0;
	double l_vel_o=0;
	double a=0.7;
	double w=0.14;
	do {
	t = ros::Time::now().toSec()-t0;
	loop_rate.sleep();
	}while(t<1);
	do{	

		t = ros::Time::now().toSec()-t0;
		vel_msgs.pose.pose.position.x=a*log(t)*cos(w*t);
		vel_msgs.pose.pose.position.y=a*log(t)*sin(w*t);
		if(t>=1){
			velocity_publisher.publish(vel_msgs);
		}
		l_vel=sqrt(pow(vel_msgs.pose.pose.position.x,2) +pow(vel_msgs.pose.pose.position.y, 2));
		printf("%f %f\n",t, l_vel-l_vel_o);
		l_vel_o=l_vel;

		ros::spinOnce(); 						
		loop_rate.sleep();
	

	}while (ros::ok());

		vel_msgs.pose.pose.position.x=0;
		vel_msgs.pose.pose.position.y=0;
		velocity_publisher.publish(vel_msgs);	
}