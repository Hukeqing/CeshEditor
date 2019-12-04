#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <vector>

using std::vector;

class Mesh
{
public:
    void init(const GLfloat *ver, const GLuint verLen, const QString &verMod, GLuint *ind = nullptr, GLuint indLen = 0);
    void push_vertice(GLfloat *ver);
    void erase_vertice(GLuint pos);
    void push_indice(GLuint *ind);
    void erase_indice(GLuint pos);
    void draw(int w, int h);
    const GLuint *getIndices() const;
    GLuint getIndicesLen() const;
private:
    // 顶点信息
    vector<GLfloat> vertices;
    QString verticesMode;
    GLuint vertexsLen;
    // 面信息
    vector<GLuint> indices;
    GLuint indicesLen;
    // 着色器
    QOpenGLShaderProgram *program;
    // 缓冲
    QOpenGLBuffer vbo;
};

#endif // MESH_H
