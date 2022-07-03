#include <iostream>
#include <vector>
using namespace std;

//顶点数据
vector<float> vertex_array =
{
       
};
//顶点索引，指示哪些顶点构成一个三角形（图元）
vector<int> element_array = {};

int main()
{
    
//三个顶点组成一个图元

//准备好mvp矩阵，带着三个顶点走一遍顶点着色器

//得到变换后的顶点,这里要做齐次除法然后丢掉不要的三角形,传入光栅化器,插值得到片元坐标以及其他属性，深度测试，丢弃不需要的片元。

//每一个片元都走一遍片元着色器

//最后写入颜色缓冲

}