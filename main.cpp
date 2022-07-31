#include <iostream>
#include <vector>
#include "mymath.hpp"
#include <fstream>
using namespace mm;

//----------------------------------------------------------------------------------------------------
//一些参数
//----------------------------------------------------------------------------------------------------
#define SCR_WIDTH 800.0f
#define SCR_HEIGHT 600.0f

//----------------------------------------------------------------------------------------------------
//模型数据结构
//----------------------------------------------------------------------------------------------------
class Triangle
{
public:
    std::vector<vec4> points;
    std::vector<vec4> colors;

    Triangle()
    {
        points.push_back(vec4(0,0,0,1));
        points.push_back(vec4(0,0,0,1));
        points.push_back(vec4(0,0,0,1));
    }
    Triangle(vec4 v1, vec4 v2, vec4 v3)
    {
        points.push_back(v1);
        points.push_back(v2);
        points.push_back(v3);
    }
    Triangle(const Triangle& t)
    {
        points = t.points;
        colors = t.colors;
    }
};

//----------------------------------------------------------------------------------------------------
//顶点着色器
//----------------------------------------------------------------------------------------------------
vec4 vertex_shader(vec4 point,const mat4& m,const mat4& v,const mat4& p)
{
    //直接组装mvp矩阵
    mat4 mv = v * m;
    mat4 mvp = p * mv;

    //假装这是opengl(=~ω~=)
    vec4 gl_position;
    gl_position = mvp * point;
    return gl_position;
}

//----------------------------------------------------------------------------------------------------
//片元着色器
//----------------------------------------------------------------------------------------------------
vec4 fragment_shader(const Triangle &t)
{
    vec4 gl_color;
    gl_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    return gl_color;
}


//----------------------------------------------------------------------------------------------------
//杂七杂八的函数
//----------------------------------------------------------------------------------------------------

//判断三角形要不要丢掉或者裁剪
bool out_of_view_volume(Triangle &t)
{
    bool flag = false;
    float w=t.points[0].v[3];
    for(int i=0;i<3;++i)
    {
        if(!(-w < t.points[i].v[0] && t.points[i].v[0] < w 
             -w < t.points[i].v[1] && t.points[i].v[1] < w
             -w < t.points[i].v[2] && t.points[i].v[2] < w))
            {
                return true;
            }
    } 
    return flag;
}

//一点小优化，找三角形的包围盒
//返回值按顺序分别是 min_y, max_y, min_x, max_x
vec4 find_boundingbox(Triangle &t)
{
    float min_x=10000, max_x=0, min_y=10000, max_y=0;
    for(int i=0;i<3;i++)
    {
        if(t.points[i].v[0]>max_x)
            max_x=t.points[i].v[0];
        if(t.points[i].v[0]<min_x)
            min_x=t.points[i].v[0];
        if(t.points[i].v[1]>max_y)
            max_y=t.points[i].v[1];
        if(t.points[i].v[1]<min_y)
            min_y=t.points[i].v[1];
    }
    return vec4(min_y, max_y, min_x, max_x);
}

//用像素点的中心对三角形进行采样
bool insideTriangle(float x, float y,const std::vector<vec4> &points)
{
    vec3 a,b,c;
    //只保留x,y的部分，z设置为1.方便后续叉乘运算
    a = vec3(points[0].v[0],points[0].v[1], 1.0f);
    b = vec3(points[1].v[0],points[1].v[1], 1.0f);
    c = vec3(points[2].v[0],points[2].v[1], 1.0f);

    vec3 pa_b = a-b;
    vec3 pb_c = b-c;
    vec3 pc_a = c-a;
    vec3 pa = vec3(points[0].v[0]-x, points[0].v[1]-y, 1.0f);
    vec3 pb = vec3(points[1].v[0]-x, points[1].v[1]-y, 1.0f);
    vec3 pc = vec3(points[2].v[0]-x, points[2].v[1]-y, 1.0f);
    if(pa.cross(pa_b).v[2] < 0 && pb.cross(pb_c).v[2] < 0 && pc.cross(pc_a).v[2] < 0)
        return true;
    return false;
}

//计算重心坐标
vec3 compute_barycentric2D(float x, float y,const std::vector<vec4> &t)
{
    float xa,xb,xc,ya,yb,yc;
    xa=t[0].v[0];
    ya=t[0].v[1];
    xb=t[1].v[0];
    yb=t[1].v[1];
    xc=t[2].v[0];
    yc=t[2].v[1];

    float a = (-(x-xb)*(yc-yb)+(y-yb)*(xc-xb)) / (-(xa-xb)*(yc-yb)+(ya-yb)*(xc-xb));
    float b = (-(x-xc)*(ya-yc)+(y-yc)*(xa-xc)) / (-(xb-xc)*(ya-yc)+(yb-yc)*(xa-xc));
    float c = 1-a-b;

    return vec3(a,b,c);
}

