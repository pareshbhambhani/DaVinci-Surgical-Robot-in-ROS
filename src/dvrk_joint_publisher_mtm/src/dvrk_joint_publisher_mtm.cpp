#include<fstream>
#include<iostream>
#include<vector>
#include <string>
#include<sstream>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>

using namespace std;
std::vector < std::vector<double> > jointAngleArray;
int rate;

void readAngles(){
	double angleValue;
	std::vector<double> jointAngles;
	std::ifstream jointFile;
	std::string line;
	jointFile.open("Angle_mtm",std::ios::in);
	if(jointFile.is_open()){
		getline(jointFile,line);
		std::stringstream stream(line);
		stream >> rate;
		std::string temp;
		while(getline(jointFile,temp)){
			std::stringstream stream(temp);
			while(stream >> angleValue){
				jointAngles.push_back(angleValue);
			}
			jointAngleArray.push_back(jointAngles);
			jointAngles.clear();
		}
	}
	else{
	std::cout<<"error while opening file";
	}
}



int main(int argc, char** argv) {
ros::init(argc, argv, "move_mtm_arm");
ros::NodeHandle n;
readAngles();
cout<<jointAngleArray.size()<<endl;
cout<<jointAngleArray[1].size()<<endl;
for(int i=0;i<jointAngleArray.size();i++){
	for(int j=0;j<jointAngleArray[i].size();j++){
		cout << j <<" : "<<jointAngleArray[i][j]  << endl;	
	} 
}
	
ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("/dvrk_mtm1/joint_states", 1000);
ros::Rate loop_rate(rate);

sensor_msgs::JointState joint_state;

joint_state.name.resize(8);
joint_state.position.resize(8);
joint_state.name[0] ="right_outer_yaw_joint";
joint_state.name[1] ="right_shoulder_pitch_joint";
joint_state.name[3] ="right_elbow_pitch_joint";
joint_state.name[4] ="right_wrist_platform_joint";
joint_state.name[5] ="right_wrist_pitch_joint";
joint_state.name[6] ="right_wrist_yaw_joint";
joint_state.name[7] ="right_wrist_roll_joint";
int i=0;
int j=0;

while (ros::ok()) {

    //update joint_state
    if(i<jointAngleArray.size()){
    joint_state.header.stamp = ros::Time::now();
    joint_state.position[0] = jointAngleArray[i][j];
    joint_state.position[1] = jointAngleArray[i][j+1];
    joint_state.position[2] = jointAngleArray[i][j+1];
    joint_state.position[3] = jointAngleArray[i][j+2];
    joint_state.position[4] = jointAngleArray[i][j+3];
    joint_state.position[5] = jointAngleArray[i][j+4];
    joint_state.position[6] = jointAngleArray[i][j+5];
    joint_state.position[7] = jointAngleArray[i][j+6];
	i++;
	}else{
		i=0;
	}

    //send the joint state
    joint_pub.publish(joint_state);
    loop_rate.sleep();
}
return 0;
}
