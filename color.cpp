#include "color.h"

char getF(int value) {
    if (value < 10)
        return char(value) + '0';
    else
        return char(value - 10) + 'A';
}

QString getFF(int value) {
    return QString(getF(value / 16)) + getF(value % 16);
}

Color::Color() = default;

Color::Color(int r, int g, int b, int a)
{
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
}

Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    data[0] = int(r);
    data[1] = int(g);
    data[2] = int(b);
    data[3] = int(a);
}

Color::Color(float r, float g, float b, float a)
{
    data[0] = int(r * 255);
    data[1] = int(g * 255);
    data[2] = int(b * 255);
    data[3] = int(a * 255);
}

Color::Color(int *value, int len)
{
    for (int i = 0; i < len; ++i)
        data[i] = value[i];
    if (len == 3)
        data[3] = 255;
}

Color::Color(unsigned int *value, int len)
{
    for (int i = 0; i < len; ++i)
        data[i] = int(value[i]);
    if (len == 3)
        data[3] = 255;
}

Color::Color(float *value, int len)
{
    for (int i = 0; i < len; ++i)
        data[i] = int(value[i] * 255);
    if (len == 3)
        data[3] = 255;
}

QString Color::getpoint255() const
{
    return QString::number(red255()) + ", " + QString::number(green255()) + ", " + QString::number(blue255());
}

QString Color::getpoint1() const
{
    return QString::number(double(red1())) + ", " + QString::number(double(green1())) + ", " + QString::number(double(blue1()));
}

QString Color::getrgb255() const
{
    return "rgba(" + getpoint255() + ")";
}

QString Color::getrgb1() const
{
    return "rgba(" + getpoint1() + ")";
}

QString Color::getwell() const
{
    return "#" + getFF(data[0]) + getFF(data[1]) + getFF(data[2]);
}

int Color::red255() const
{
    return data[0];
}

float Color::red1() const
{
    return data[0] * 1.0f / 255;
}

int Color::green255() const
{
    return data[1];
}

float Color::green1() const
{
    return data[1] * 1.0f / 255;
}

int Color::blue255() const
{
    return data[2];
}

float Color::blue1() const
{
    return data[2] * 1.0f / 255;
}