//计算片元对应的像素索引
int get_index(float x, float y)
{
    //int ind = (SCR_HEIGHT-y)*SCR_WIDTH - x;
    int ind = y*SCR_WIDTH + x;
    return ind;
}


//==========================================================================================
//通过颜色缓冲数据生成bmp图象文件
//==========================================================================================
void generate_img(const std::vector<vec4> &color_buf)
{
    int m_width = SCR_WIDTH;
    int m_height = SCR_HEIGHT;
    std::ofstream f;
    f.open("a.bmp", std::ios::out | std::ios::binary);

    if(!f.is_open())
    {
        std::cout << "文件创建失败" << std::endl;
        return;
    }

    unsigned char bmpPad[3] = {0,0,0};
    const int paddingAmount = ((4 - m_width%4)%4);

    const int fileHeaderSize = 14;
    const int informationHearSize = 40;
    const int fileSize = fileHeaderSize + informationHearSize + m_width * m_height * 3 + paddingAmount * m_height;

    unsigned char fileHeader[fileHeaderSize];

    //File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    //File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    fileHeader[10] = fileHeaderSize + informationHearSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHearSize];

    informationHeader[0] = informationHearSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;

    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;

    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;

    informationHeader[12] = 1;
    informationHeader[13] = 0;
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), informationHearSize);

    for(int y=m_height-1; y>=0; y--)//这里看着这么奇怪是因为当时往颜色缓冲里写数据的时候是一排一排写的。而图像最顶端的一排在数组的最后一排。
    {
        for(int x=0; x<m_width; ++x)
        {
            int ind = get_index(x, y);
            unsigned char r = static_cast<unsigned char>(color_buf[ind].v[0] * 255.0f);
            unsigned char g = static_cast<unsigned char>(color_buf[ind].v[1] * 255.0f);
            unsigned char b = static_cast<unsigned char>(color_buf[ind].v[2] * 255.0f);

            unsigned char color[] = {r, g, b};
            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }

    f.close();

    std::cout << "文件创建" << std::endl;
}


//==========================================================================================
//渲染管线由此开始
//==========================================================================================
int main()
{
//三个顶点组成一个图元
Triangle triangle0({0,0,0,1},{1,1,0,1},{0,1,0,1});
triangle0.colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
triangle0.colors.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
triangle0.colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));

Triangle triangle1({1,0,-2,1},{1,1,-2,1},{0,1,-2,1});
triangle1.colors.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
triangle1.colors.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
triangle1.colors.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));

//把三角形填入数组
std::vector<Triangle> triangles;
triangles.push_back(triangle0);
triangles.push_back(triangle1);

//准备相机属性
vec3 eye_pos = vec3(0,0,10);
vec3 gaze_vec = vec3(0,0,-1);

//准备颜色缓冲和深度缓冲
std::vector<vec4> col_buf(SCR_WIDTH*SCR_HEIGHT, vec4(0.1f, 0.1f, 0.1f, 0.1f));
std::vector<float> dep_buf(SCR_WIDTH*SCR_HEIGHT, 1);
//col_buf.resize(SCR_WIDTH*SCR_HEIGHT);
//dep_buf.resize(SCR_WIDTH*SCR_HEIGHT);

