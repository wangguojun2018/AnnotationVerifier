#include <iostream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <pcl/io/pcd_io.h>
#include <pcl/console/parse.h>
#include <Eigen/Core>
#include <iostream>
#include <eigen3/Eigen/Geometry>
#include <sstream>
#include <math.h>
#include <pcl/console/parse.h>
#include <pcl/filters/passthrough.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.cpp"
// #include "types.h"

using namespace std;
using namespace Eigen;

int framenum = 0;
string pcd_dir = "../data/pcd";
string label_dir = "../data/label";
string pcd_verified_dir;
string label_verified_dir;
string current_pcd_path;
string current_label_path;
PointCloud::Ptr cloud(new PointCloud);
vector<Vector3f, aligned_allocator<Vector3f>> centers;
vector<Vector3f, aligned_allocator<Vector3f>> dims;
vector<Quaternionf, aligned_allocator<Quaternionf>> quas;
vector<string> class_names;
string file_no_suffix;
vector<string> file_lists;

void read_cloud_label()
{
    if (file_lists.size()==0)
    {
       return;
    }
    
    centers.clear();
    dims.clear();
    quas.clear();
    class_names.clear();
    cloud->clear();

    file_no_suffix = file_lists[framenum].substr(0, file_lists[framenum].rfind("."));

    current_pcd_path = pcd_dir + file_no_suffix + ".pcd";

    pcl::io::loadPCDFile<PointType>(current_pcd_path, *cloud);

    current_label_path = label_dir + file_no_suffix + ".txt";

    read_3dbbox(current_label_path, centers, quas, dims, class_names);
}

void keyboardEventOccurredPlus(const pcl::visualization::KeyboardEvent &event, void *nothing)
{
    pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *>(nothing);

    if (event.getKeySym() == "d" && event.keyDown())
    {

        framenum += 1;
        if (framenum >= file_lists.size())
        {
            framenum = 0;
        }

        read_cloud_label();

        if (!viewer->updateText(file_no_suffix, 10, 950, 20, 0, 1, 0,"frameid_text"))
        {
            viewer->addText(file_no_suffix, 10, 950, 20, 0, 1, 0, "frameid_text");
        }
    }
}
void keyboardEventOccurredSub(const pcl::visualization::KeyboardEvent &event, void *nothing)
{
    pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *>(nothing);

    if (event.getKeySym() == "a" && event.keyDown())
    {

        framenum -= 1;
        if (framenum < 0)
        {
            framenum = file_lists.size() - 1;
        }
        read_cloud_label();
    }

    if (!viewer->updateText(file_no_suffix, 10, 950, 20, 0, 1, 0,"frameid_text"))
    {
        viewer->addText(file_no_suffix, 10, 950, 20, 0, 1, 0, "frameid_text");
    }
}

void keyboardEventOccurred_save(const pcl::visualization::KeyboardEvent &event, void *viewer_void)
{
    pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *>(viewer_void);
    if (event.getKeySym() == "s" && event.keyDown())
    {

        // string::size_type pos = current_pcd_path.rfind("/") + 1;
        // string file_no_suffix = current_pcd_path.substr(pos, current_pcd_path.rfind(".") - pos);
        string new_pcd_path = pcd_verified_dir + file_no_suffix + ".pcd";
        string new_label_path = label_verified_dir + file_no_suffix + ".txt";
        file_lists.erase(file_lists.begin()+framenum);

        framenum-=1;
        if (framenum < 0)
        {
            framenum = file_lists.size() - 1;
        }

        system(("mv " + current_pcd_path + " " + new_pcd_path).c_str());
        cout << "move verified " << file_no_suffix + ".pcd"
             << " successful!" << endl;
        system(("mv " + current_label_path + " " + new_label_path).c_str());
        cout << "move verified " << file_no_suffix + ".txt"
             << " successful!" << endl;

        // if (!viewer->updateText("save "+file_no_suffix+" successful", 1000, 950, 20, 0, 1, 0))
        {
            viewer->addText("save "+file_no_suffix+" successful", 1000, 950, 20, 0, 1, 0, "save_text");
        }
    }
}

