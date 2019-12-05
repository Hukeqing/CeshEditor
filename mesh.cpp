#include "mesh.h"

void Mesh::init()
{
    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../CeshEditor/vcm.vsh");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../CeshEditor/vcm.fsh");
    program->link();

    vbo.create();
}
/**    vertice    **/
void Mesh::push_vertice(GLfloat *ver, GLuint len)
{
    for (GLuint i = 0; i < 6 * len; ++i)
        vertices.push_back(*(ver + i));
    vbo.bind();
    vbo.allocate(vertices.data(), int(vertices.size() * sizeof (GLfloat)));
    vbo.release();
}

void Mesh::erase_vertice(GLuint pos)
{
    vertices.erase(vertices.begin() + pos);
}

const vector<GLfloat> &Mesh::get_vertice() const
{
    return vertices;
}

const GLfloat *Mesh::get_vertice(GLuint pos) const
{
    return vertices.data() + pos * 6;
}

Vector3 Mesh::get_vertice_position(GLuint index) const
{
    Vector3 res;
    res.x = vertices[index * 6 + 0];
    res.y = vertices[index * 6 + 1];
    res.z = vertices[index * 6 + 2];
    return res;
}

QString Mesh::get_vertice_position_name(GLuint index) const
{
    QString res = "x: " + QString::number(double(vertices[index * 6 + 0])) + ", " +
            "y: " + QString::number(double(vertices[index * 6 + 1])) + ", " +
            "z: " + QString::number(double(vertices[index * 6 + 2]));
    return res;
}

QColor Mesh::get_vertice_color(GLuint index) const
{
    QColor res(int(vertices[index * 6 + 3] * 255),
            int(vertices[index * 6 + 4] * 255),
            int(vertices[index * 6 + 5] * 255));
    return res;
}

QString Mesh::get_vertice_color_name(GLuint index) const
{
    QString res = QString::number(double(vertices[index * 6 + 3] * 255)) + ", " +
            QString::number(double(vertices[index * 6 + 4] * 255)) + ", " +
            QString::number(double(vertices[index * 6 + 5] * 255));
    return res;
}

QString Mesh::get_vertice_css(GLuint index) const
{
    QString res = "background: rgb(" + get_vertice_color_name(index) + ");";
    if (vertices[index * 6 + 3] * 255 * 0.299f +
            vertices[index * 6 + 4] * 255 * 0.578f +
            vertices[index * 6 + 4] * 255 * 0.114f >= 192) {
        res += "color: black;";
    } else {
        res += "color: white;";
    }
    return res;
}

void Mesh::change_vertice(GLuint index, GLfloat *value)
{
    for (GLuint i = 0; i < 6; ++i)
        vertices[index * 6 + i] = *(value + i);
    vbo.bind();
    vbo.allocate(vertices.data(), int(vertices.size() * sizeof (GLfloat)));
    vbo.release();
}
/**    indice     **/
void Mesh::push_indice(GLuint *ind, GLuint len)
{
    for (GLuint i = 0; i < 3 * len; ++i)
        indices.push_back(*(ind + i));
}

void Mesh::erase_indice(GLuint pos)
{
    indices.erase(indices.begin() + pos);
}

const vector<GLuint> &Mesh::get_indice() const
{
    return indices;
}

QString Mesh::get_indice_name(GLuint index) const
{
    return QString::number(indices[index * 3 + 0]) + ", " +
           QString::number(indices[index * 3 + 1]) + ", " +
           QString::number(indices[index * 3 + 2]);
}

void Mesh::draw(const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
    vbo.bind();
    program->bind();
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    program->enableAttributeArray(0);
    program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof (GLfloat), 3, sizeof(GLfloat) * 6);
    program->enableAttributeArray(1);

    QMatrix4x4 model;
    model.translate(transform.position.x, transform.position.y, transform.position.z);
    model.rotate(transform.rotation.x, 1, 0, 0);
    model.rotate(transform.rotation.y, 0, 1, 0);
    model.rotate(transform.rotation.z, 0, 0, 1);
    program->setUniformValue("model", model);
    program->setUniformValue("projection", projection);
    program->setUniformValue("view", view);
}

const GLuint *Mesh::get_indice_data() const
{
    return indices.data();
}

GLuint Mesh::get_indices_len() const
{
    return GLuint(indices.size());
}