for(std::vector<Triangle>::iterator it = triangles.begin();it!=triangles.end();it++)
{
    Triangle triangle = *it;
    //准备好mvp矩阵，带着三个顶点走一遍顶点着色器
    mat4 model, view, projection;
    model.set_uni();
    view.get_view_matrix(eye_pos, gaze_vec);
    projection.get_perspective_matrix(70.0f, 4/3, 0.1f, 1000.0f);

    //view space下的三角形坐标
    std::vector<vec4> view_tri_pos;
    view_tri_pos = triangle.points;

    //顶点着色器遍历三角形每一个顶点
    for(int i=0;i<3;++i)
    {
        //triangle内数据更新为经过mvp变换后但是还没有进行齐次除法的顶点坐标
        triangle.points[i] = vertex_shader(triangle.points[i], model, view, projection);
        //获得view space下的三角形坐标，用于在光栅化阶段插值得到shading_point
        mat4 mv = view * model;
        view_tri_pos[i] = mv * view_tri_pos[i];
    }

    //后续大致思路：得到变换后的顶点,这里要做齐次裁剪然后丢掉不要的三角形。没被裁剪的三角形传入光栅化器,插值得到片元坐标以及其他属性，深度测试，丢弃不需要的片元。
    //齐次裁剪
    if(out_of_view_volume(triangle))
    {
        //丢掉图元或者裁剪也行,然后直接开始处理下一个三角形
    }

    //开始光栅化流程
    //齐次除法
    for(int i=0;i<3;++i)
    {
        float w = triangle.points[i].v[3];
        triangle.points[i].v[0] /= w;
        triangle.points[i].v[1] /= w;
        triangle.points[i].v[2] /= w;
        //w分量留着后面做插值矫正
    }

    //准备viewport矩阵
    mat4 vp;
    vp.get_viewport_matrix(SCR_WIDTH,SCR_HEIGHT);
    //旧三角形的w分量用于做插值矫正
    Triangle new_triangle = Triangle(triangle);
    for(int i=0;i<3;++i)
    {
        //新三角形在NDC空间下的坐标为(x,y,z,1),把w分量设置为1是为了视口变换矩阵能够正确地应用
        new_triangle.points[i].v[3] /= triangle.points[i].v[3];
    }
    //三角形由NDC转到屏幕空间
    new_triangle.points[0] = vp * new_triangle.points[0];
    new_triangle.points[1] = vp * new_triangle.points[1];
    new_triangle.points[2] = vp * new_triangle.points[2];
    for(int i=0;i<3;++i)
    {
        //把旧三角形的w分量(-w是顶点在view space下的深度值)传给新三角形，打包在一起，方便光栅化器做插值矫正
        new_triangle.points[i].v[3] = triangle.points[i].v[3];
    }

    //生成fragments
    vec4 boundingbox = find_boundingbox(new_triangle);
    //列是x，行是y
    for(int frag_row=int(boundingbox.v[0]); frag_row<=int(boundingbox.v[1]); ++frag_row)
    {
        for(int frag_col=int(boundingbox.v[2]); frag_col<=int(boundingbox.v[3]); ++frag_col)
        {
            if(insideTriangle(float(frag_col+0.5), float(frag_row+0.5), new_triangle.points))
            {
                //计算重心坐标
                vec3 bary_cor = compute_barycentric2D(frag_col+0.5, frag_row+0.5, new_triangle.points);
                float alpha = bary_cor.v[0];
                float beta = bary_cor.v[1];
                float gamma = bary_cor.v[2];

                //new_triangle.points[i].v[3] is the vertex view space depth value z.
                //Z is interpolated view space depth for the current pixel
                //zp is depth between zNear and zFar, used for z-buffer
                //透视插值矫正,可以让我们对任何属性以view space中的比例进行插值

                //像素对应在view space 的深度值
                float z = 1.0f / (alpha / new_triangle.points[0].v[3] + beta / new_triangle.points[1].v[3] + gamma / new_triangle.points[2].v[3]);
                //使用screen space的重心坐标对NDC中三角形的深度进行插值
                float zp = alpha * new_triangle.points[0].v[2] / new_triangle.points[0].v[3] + beta * new_triangle.points[1].v[2] / new_triangle.points[1].v[3] + gamma * new_triangle.points[2].v[2] / new_triangle.points[2].v[3];
                //插值矫正
                zp *= z;//用于比较的深度值，我们认为0为最近处，1为无限远


                int ind=get_index(frag_col, frag_row);
                //深度测试
                if(dep_buf[ind] > zp)
                {
                    //插值view space顶点坐标
                    vec4 shading_coord = alpha * view_tri_pos[0] * (1/new_triangle.points[0].v[3]) + beta * view_tri_pos[1] * (1/new_triangle.points[1].v[3]) + gamma * view_tri_pos[2] * (1/new_triangle.points[2].v[3]);
                    shading_coord = shading_coord * z;
                    //插值顶点的颜色属性
                    vec4 frag_color = alpha * new_triangle.colors[0] * (1/new_triangle.points[0].v[3]) + beta * new_triangle.colors[1] * (1/new_triangle.points[1].v[3]) + gamma * new_triangle.colors[2] * (1/new_triangle.points[2].v[3]);
                    frag_color = frag_color * z;
                    //获取fragment_shader计算的颜色，
                    //vec4 frag_color = fragment_shader(new_triangle);
                    //写入颜色缓冲
                    col_buf[get_index(frag_col, frag_row)] = frag_color;
                    //更新深度缓冲
                    dep_buf[ind] = zp;
                }
            }
        }
    }
}

//生成bmp图像文件
generate_img(col_buf);

/*
//测试代码
for(int i=0;i<SCR_HEIGHT;++i)
{
    for(int j=0;j<SCR_WIDTH;++j)
    {
        if(col_buf[get_index(i,j)].v[0] == 1.0f)
            std::cout << 1;
        else 
            std::cout << 0;
    }
    std::cout << std::endl; 
}
*/

return 0;
}
