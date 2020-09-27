/*
 * @Authors: Guojun Wang
 * @Date: 1970-01-01 08:00:00
 * @LastEditors: Guojun Wang
 * @LastEditTime: 2020-09-23 12:03:01
 */

#include<string>
#include<vector>
#include"types.h"
#include<Eigen/Core>



using namespace std;
using namespace Eigen;

template <class Type>
Type stringToNum(const string& str);


vector<string> split(const string& str, const string& delim0);
bool computePairNum_bin(std::string pair1,std::string pair2);

void sort_filelists(std::vector<std::string>& filists,std::string type);
void read_filelists(const std::string& dir_path,std::vector<std::string>& out_filelsits,std::string type);



void read_3dbbox(string label_path,vector<Vector3f,aligned_allocator<Vector3f> >& translation,
                vector<Quaternionf,aligned_allocator<Quaternionf> > &rotation,
                vector<Vector3f,aligned_allocator<Vector3f> >& dimentions,vector<string>& class_names);


void readKittiPclBinData(std::string &in_file,pcl::PointCloud<PointType>::Ptr cloud);


void read_semantic(std::string &in_file,pcl::PointCloud<PointType>::Ptr cloud);