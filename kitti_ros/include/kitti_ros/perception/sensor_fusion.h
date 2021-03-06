#ifndef sensorfusion_H
#define sensorfusion_H

#include <jsk_recognition_msgs/BoundingBox.h>
#include <jsk_recognition_msgs/BoundingBoxArray.h>
#include <kitti_ros/kitti_data_operator.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <ros/ros.h>
#include <kitti_ros/segmenters_lib/segmenter_manager.hpp>
#include <object_builders/base_object_builder.hpp>
#include <object_builders/object_builder_manager.hpp>

#include "common/parameter.hpp" /* common::getSegmenterParams */
#include "common/publisher.hpp"
#include "common/time.hpp"     /* common::Clock */
#include "common/types/type.h" /* PointICloudPtr */

using namespace autosense;

class SensorFusion {
   public:
    SensorFusion();
    ~SensorFusion();

    void FillKittiData4Fusion();

    void RGBPCL_PCL2ImageFusion();

    void ProcessLabelofBEVImage(std::string& label_infile_string,
                                std::string image_file_path);

    void SetKITTIDataOperator(KITTIDataOperator* value);

    const KITTIDataOperator* GetKITTIDataOperator();

    void SetKittiObjectOperator(KittiObjectOperator* value);

    const KittiObjectOperator* GetKittiObjectOperator();

    void SetTools(Tools* value);

    const Tools* GetTools();

    void SegmentedPointCloudFromMaskRCNN(cv::Mat* maskrcnn_segmented_image,
                                         std::string image_file_path);

    void SetSegmentedLidarScan(sensor_msgs::PointCloud2 value);

    sensor_msgs::PointCloud2 GetSegmentedLidarScan();

    void ProcessObjectBuilder(
        pcl::PointCloud<pcl::PointXYZI>::Ptr out_cloud_obj_builder,
        std::string image_file_path, cv::Mat* maskrcnn_segmented_image);

    void PublishRawData();

   private:
    float EuclidianDistofPoint(pcl::PointXYZRGB* colored_3d_point);

    void CreateBirdviewPointcloudImage(
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr out_cloud,
        std::string image_file);

    ros::NodeHandlePtr nh_;
    ros::NodeHandle private_nh;

    KITTIDataOperator* kitti_data_operator_;
    KittiObjectOperator* kitti_object_operator_;

    Tools* tools_;
    sensor_msgs::PointCloud2 lidar_scan_;
    sensor_msgs::PointCloud2 segmented_lidar_scan_;
    cv::Mat kitti_left_cam_img_;

    boost::shared_ptr<segmenter::BaseSegmenter> ground_remover_;
    boost::shared_ptr<segmenter::BaseSegmenter> segmenter_;
    std::string frame_id_;
    // ROS Subscriber
    // ROS Publisher

    ros::Publisher vis_pub_;
    ros::Publisher ground_pub_;
    ros::Publisher nonground_pub_;
    ros::Publisher clusters_pub_;

    ros::Publisher detected_obstacles_publisher_;

    ros::Publisher rgb_pointcloud_pub_;

    ros::Publisher segmented_pointcloud_from_maskrcnn_pub_;

    ros::Publisher pointcloud_projected_image_pub_;

    ros::Publisher kitti_pcl_pub_;

    ros::Publisher kitti_image_pub_;

    ros::Publisher birdview_pointcloud_image_pub_;

    ros::Publisher jsk_box_array_pub_;

    ros::Publisher box_projetcted_image_pub_;

    Eigen::MatrixXf TRANS_VELO_TO_CAM;
};
#endif