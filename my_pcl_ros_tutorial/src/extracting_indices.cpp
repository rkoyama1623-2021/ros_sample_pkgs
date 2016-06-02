#include <iostream>
#include <string>
#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/extract_indices.h>

//get basename from path
std::string basename(const std::string& path) {
    return path.substr(path.find_last_of('/') + 1);
}

int
main (int argc, char** argv)
{
  pcl::PCLPointCloud2::Ptr 
    cloud_blob (new pcl::PCLPointCloud2), 
    cloud_filtered_blob (new pcl::PCLPointCloud2);
  pcl::PointCloud<pcl::PointXYZ>::Ptr 
    cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>), 
    cloud_p (new pcl::PointCloud<pcl::PointXYZ>), 
    cloud_f (new pcl::PointCloud<pcl::PointXYZ>);

  //Setting pcd file
  std::string file_path;
  if (argc < 2) {
    std::cerr << "[usage] you may need to specify the pcd file name.\n"
              << "rosrun my_pcl_ros_tutorial extracting_indices <file name>"
              << std::endl
              << std::endl;
    file_path="table_scene_lms400.pcd";
  }
  else file_path = argv[1];
  std::string file_name = basename(file_path);
  std::cerr << "file path is " << file_path << std::endl;
  std::cerr << "file name is " << file_name << std::endl;
  std::cerr << "\n" << std::endl;

  /* load pcd file */
  // Fill in the cloud data
  pcl::PCDReader reader;
  reader.read (file_path, *cloud_blob);

  std::cerr << "PointCloud before filtering: " << cloud_blob->width * cloud_blob->height << " data points." << std::endl;

  /* downsample pcl */
  // Create the filtering object: downsample the dataset using a leaf size of 1cm
  pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
  sor.setInputCloud (cloud_blob);
  sor.setLeafSize (0.01f, 0.01f, 0.01f);//a leaf size of 1cm
  sor.filter (*cloud_filtered_blob);

  // Convert to the templated PointCloud
  pcl::fromPCLPointCloud2 (*cloud_filtered_blob, *cloud_filtered);

  std::cerr << "PointCloud after filtering: " << cloud_filtered->width * cloud_filtered->height << " data points." << std::endl;

  // Write the downsampled version to disk
  pcl::PCDWriter writer;
  std::stringstream ss;
  ss << file_name.replace(file_name.find(".pcd"),4,"") << "_downsampled.pcd";
  writer.write<pcl::PointXYZ> (ss.str (), *cloud_filtered, false);
  ss.str("");

  /* Segmentation */
  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients ());
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices ());
  // Create the segmentation object
  pcl::SACSegmentation<pcl::PointXYZ> seg;
  // Optional
  seg.setOptimizeCoefficients (true);
  // Mandatory
  seg.setModelType (pcl::SACMODEL_PLANE);
  //seg.setModelType (pcl::SACMODEL_CYLINDER);
  seg.setMethodType (pcl::SAC_RANSAC);
  seg.setMaxIterations (1000);
  seg.setDistanceThreshold (0.01);

  /* Extract Indices */
  // Create the filtering object
  pcl::ExtractIndices<pcl::PointXYZ> extract;

  int i = 0, nr_points = (int) cloud_filtered->points.size ();//down sampled PointCloud
  // While 30% of the original cloud is still there
  while (cloud_filtered->points.size () > 0.3 * nr_points)
    {
      // Segment the largest planar component from the remaining cloud
      //// set input
      seg.setInputCloud (cloud_filtered);
      //// set inliers(pcl::PointIndices::Ptr) and coefficients of model (pcl::ModelCoefficients)
      seg.segment (*inliers, *coefficients);

      if (inliers->indices.size() == 0)
        {
          std::cerr << "Could not estimate a planar model for the given dataset." << std::endl;
          break;
        }

      // Extract the inliers
      std::cerr << "size of cloud_filtered is : " << cloud_filtered->width * cloud_filtered->height << " data points." << std::endl;
      std::cerr << "size of inliers is : " << inliers->indices.size() << " data points." << std::endl;
      extract.setInputCloud (cloud_filtered);//set input cloud(inherited from PCLBase)
      extract.setIndices (inliers);//set input indicies
      extract.setNegative (false);//true にすると inliers が除去され残りがoutputされ、false にすると inliers 以外が除去され、inliersに該当する点群がoutputされます。
      extract.filter (*cloud_p);//execute filter and substitute the output into cloud_p
      std::cerr << "PointCloud representing the planar component: " << cloud_p->width * cloud_p->height << " data points." << std::endl << std::endl;

      // std::stringstream ss;
      ss << file_name << "_" << i << ".pcd";
      writer.write<pcl::PointXYZ> (ss.str (), *cloud_p, false);
      ss.str("");

      // Create the filtering object
      extract.setNegative (true);
      extract.filter (*cloud_f);
      cloud_filtered.swap (cloud_f);
      // //// check the extracted indices
      // std::cerr << "extracted indices are " << std::endl;
      // for (std::vector<int>::const_iterator j = inliers->indices.begin(); j != inliers->indices.end(); ++j)
      //   std::cerr << *j << ' ';
      // std::cerr <<std::endl;


      i++;
    }

  return (0);
}
