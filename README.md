<!--
 * @Authors: Guojun Wang
 * @Date: 1970-01-01 08:00:00
 * @LastEditors: Guojun Wang
 * @LastEditTime: 2020-09-27 16:48:34
 -->
# PointCloud Verifing Tool
Tool for verifing semantic annotaion and bounding box annotation.
![bbox复核](https://github.com/wangguojun2018/AnnotationVerifier/blob/master/data/bbox.png)
![semantic复核](https://github.com/wangguojun2018/AnnotationVerifier/blob/master/data/semantic.png)

用于复核已经标注好的3D语义标注和边界框标注

## Dependencies
* PCL

## Build
On Ubuntu 16.04 and 18.04, most of the dependencies can be installed from the package manager: 

Install PCL
```bash
sudo apt install libpcl-dev
```
Clone the repository
```bash
git clone https://github.com/wangguojun2018/AnnotationVerifier.git
```
Then, build the project
```bash
cd AnnotationVerifier
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j8
```

## Usage



bbox_verifier
```bash
cd build
./bbox_verifier -pcd ../data/bbox/pcd/ -label ../data/bbox/label/
```

semantic_verifier
```bash
cd build
./semantic_verifier -pcd ../data/semantic/pcd/ -label ../data/semantic/label/
```
## Folder structure

When loading a sequence, the data must be organized as follows:

<pre>
point cloud folder
├── pcd/             -- directory containing ".pcd" files with point clouds.   
├── label/           -- label directory containing ".label" or ".txt" for semantic or bbox annotations.  
├── pcd_verified/    -- will be generated if not present. 它包含筛选的正确的点云   
└── label_verified/  -- will be generated if not present. 它包含筛选的正确的标注   

</pre>

### 具体使用方法
按`a`键切换到上一帧，按`d`键切换到下一帧，如果标注正确合格，按`s`键保存，自动将`pcd`文件和相应的标注文件移动到`pcd_verified`和`label_verified`文件夹,最终在`pcd`和`label`文件夹剩余的所有的点云和标注为不合格。
