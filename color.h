#ifndef COLOR_H
#define COLOR_H
#include <QString>

class Color
{
public:
    int data[4];
    Color();
    Color(int r, int g, int b, int a = 255);
    Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);
    Color(float r, float g, float b, float a = 1);
    Color(const int *value, int len = 3);
    Color(const unsigned int *value, int len = 3);
    Color(const float *value, int len = 3);
    QString getpoint255() const;
    QString getpoint1() const;
    QString getrgb255() const;
    QString getrgb1() const;
    QString getwell() const;
    int red255() const;
    float red1() const;
    int green255() const;
    float green1() const;
    int blue255() const;
    float blue1() const;

    static QString get3ColorCup(const Color &a, const Color &b, const Color &c);
};

#endif // COLOR_H
