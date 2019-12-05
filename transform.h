#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QOpenGLFunctions>
#include <QMatrix4x4>

struct Vector3 {
    GLfloat x, y, z;
    Vector3();
    Vector3(GLfloat x, GLfloat y, GLfloat z);
    void normalize();

    Vector3 operator+(const Vector3 &other) const;
    void operator+=(const Vector3 &other);
    Vector3 operator-(const Vector3 &other) const;
    Vector3 operator-();
    void operator-=(const Vector3 &other);
    QVector3D data() const;
};

class Transform {
public:
    Vector3 position, rotation, scale;
    Vector3 forward, up, left;
    Transform();
    void translate(const Vector3 &v);
    void rotate(const Vector3 &v);
    void get_view_matrix(QMatrix4x4 &m) const;
};

#endif // TRANSFORM_H
