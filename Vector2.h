#pragma once
#include <iostream>
#include <math.h>

//Basic mutable const-correct vector class with lots of operator overloading!
//David Moore, 2012, public domain!
class Vector2
{
    public:
        double x;
        double y;
        Vector2();
        Vector2(double a, double b);
        //vector addition and subtraction
        Vector2 operator+	(const Vector2& b) const;
        Vector2& operator+=	(const Vector2& b);
        Vector2 operator-	(const Vector2& b) const;
        Vector2& operator-=	(const Vector2& b);

        //multiplication/division on righthand side by scalar.
        //Division by zero throws an exception as expected.
        Vector2 operator*	(double b) const;
        Vector2& operator*=	(double b);
        Vector2 operator/	(double b) const;
        Vector2& operator/=	(double b);

        //dot product and cross product [ (x,y)*(a,b)==x*a+y*b, (x,y)^(a,b) = x*b-y*a ]
        //Delete if these aren't to your taste :)
        double operator*(const Vector2& b) const;
        double operator^(const Vector2& b) const;

        //calculate length and mutate to (x/length,y/length).
        void normalize();

        //return normalized version of the vector.
        Vector2 normalized() const;

        //return the length squared of the vector
        double length2() const;

        //return the length of the vector
        double length() const;
};

//Vector left multiplication
inline Vector2 operator* (double b, const Vector2& c);

//Writing a vector to an ostream as "(x,y)"
inline std::ostream& operator<<(std::ostream& out, const Vector2& c);

//Linearly interpolate between vec1 and vec2.
inline Vector2 lerp(double t, const Vector2& vec1, const Vector2& vec2);

//cubic interpolation between v1 and v2 with control points v0 v3.
inline Vector2 cerp(double t, const Vector2& vec0, const Vector2& vec1, const Vector2& vec2, const Vector2& vec3);

inline Vector2::Vector2() : x(0), y(0) { }
inline Vector2::Vector2(double a, double b) : x(a), y(b) { }
inline Vector2 Vector2::operator+ (const Vector2& b) const {
    return Vector2(x + b.x, y + b.y);
}
inline Vector2& Vector2::operator+= (const Vector2& b) {
    x = x + b.x;
    y = y + b.y;
    return *this;
}
inline Vector2 Vector2::operator-(const Vector2& b) const {
    return Vector2(x - b.x, y - b.y);
}
inline Vector2& Vector2::operator-=(const Vector2& b) {
    x = x - b.x;
    y = y - b.y;
    return *this;
}
inline double Vector2::operator* (const Vector2& b) const {
    return (x*b.x + y * b.y);
}
inline double Vector2::operator^(const Vector2& b) const {
    return (x*b.y - y * b.x);
}
inline Vector2 Vector2::operator* (double b) const {
    return Vector2(x*b, y*b);
}
inline Vector2& Vector2::operator*= (double b)
{
    x = x * b;
    y = y * b;
    return *this;
}
inline Vector2 Vector2::operator/ (double b) const {
    return Vector2(x / b, y / b);
}
inline Vector2& Vector2::operator/= (double b) {
    x = x / b;
    y = y / b;
    return *this;
}
inline void Vector2::normalize() {
    double dist = sqrt(x*x + y * y);
    x /= dist;
    y /= dist;
}
inline Vector2 Vector2::normalized() const {
    double dist = sqrt(x*x + y * y);
    return Vector2(x / dist, y / dist);
}
inline double Vector2::length() const {
    return sqrt(x*x + y * y);
}
inline double Vector2::length2() const {
    return x * x + y * y;
}

std::ostream& operator<<(std::ostream& out, const Vector2& c) {
    out << '(' << c.x << ',' << c.y << ')';
    return out;
}
Vector2 operator* (double b, const Vector2& c) {
    Vector2 temp;
    temp.x = c.x*b;
    temp.y = c.y*b;
    return temp;
}
