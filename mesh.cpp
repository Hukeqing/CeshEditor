#include "mesh.h"
#include "defination.h"

using std::endl;

void Mesh::init()
{
    program = new QOpenGLShaderProgram;
    const char *vs = R"(#version 330 core
                        layout (location = 0) in vec3 aPos;
                        layout (location = 1) in vec3 aColor;

                        uniform mat4 projection;
                        uniform mat4 view;
                        uniform mat4 model;

                        out vec3 ourColor;

                        void main()
                        {
                            gl_Position = projection * view * model * vec4(aPos, 1.0f);
                            ourColor = aColor;
                        })";
    const char *fs = R"(#version 330 core
                        out vec4 FragColor;
                        in vec3 ourColor;

                        void main()
                        {
                            FragColor = vec4(ourColor, 1.0);
                        })";
//    program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../CeshEditor/vcm.vsh");
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
//    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../CeshEditor/vcm.fsh");
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
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
    vertices.erase(vertices.begin() + pos * 6, vertices.begin() + (pos + 1) * 6);
    vbo.bind();
    vbo.allocate(vertices.data(), int(vertices.size() * sizeof (GLfloat)));
    vbo.release();
}

const vector<GLfloat> &Mesh::get_vertice() const
{
    return vertices;
}

const GLfloat *Mesh::get_vertice(GLuint pos) const
{
    return vertices.data() + pos * 6;
}

GLuint Mesh::get_vertice_number() const
{
    return GLuint(vertices.size() / 6);
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

Color Mesh::get_vertice_color_class(GLuint index) const
{
    return Color(vertices.data() + index * 6 + 3);
}
/**    indice     **/
void Mesh::push_indice(GLuint *ind, GLuint num, GLuint len)
{
    for (GLuint i = 0; i < num * len; ++i)
        indices.push_back(*(ind + i));
}

void Mesh::erase_indice(GLuint pos)
{
    indices.erase(indices.begin() + pos * 3, indices.begin() + (pos + 1) * 3);
}

GLuint Mesh::get_indice_number() const
{
    return GLuint(indices.size() / 3);
}

const vector<GLuint> &Mesh::get_indice() const
{
    return indices;
}

const GLuint *Mesh::get_indice(GLuint pos) const
{
    return indices.data() + pos * 3;
}

QString Mesh::get_indice_name(GLuint index) const
{
    return QString::number(indices[index * 3 + 0]) + ", " +
           QString::number(indices[index * 3 + 1]) + ", " +
           QString::number(indices[index * 3 + 2]);
}

void Mesh::change_indice(GLuint index, GLuint *value)
{
    for (GLuint i = 0; i < 3; ++i)
        indices[index * 3 + i] = *(value + i);
}

QString Mesh::get_indice_3colorCup(GLuint index)
{
    return  Color::get3ColorCup(get_vertice_color_class(indices[index * 3 + 0]),
                                get_vertice_color_class(indices[index * 3 + 1]),
                                get_vertice_color_class(indices[index * 3 + 2]));
}

/**    draw     **/
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

void Mesh::undraw()
{
    vbo.release();
    program->release();
}

const GLuint *Mesh::get_indice_data() const
{
    return indices.data();
}

GLuint Mesh::get_indices_len() const
{
    return GLuint(indices.size());
}

void Mesh::clear()
{
    vertices.clear();
    indices.clear();
    vbo.bind();
    vbo.allocate(vertices.data(), int(vertices.size() * sizeof (GLfloat)));
    vbo.release();
}

void Mesh::writeCesh(std::fstream &out)
{
    out << "Cesh file, Copyleft @ 2019 mauve" << endl;
    out << VERSION << endl;
    char *verticesBegin = reinterpret_cast<char *>(vertices.data());
    char *verticesEnd = reinterpret_cast<char *>(vertices.data() + vertices.size());
    out << std::distance(verticesBegin, verticesEnd) << endl;
    auto iter = verticesBegin;
    while (iter != verticesEnd) {
        out << *iter;
        iter++;
    }
    char *indicesBegin = reinterpret_cast<char *>(vertices.data());
    char *indicesEnd = reinterpret_cast<char *>(vertices.data() + vertices.size());
    out << std::distance(indicesEnd, indicesBegin) << endl;
    iter = indicesBegin;
    while (iter != indicesEnd) {
        out << *iter;
        iter++;
    }
}
