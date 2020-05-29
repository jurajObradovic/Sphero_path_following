#include "ros/ros.h"  //početak svakog c++ programa
#include "sphero_path_following/pose.h"  //ubaciti i geometry_msgs buduću da je to tip poruke koji koristimo
#include "nav_msgs/Odometry.h"
#include "dynamic_reconfigure/Config.h"
#define PI 3.1415926
//from nav_msgs.msg import Odometry
ros::Publisher velocity_publisher; 
ros::Subscriber param_subscriber;//stvaram globalnu varijablu za  publisher
ros::Subscriber pose_subscriber;
using namespace std;  //nešto što mora uvijek isto
#include <math.h>      /* sin */

double MAX_brzina2=0;
double x_virtual=0;
double y_virtual=0;





void circle(double r); //metoda za gibanje ravno, deklaracija metode korak(2)
void param_Callback(const dynamic_reconfigure::Config brzina_max);
void pose_Callback(const nav_msgs::Odometry virtual_odom);
void num_eight(double a);
void spinaround(double r);
void spiral();
void elipse(double a, double b);
	




void param_Callback(const dynamic_reconfigure::Config brzina_max)
{
       	ros::param::get("/dyn_reconf2/max_speed", MAX_brzina2);
        
}

void pose_Callback(const nav_msgs::Odometry virtual_odom){

	    x_virtual=virtual_odom.pose.pose.position.x;
       	y_virtual=virtual_odom.pose.pose.position.y;
}


int main(int argc, char **argv)
{

	ros::init(argc, argv, "follow_path_virtual_node_2"); //stvaranje noda(korak 1)
	ros::NodeHandle n;
	velocity_publisher= n.advertise<nav_msgs::Odometry>("virtual_robot2/odom", 10);
	param_subscriber=n.subscribe("/dyn_reconf2/parameter_updates", 10, param_Callback);
	pose_subscriber=n.subscribe("/virtual_robot/odom", 10, pose_Callback);


	std::string path2;
	double circle_radius2, eight_length2, elipse_a2, elipse_b2; 														//path
	n.getParam("/path2", path2);
	n.getParam("/circle_radius2", circle_radius2);
	n.getParam("/eight_length2", eight_length2);
	n.getParam("/elipse_a2", elipse_a2);
	n.getParam("/elipse_b2", elipse_b2);
	cout<<path2;
	cout<<circle_radius2;


	if (path2=="circle"){
		circle(circle_radius2);
	}
	if (path2=="elipse"){
		elipse(elipse_a2, elipse_b2);
	}
	if (path2=="eight"){
		num_eight(eight_length2/2);
	}
	if (path2=="spiral"){
		spiral();
	}
	if (path2=="spin"){
		spinaround(2);
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
	w=MAX_brzina2/r;
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
	w=2*MAX_brzina2/(a+b);
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
	double w=MAX_brzina2/a*1.2;		//empirijski postavljen odnos kutne brzine i Maksimalne brzine koji daje dobro vladanje
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


void spinaround(double r){

	nav_msgs::Odometry vel_msgs;
	//sphero_path_following::pose vel_msgs; 
	vel_msgs.header.frame_id="virtual_odom2";
	ros::Rate loop_rate(100);
	double t0= ros::Time::now().toSec(); 
	double t=0;
	double w;
	double posex;
	double posey;
	 //treba mi da bi mogao brojati kruznice i kada se unese t0
	
	do{			
			//stvaram poruku tipa geometry_msgs::Twist
	t = ros::Time::now().toSec()-t0;

	w=0.9*MAX_brzina2/r;  //malo usporenje zbog boljeg ponašanja
	posex=x_virtual+2*cos((w)*t-0.5);
	posey=y_virtual+2*sin((w)*t-0.5);
	//w=abs(-0.003*pow(dist,2)+(MAX_brzina2/r)); 

	vel_msgs.pose.pose.position.x=posex;
	vel_msgs.pose.pose.position.y=posey;
	
	
	velocity_publisher.publish(vel_msgs);
	ros::spinOnce(); 						
	loop_rate.sleep();


	}while ((ros::ok()));

}