#include "mymath.hpp"

#define MY_PI 3.1415926

mm::mat4 operator*(const mm::mat4 left_mat, const mm::mat4 right_mat)
{
    mm::mat4 temp = mm::mat4();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                temp.m[i][j] += left_mat.m[i][k] * right_mat.m[k][j];
            }
        }
    }
    return temp;
}

mm::vec4 operator*(const mm::mat4& mat, const mm::vec4& v1)
{
    mm::vec4 temp = mm::vec4();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            temp.v[i] += mat.m[i][j] * v1.v[j];
        }
    }
    return temp;
}

mm::vec4 operator*(const float& num, const mm::vec4& v1)
{
    mm::vec4 temp = mm::vec4();
    for (int i = 0; i < 4; ++i)
    {
        temp.v[i] = num * v1.v[i];
    }
    return temp;
}

mm::vec4 operator*(const mm::vec4& v1, const float& num)
{
    mm::vec4 temp = mm::vec4();
    for (int i = 0; i < 4; ++i)
    {
        temp.v[i] = num * v1.v[i];
    }
    return temp;
}

mm::vec4 operator+(const mm::vec4& v1, const mm::vec4& v2)
{
    mm::vec4 temp;
    temp = mm::vec4(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2], v1.v[3] + v2.v[3]);
    return temp;
}

mm::vec3 operator-(const mm::vec3& v1, const mm::vec3& v2)
{
    mm::vec3 temp;
    temp.v[0] = v1.v[0] - v2.v[0];
    temp.v[1] = v1.v[1] - v2.v[1];
    temp.v[2] = v1.v[2] - v2.v[2];
    return temp;
}

mm::vec3 mm::vec3::operator-()
{
    mm::vec3 temp;
    temp.v[0] = -v[0];
    temp.v[1] = -v[1];
    temp.v[2] = -v[2];
    return temp;
}

float mm::vec3::dot(const vec3 &v1)
{
    return v[0] * v1.v[0] + v[1] * v1.v[1] + v[2] * v1.v[2];
}

float mm::vec3::norm()
{
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

mm::vec3 mm::vec3::normalized()
{
    float n=norm();
    v[0] = v[0]/n;
    v[1] = v[1]/n;
    v[2] = v[2]/n;
    return *this;
}

mm::vec3 mm::vec3::cross(const vec3& v1)
{
    return vec3(v[1]*v1.v[2]-v[2]*v1.v[1], v[2]*v1.v[0]-v[0]*v1.v[2], v[0]*v1.v[1]-v[1]*v1.v[0]);
}

mm::vec3::vec3()
{
    v[0]=v[1]=v[2]=0;
}

mm::vec3::vec3(float x, float y, float z)
{
    v[0]=x;
    v[1]=y;
    v[2]=z;
}

float mm::vec4::dot(const vec4 &v1)
{
    return v[0] * v1.v[0] + v[1] * v1.v[1] + v[2] * v1.v[2] + v[3] * v1.v[3];
}

mm::vec4::vec4()
{
    v[0]=v[1]=v[2]=v[3]=0;
}

mm::vec4::vec4(float x, float y, float z, float w)
{
    v[0]=x;
    v[1]=y;
    v[2]=z;
    v[3]=w;
}

mm::mat4::mat4()
{
    m[0][0] = m[1][1] = m[2][2] = m[3][3] =0.0f;
    m[0][1] = m[0][2] = m[0][3] = 0.0f;
    m[1][0] = m[1][2] = m[1][3] = 0.0f;
    m[2][0] = m[2][1] = m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = 0.0f;
}

mm::mat4::mat4(const float* a)
{
    m[0][0] = a[0];
    m[0][1] = a[1];
    m[0][2] = a[2];
    m[0][3] = a[3];
    
    m[1][0] = a[0];
    m[1][1] = a[1];
    m[1][2] = a[2];
    m[1][3] = a[3];

    m[2][0] = a[0];
    m[2][1] = a[1];
    m[2][2] = a[2];
    m[2][3] = a[3];

    m[3][0] = a[0];
    m[3][1] = a[1];
    m[3][2] = a[2];
    m[3][3] = a[3];
}

void mm::mat4::set_uni()
{
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    m[0][1] = m[0][2] = m[0][3] = 0.0f;
    m[1][0] = m[1][2] = m[1][3] = 0.0f;
    m[2][0] = m[2][1] = m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = 0.0f;
}

void mm::mat4::reset()
{
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    m[0][1] = m[0][2] = m[0][3] = 0.0f;
    m[1][0] = m[1][2] = m[1][3] = 0.0f;
    m[2][0] = m[2][1] = m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = 0.0f;
}

mm::mat4 mm::mat4::get_scale_matrix(float x, float y, float z)
{
    set_uni();
    m[0][0]=x;
    m[1][1]=y;
    m[2][2]=z;
    return *this;
}

mm::mat4 mm::mat4::get_translate_matrix(float x, float y, float z)
{
    set_uni();
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;
    return *this;
}

mm::mat4 mm::mat4::get_perspective_matrix(float fov, float ratio, float near, float far)
{
    set_uni();
    float l,b,n;
    float r,t,f;
    f = far;
    n = near;
    t = tan(fov/2 * MY_PI / 180)*n;
    b = -t;
    r = 2 * t * ratio;
    l = -r;

    m[0][0]=2*n/r-l;
    m[0][2]=(r+l)/r-l;
    m[1][1]=2*n/t-b;
    m[1][2]=(t+b)/(t-b);
    m[2][2]=-(n+f)/(n-f);
    m[2][3]=-2*(f*n)/(n-f);
    m[3][2]=-1;
    m[3][3]=0;

    return *this;
}

mm::mat4 mm::mat4::get_view_matrix(mm::vec3 &e, mm::vec3 &g)
{
    set_uni();
    vec3 t = vec3(0.0f,1.0f,0.0f);
    vec3 w,u,v;
    w = -g.normalized();;
    u = t.cross(w).normalized();
    v = w.cross(u);

    mat4 move;
    move.set_uni();
    move.get_translate_matrix(-e.v[0], -e.v[1], -e.v[2]);
    mat4 base;
    base.set_uni();
    base.m[0][0] = u.v[0];
    base.m[0][1] = u.v[1];
    base.m[0][2] = u.v[2];

    base.m[1][0] = v.v[0];
    base.m[1][1] = v.v[1];
    base.m[1][2] = v.v[2];

    base.m[2][0] = w.v[0];
    base.m[2][1] = w.v[1];
    base.m[2][2] = w.v[2];

    mat4 negateZ;
    negateZ.set_uni();
    negateZ.m[2][2] = -1;

    *this = negateZ * base * move;

    return *this;
}
mm::mat4 mm::mat4::get_viewport_matrix(float width, float height)
{
    set_uni();
    m[0][0]=width/2;
    m[1][1]=height/2;
    m[0][3]=width/2;
    m[1][3]=height/2;
    m[2][2]=0.5f;
    m[2][3]=-0.5f;
    return *this;
}


