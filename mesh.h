#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <vector>
#include <QColor>
#include <fstream>
#include "transform.h"

using std::vector;
using std::fstream;

class Mesh
{
public:
    void init();
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

    void push_indice(GLuint *ind, GLuint len = 1);
    void erase_indice(GLuint pos);
    const vector<GLuint> &get_indice() const;
    const GLuint *get_indice(GLuint pos) const;
    GLuint get_indice_number() const;
    QString get_indice_name(GLuint index) const;

    void draw(const QMatrix4x4 &projection, const QMatrix4x4 &view);
    const GLuint *get_indice_data() const;
    GLuint get_indices_len() const;
    void change_indice(GLuint index, GLuint *value);
    Transform transform;                // 模型矩阵

    void writeCesh(fstream &out);
    void writeObj(fstream &out);
private:
    vector<GLfloat> vertices;           // 顶点信息
    vector<GLuint> indices;             // 面信息
    QOpenGLShaderProgram *program;      // 着色器
    QOpenGLBuffer vbo;                  // 缓冲
};

#endif // MESH_H
