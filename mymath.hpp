#pragma once
#include <cmath>
//----------------------------------------------------------------------------------------------------
//百分之百手作垃圾线代运算库，突出一个能用就行
//----------------------------------------------------------------------------------------------------
namespace mm
{
    //----------------------------------------------------------------------------------------------------
    //向量和矩阵的声明
    //----------------------------------------------------------------------------------------------------
    class vec3
    {
    private:
    public:
        float v[3];

        float dot(const vec3 &v1);
        float norm();
        vec3 normalized();
        vec3 cross(const vec3 &v1);
        vec3();
        vec3(float x, float y, float z);
        vec3 operator-();
    };

    class vec4
    {
    private:
    public:
        float v[4];

        float dot(const vec4 &v1);
        vec4();
        vec4(float x, float y, float z, float w);
    };

    class mat4
    {
    private:
    public:
        float m[4][4];

        mat4();
        mat4(const float* a);
        void set_uni();
        void reset();
        mat4 get_scale_matrix(float x, float y, float z);
        mat4 get_translate_matrix(float x, float y, float z);
        mat4 get_perspective_matrix(float fov, float ratio, float near, float far);
        mat4 get_view_matrix(vec3 &e, vec3 &g);
        mat4 get_viewport_matrix(float width, float height);
    };
}

mm::mat4 operator*(const mm::mat4 left_mat, const mm::mat4 right_mat);
mm::vec4 operator*(const mm::mat4& mat, const mm::vec4& v1);
mm::vec4 operator*(const float& num, const mm::vec4& v1);
mm::vec4 operator+(const mm::vec4& v1, const mm::vec4& v2);
mm::vec3 operator-(const mm::vec3& v1, const mm::vec3& v2);



















