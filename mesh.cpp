#include "mesh.h"
#include <QOpenGLFunctions>

void Mesh::init(const GLfloat *ver, const GLuint verLen, const QString &verMod, GLuint *ind, GLuint indLen)
{
    vertexsLen = verLen;
    vertices = new GLfloat[vertexsLen];
    memcpy(vertices, ver, vertexsLen * sizeof (GLfloat));
    verticesMode = verMod;

    indicesLen = indLen;
    indices = new GLuint[indicesLen];
    memcpy(indices, ind, indicesLen * sizeof (GLuint));
    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../OpenGL/" + verticesMode + ".vsh");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../OpenGL/" + verticesMode + ".fsh");
    program->link();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, int(vertexsLen * sizeof (GLfloat)));
    vbo.release();
}

void Mesh::draw(int w, int h)
{
    program->bind();
    vbo.bind();
//    int tupleSize = 0;
//    int offset = 0;
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
//    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    program->enableAttributeArray(0);
    program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof (GLfloat), 3, sizeof(GLfloat) * 6);
    program->enableAttributeArray(1);

    QMatrix4x4 matrix;
    matrix.perspective(45.0f, GLfloat(w)/GLfloat(h), 0.1f, 100.0f);
    matrix.translate(0, 0, -3);
    matrix.rotate(-60, 0, 1, 0);  //绕Y轴逆时针旋转
    program->setUniformValue("matrix", matrix);

}

const GLuint *Mesh::getIndices() const
{
    return indices;
}

GLuint Mesh::getIndicesLen() const
{
    return indicesLen;
}
