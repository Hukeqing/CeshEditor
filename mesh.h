#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Mesh
{
public:
    void init(const GLfloat *ver, const GLuint verLen, const QString &verMod, GLuint *ind = nullptr, GLuint indLen = 0);
    void draw(int w, int h);
    const GLuint *getIndices() const;
    GLuint getIndicesLen() const;
private:
    GLfloat *vertices;
    QString verticesMode;
    GLuint vertexsLen;
    GLuint *indices;
    GLuint indicesLen;
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
};

#endif // MESH_H
