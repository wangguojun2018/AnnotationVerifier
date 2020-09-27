/*
 * @Authors: Guojun Wang
 * @Date: 1970-01-01 08:00:00
 * @LastEditors: Guojun Wang
 * @LastEditTime: 2020-09-27 13:57:17
 */

#include "utils.h"
#include<fstream>
#include <dirent.h>


using namespace std;
using namespace Eigen;

template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

vector<string> split(const string& str, const string& delim0 ) 
{
    vector<string> res;
    if("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char * strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str());

    char * d0 = new char[delim0.length() + 1];
    //    char * d1 = new char[delim1.length() + 1];
    strcpy(d0, delim0.c_str());
    //    strcpy(d1, delim1.c_str());

    char *p = strtok(strs, d0);
    //    cout<<"split string is "<<p<<endl;
    //    printf("%s ", p);
    while(p) 
    {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d0);
    }

    return res;
}
bool computePairNum_bin(std::string pair1,std::string pair2)
{
    return pair1 < pair2;
}

void sort_filelists(std::vector<std::string>& filists,std::string type)
{
    if (filists.empty())return;
    std::sort(filists.begin(),filists.end(),computePairNum_bin);
}
void read_filelists(const std::string& dir_path,std::vector<std::string>& out_filelsits,std::string type)
{
    struct dirent *ptr;
    DIR *dir;
    dir = opendir(dir_path.c_str());
    out_filelsits.clear();
    while ((ptr = readdir(dir)) != NULL)
    {
        std::string tmp_file = ptr->d_name;
        if (tmp_file[0] == '.')
            continue;

        if (type.size() <= 0)
        {
            out_filelsits.push_back(ptr->d_name);
        }
        else
        {
            if (tmp_file.size() < type.size())
                continue;
            std::string tmp_cut_type = tmp_file.substr(tmp_file.size() - type.size(),type.size());
            if (tmp_cut_type == type)
            {
                out_filelsits.push_back(ptr->d_name);
            }
        }
    }
    closedir(dir);
}


void read_3dbbox(string label_path,vector<Vector3f,aligned_allocator<Vector3f> >& translation,
                vector<Quaternionf,aligned_allocator<Quaternionf> > &rotation,
                vector<Vector3f,aligned_allocator<Vector3f> >& dimentions,vector<string>& class_names)
{
    ifstream filestream;
    filestream.open(label_path.c_str(),ios::in);
    string tmp;
    vector<string> tmplist;
    while (getline(filestream,tmp))
    {
        tmplist=split(tmp,"\t ");
        Vector3f trans;
        Quaternionf qua;
        Vector3f dim;

        for (int i = 0; i < tmplist.size(); ++i)
        {
           
            
            if (i>=11 && i<=13)
            {
                trans(i-11)=stringToNum<float>(tmplist[i]);
            }
            else if (i>=8 && i<=10)
            {
                dim(i-8)=stringToNum<float>(tmplist[i]);
            }
            else  if (i==14)
            {
                Matrix3f R;
                float radian=stringToNum<float>(tmplist[i]);
                R = AngleAxisf(radian, Vector3f::UnitZ());
                qua =R;
            }

            else if (i==0)
            {
               class_names.push_back(tmplist[i]);
            }
            
        }
        translation.push_back(trans);
        dimentions.push_back(dim);
        rotation.push_back(qua);
    }
}


void readKittiPclBinData(std::string &in_file,pcl::PointCloud<PointType>::Ptr cloud)
{
    cloud->clear();

    std::fstream input(in_file.c_str(), std::ios::in | std::ios::binary);
    if(!input.good()){
        std::cerr << "Could not read file: " << in_file << std::endl;
        exit(EXIT_FAILURE);
    }
    input.seekg(0, std::ios::beg);

    int i;
    for (i=0; input.good() && !input.eof(); i++) {
        PointType point;
        input.read((char *) &point.x, 3*sizeof(float));
        input.read((char *) &point.intensity, sizeof(float));
//        input.read((char *) &point.intensity, sizeof(float));
        cloud->push_back(point);
    }
    input.close();

}


void read_semantic(std::string &in_file,pcl::PointCloud<PointType>::Ptr cloud)
{
    // cloud->clear();

    std::fstream input(in_file.c_str(), std::ios::in | std::ios::binary);
    
    if(!input.good())
    {
        std::cerr << "Could not read file: " << in_file << std::endl;
        exit(EXIT_FAILURE);
    }
    //设置文件流指针位置为起始位置
    input.seekg(0, std::ios::beg);

    int i;
    for (i=0; input.good() && !input.eof(); i++) 
    {
        
        PointType& point=cloud->points[i];
        input.read((char *)&point.intensity, sizeof(uint32_t));
        
    }
    input.close();

}