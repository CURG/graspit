
#include "include/EGPlanner/energy/convNetEnergy.h"
#include "include/robot.h"
#include "include/grasp.h"
#include "include/debug.h"
#include "include/world.h"




#include <cstdlib>

 ConvNetEnergy::ConvNetEnergy(): SearchEnergy()
 {
     int argc = 0;
     char* argv[] = {NULL};
     ros::init(argc, argv, "get_grasp_quality_client");

     n = new ros::NodeHandle("");

     ROS_INFO("Successfully Initialized ConvNet Energy");
 }

int ConvNetEnergy::getGraspMetricsFromConvNet(std::string model_filepath,
                                              std::string grasp_points_filepath,
                                              double *epsilon_quality,
                                              double *volume_quality,
                                              double *energy) const
{
    grasp_service::GetGraspMetric srv;
    srv.request.model_filepath = model_filepath;
    srv.request.grasp_points_filepath = grasp_points_filepath;

    ros::ServiceClient client  = n->serviceClient<grasp_service::GetGraspMetric>("get_grasp_quality");
    if (client.call(srv))
    {
        ROS_INFO("epsilon: %f, volume: %f, energy: %f",
                 srv.response.epsilon_quality,
                 srv.response.volume_quality,
                 srv.response.energy);
        *epsilon_quality = srv.response.epsilon_quality;
        *volume_quality = srv.response.volume_quality;
        *energy = srv.response.energy;
    }
    else
    {
        ROS_ERROR("Failed to call service get_grasp_quality");
        return 1;
    }

    return 0;
}



double ConvNetEnergy::energy() const
{

    mHand->getGrasp()->collectVirtualContacts();

//    // save object model binvox location
//    // save contact location
    std::string model_filepath = "/home/iakinola/curg/cgdb/psb/benchmark/db/4/m482/m482.binvox";
    std::string grasp_points_filepath = "/home/iakinola/Desktop/grasp_quality_conv_net/contactpoint.binvox";


//    DBaseDlg::saveBinvoxOfContacts(QString(grasp_points_filepath.c_str()), DBaseDlg::getVirtualContactPointsLocationsFromHand());

    std::vector<vec3> contactLocs = DBaseDlg::getVirtualContactPointsLocationsFromHand();
    int num_contacts = DBaseDlg::saveBinvoxOfContactsDirectIndex(QString(grasp_points_filepath.c_str()), contactLocs);

    // get number of contacts
//    int num_contacts = contactLocs.size();
    DBGA("Number of contacts: \t" << num_contacts);

//    std::cin.ignore();
//    assert(false);

    // send paths to server
    double epsilon_quality, volume_quality, energy;
    getGraspMetricsFromConvNet(model_filepath, grasp_points_filepath, &epsilon_quality, &volume_quality, &energy);

    // return result from convnet
//    return energy;



    // if number of contacts is less than 3, return contactEnergy
    if (num_contacts < 5)
    {
        return contactEnergy();
    }

    return energy;
}






double ConvNetEnergy::contactEnergy() const
{
    mHand->getGrasp()->collectVirtualContacts();

    //DBGP("Contact energy computation")
    //average error per contact
    VirtualContact *contact;
    vec3 p,n,cn;
    double totalError = 0;
    for (int i=0; i<mHand->getGrasp()->getNumContacts(); i++)
    {
        contact = (VirtualContact*)mHand->getGrasp()->getContact(i);
        contact->getObjectDistanceAndNormal(mObject, &p, NULL);
        double dist = p.len();

        //this should never happen anymore since we're never inside the object
        //if ( (-1.0 * p) % n < 0) dist = -dist;

        //BEST WORKING VERSION, strangely enough
        totalError += fabs(dist);

        //let's try this some more
        //totalError += distanceFunction(dist);
        //cn = -1.0 * contact->getWorldNormal();

        //new version
        cn = contact->getWorldNormal();
        n = normalise(p);
        double d = 1 - cn % n;
        totalError += d * 100.0 / 2.0;
    }

    totalError /= mHand->getGrasp()->getNumContacts();

    //DBGP("Contact energy: " << totalError);
    return totalError;
}