int main(int argc, char **argv)
{

    pcl::console::parse_argument(argc, argv, "-pcd", pcd_dir);
    pcl::console::parse_argument(argc, argv, "-label", label_dir);
    // pcl::console::parse_argument(argc, argv, "-frame", frame_num);

    cout << "Read pcd file from " << pcd_dir << endl;
    cout << "Read label file from " << label_dir << endl;

    if (pcd_dir.substr(pcd_dir.length() - 1, 1) != "/")
    {
        pcd_dir += "/";
    }

    if (label_dir.substr(label_dir.length() - 1, 1) != "/")
    {
        label_dir += "/";
    }
    cout<<"pcd dir is "<<pcd_dir<<endl;
    string::size_type iPos = pcd_dir.rfind("/", pcd_dir.length() - 2);
    string root_dir = pcd_dir.substr(0, iPos);
    cout<<"root dir is "<<root_dir<<endl;
    pcd_verified_dir = root_dir + "/" + "pcd_verified/";
    label_verified_dir = root_dir + "/" + "label_verified/";

    if (opendir(pcd_verified_dir.c_str()) == nullptr)
    {
        try
        {
            system(("mkdir " + pcd_verified_dir).c_str());
            cout << "Create pcd verified dir successful!" << endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            cout << "Create pcd verified dir failed!" << endl;
        }
    }

    if (opendir(label_verified_dir.c_str()) == nullptr)
    {
        try
        {
            system(("mkdir " + label_verified_dir).c_str());
            cout << "Create label verified dir successful!" << endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            cout << "Create label verified dir failed!" << endl;
        }
    }

    read_filelists(label_dir, file_lists, "txt");
    sort_filelists(file_lists, "txt");

    file_no_suffix = file_lists[framenum].substr(0, file_lists[framenum].rfind("."));

    current_pcd_path = pcd_dir + file_no_suffix + ".pcd";

    pcl::io::loadPCDFile<PointType>(current_pcd_path, *cloud);

    current_label_path = label_dir + file_no_suffix + ".txt";

    read_3dbbox(current_label_path, centers, quas, dims, class_names);

    pcl::visualization::PCLVisualizer viewer("3DBoxViewer");

    viewer.setBackgroundColor(0, 0, 0);
    viewer.addCoordinateSystem(5);

    pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZI>::Ptr
        field_handler(new pcl::visualization::PointCloudColorHandlerGenericField<PointType>("z"));

    viewer.registerKeyboardCallback(&keyboardEventOccurredPlus, &viewer);
    viewer.registerKeyboardCallback(&keyboardEventOccurredSub, &viewer);

    viewer.registerKeyboardCallback(&keyboardEventOccurred_save, &viewer);

    while (!viewer.wasStopped())
    {

        viewer.removeAllPointClouds(); //删除点云缓存
                                       //        viewer.removeAllPointClouds(v1);//删除点云缓存
        viewer.removeAllShapes();

        viewer.addText(file_no_suffix, 10, 950, 20, 0, 1, 0, "frameid_text");

        field_handler->setInputCloud(cloud);
        viewer.addPointCloud(cloud, *field_handler, "points");
        viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,
                                                2, "points");
        viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY,
                                                0.9, "points");

        //add predictions boxes
        pcl::PointXYZ point;
        for (int j = 0; j < centers.size(); ++j)
        {
            stringstream ss;
            stringstream cls;
            ss << j;
            cls << class_names[j];

            viewer.addCube(centers[j], quas[j], dims[j](0), dims[j](1), dims[j](2), "cube" + ss.str());

            viewer.setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME,
                                               "cube" + ss.str());
            viewer.setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 4,
                                               "cube" + ss.str());
            viewer.setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 1, 0, 0,
                                               "cube" + ss.str());
        }

        viewer.spinOnce(50);
    }

    return 0;
}
