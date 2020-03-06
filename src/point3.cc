#include "point3.hh"
#include "vector3.hh"

std::ostream &operator<<(std::ostream &os, const Point3 &p)
{
    return os << "Point x = " << p.x_ << " ,y = " << p.y_ << " ,z = " << p.z_;
}

float Point3::getX() const {
    return x_;
}

float Point3::getY() const {
    return y_;
}

float Point3::getZ() const {
    return z_;
}

Point3 Point3::operator-(const Point3 &other) const{
    return Point3(this->x_ - other.getX(), this->y_ -other.getY(), this->z_ -other.getZ());
}

Point3 Point3::operator+(const Vector3 &v) const{
    return Point3(this->x_ + v.getX(), this->y_ + v.getY(), this->z_ + v.getZ());
}