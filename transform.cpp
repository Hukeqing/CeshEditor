#include "transform.h"
#include <cmath>

double radians(GLfloat f)
{
    return double(f) * acos(-1) / 180;
}

Vector3 cross(const Vector3 &a, const Vector3 &b)
{
    return Vector3(a.y * b.z - b.y * a.z, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}

void Vector3::normalize()
{
    GLfloat size = GLfloat(sqrt(double(x * x + y * y + z * z)));
    x /= size;
    y /= size;
    z /= size;
}

Vector3 Vector3::operator+(const Vector3 &other) const
{
    Vector3 res(x, y, z);
    res.operator+=(other);
    return res;
}

void Vector3::operator+=(const Vector3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

Vector3 Vector3::operator-()
{
    Vector3 res(x, y, z);
    res.x = -res.x;
    res.y = -res.y;
    res.z = -res.z;
    return res;
}

Vector3 Vector3::operator-(const Vector3 &other) const
{
    Vector3 res(x, y, z);
    res.operator-=(other);
    return res;
}

void Vector3::operator-=(const Vector3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

QVector3D Vector3::data() const
{
    return QVector3D(x, y, z);
}

Transform::Transform() : position(Vector3(0, 0, 0)), rotation(Vector3(0, 0, 0)), scale(Vector3(1, 1, 1)),
                          forward(Vector3(0, 0, 1)),       up(Vector3(0, 1, 0)),  left(Vector3(1, 0, 0)) {}

void Transform::translate(const Vector3 &v)
{
    position += v;
}

void Transform::rotate(const Vector3 &v)
{
    rotation += v;
    apply();
}

void Transform::get_view_matrix(QMatrix4x4 &m) const
{
    m.lookAt(position.data(), (position + forward).data(), up.data());
}

void Transform::apply()
{
    forward.x = GLfloat(-sin(radians(rotation.y)));
    forward.y = GLfloat(sin(radians(rotation.x)));
    forward.z = GLfloat(cos(radians(rotation.x)) * cos(radians(rotation.y)));
    left = cross(Vector3(0, 1, 0), forward);
    up = cross(forward, left);
    left.normalize();
    up.normalize();
}
