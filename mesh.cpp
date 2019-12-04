#include "mesh.h"
#include <QOpenGLFunctions>

void Mesh::init(const GLfloat *ver, const GLuint verLen, const QString &verMod, GLuint *ind, GLuint indLen)
{
    vertexsLen = verLen;
    vertices.insert(vertices.end(), ver, ver + verLen);
    verticesMode = verMod;

    indicesLen = indLen;
    indices.insert(indices.end(), ind, ind + indLen);
    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../OpenGL/" + verticesMode + ".vsh");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../OpenGL/" + verticesMode + ".fsh");
    program->link();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.data(), int(vertexsLen * sizeof (GLfloat)));
    vbo.release();
}

void Mesh::draw(int w, int h)
{
    program->bind();
    vbo.bind();
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    program->enableAttributeArray(0);
    program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof (GLfloat), 3, sizeof(GLfloat) * 6);
    program->enableAttributeArray(1);

    QMatrix4x4 matrix;
    matrix.perspective(45.0f, GLfloat(w) / GLfloat(h), 0.1f, 100.0f);
    matrix.lookAt(QVector3D(0, 0, 0), QVector3D(0, 0, 1), QVector3D(0, 1, 0));
    matrix.translate(0, 0, 3);
    matrix.rotate(-60, 0, 1, 0);  //绕Y轴逆时针旋转
    matrix.rotate(-60, 0, 0, 1);  //绕Z轴逆时针旋转
    program->setUniformValue("matrix", matrix);
}

const GLuint *Mesh::getIndices() const
{
    return indices.data();
}

GLuint Mesh::getIndicesLen() const
{
    return indicesLen;
}
