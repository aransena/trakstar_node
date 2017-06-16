//
// Created by aransena on 15/05/17.
//
#include "ros/ros.h"
#include "tf/tf.h"
#include "PointATC3DG.h"
#include "std_msgs/String.h"
#include <string>
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/PoseStamped.h"

float deg2rad(float deg)
{
    return 2*M_PI * (deg/360);
}

double inch2m(double inch){
    return inch*25.4/1000.00;
}

int main(int argc, char **argv)
{
    std::stringstream ss;
    bool array_mode = false;
    if(argc > 1){
	std::string mode = argv[1];
	if(mode=="a"){
	       std::cout << "Array Mode" << std::endl;
 	       array_mode = true;
	}
    }
    // Setup TrakStar
    std::cout << "Connecting..\n";
    PointATC3DG bird;
    std::cout << "Something returned...\n" << bird.ok();
    if( !bird ) return -1;
    bird.setSuddenOutputChangeLock( 0 );

    double X, Y, Z, dX, dY, dZ, dAzimuth, dElevation, dRoll;
    ros::Publisher sensor_pub;
    geometry_msgs::PoseStamped sensor_pose;
    geometry_msgs::PoseArray sensor_data;
    sensor_pose.header.frame_id = "base";

    int numsen=bird.getNumberOfSensors();
    ros::Publisher publishers[numsen];
    std::cout << "nSensors: " << numsen << std::endl;

    // Setup ROS
    ros::init(argc, argv, "trakstar_node");
    ros::NodeHandle n;
    if(array_mode) {
        sensor_pub = n.advertise<geometry_msgs::PoseArray>("trakstar/trakstar_data", 1000);
        sensor_data.header.frame_id = "base";
    }
    else {
        for (int i = 0; i < numsen; i++) {
            ss.str(std::string());
            ss << "trakstar/trakstar_" << i;
            const std::string topic_name = ss.str().c_str();
            publishers[i] = n.advertise<geometry_msgs::PoseStamped>(topic_name, 1000);
        }
    }
    ros::Rate loop_rate(50);

    while (ros::ok())
    {
        if(array_mode) {
            sensor_data.header.stamp = ros::Time::now();
        }
        for(int sensor_id=0; sensor_id<numsen; sensor_id++) {
            bird.getCoordinatesAngles(sensor_id, dX, dY, dZ, dAzimuth, dElevation, dRoll);
            sensor_pose.header.stamp = ros::Time::now();

            // values given in inch, convert to m.
            X = inch2m(dX);
            Y = inch2m(dY);
            Z = inch2m(dZ);

            sensor_pose.pose.position.x = X;
            sensor_pose.pose.position.y = Y;
            sensor_pose.pose.position.z = Z;

            sensor_pose.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(deg2rad(dRoll),
                                                                                   deg2rad(dElevation),
                                                                                   deg2rad(dAzimuth));

            ss << "i: " << sensor_id << ", X: " << X << ", Y: " << Y << ", Z: " << Z
               << ", A: " << dAzimuth << ", E: " << dElevation << ", R: " << dRoll << std::endl;
            //ROS_INFO("%s", ss.str().c_str());

            if(array_mode) {
                sensor_data.poses.push_back(sensor_pose.pose);
            }
            else{
                publishers[sensor_id].publish(sensor_pose);
            }
        }
        if(array_mode) {
            sensor_pub.publish(sensor_data);
            sensor_data.poses.clear();
        }
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
