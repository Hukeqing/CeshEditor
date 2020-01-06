#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <vector>
#include <QColor>
#include <iostream>
#include <QTextStream>
#include "transform.h"
#include "color.h"

using std::vector;

class Mesh
{
public:
    void init();
    // vertice
    void push_vertice(GLfloat *ver, GLuint len = 1);
    void erase_vertice(GLuint pos);
    const vector<GLfloat> &get_vertice() const;
    const GLfloat *get_vertice(GLuint pos) const;
    GLuint get_vertice_number() const;
    Vector3 get_vertice_position(GLuint index) const;
    QString get_vertice_position_name(GLuint index) const;
    QColor get_vertice_color(GLuint index) const;
    QString get_vertice_color_name(GLuint index) const;
    QString get_vertice_css(GLuint index) const;
    void change_vertice(GLuint index, GLfloat *value);
    Color get_vertice_color_class(GLuint index) const;
    // indice
    void push_indice(GLuint *ind, GLuint num = 1, GLuint len = 3);
    void erase_indice(GLuint pos);
    const vector<GLuint> &get_indice() const;
    const GLuint *get_indice(GLuint pos) const;
    GLuint get_indice_number() const;
    QString get_indice_name(GLuint index) const;
    void change_indice(GLuint index, GLuint *value);
    QString get_indice_3colorCup(GLuint index);
    // draw
    void draw(const QMatrix4x4 &projection, const QMatrix4x4 &view);
    void undraw();
    const GLuint *get_indice_data() const;
    GLuint get_indices_len() const;
    Transform transform;                // 模型矩阵
    // 2.0
    void writeCesh(QTextStream &out);
    void writeObj(QTextStream &out);
    void loadCesh(QTextStream &in);
    void clear();
private:
    vector<GLfloat> vertices;           // 顶点信息
    vector<GLuint> indices;             // 面信息
    QOpenGLShaderProgram *program;      // 着色器
    QOpenGLBuffer vbo;                  // 缓冲
};

#endif // MESH_H
