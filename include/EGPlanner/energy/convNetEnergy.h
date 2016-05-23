
#ifndef _convnetenergy_h_
#define _convnetenergy_h_

#include "ros/ros.h"

#include "include/EGPlanner/energy/searchEnergy.h"
#include "grasp_service/GetGraspMetric.h"

#include "DBase/dbaseDlg.h"

class ConvNetEnergy: public SearchEnergy
{

protected:

    ros::NodeHandle *n;


    int getGraspMetricsFromConvNet(std::string model_filepath,
                                   std::string grasp_points_filepath,
                                   double *epsilon_quality, double *volume_quality, double *energy) const;

public:
    ConvNetEnergy();
    double energy() const;
    double contactEnergy() const;
};


#endif
