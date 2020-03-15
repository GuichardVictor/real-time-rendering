#include "vector3.hh"

Vector3 Vector3::operator*(const float &l) const
{
    return Vector3(this->x_ * l, this->y_ * l, this->z_ * l);
}

Vector3 Vector3::operator-(const Vector3 &v) const
{
    return Vector3(this->x_ - v.x_, this->y_ - v.y_, this->z_ - v.z_);
}

Vector3 Vector3::operator/(const float &l) const
{
    return Vector3(this->x_ / l, this->y_ / l, this->z_ / l);
}

std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
    return os << "Vector x = " << v.x_ << ", y = " << v.y_ << ", z = " << v.z_;
}

Vector3 Vector3::normalize() const {
    float dist = sqrtf(this->x_ * this->x_ + this->y_* this->y_ + this->z_ * this->z_);
    return Vector3(this->x_ / dist, this->y_ / dist, this->z_ / dist);
}

float dot(const Vector3 &a, const Vector3 &b) {
    return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}

float Vector3::norm() {
    return sqrtf(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
}

Vector3 crossProduct(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.y_ * b.z_ - b.y_ * a.z_, a.z_ * b.x_ - a.x_ * b.z_, a.x_ * b.y_ - a.y_ * b.x_);
}