#include<iostream>
#include <limits>

#include "vector3.hh"
#include "camera.hh"

void Camera::init(const Vector3& globalUp, const Vector3& globalRight)
{
    Vector3 forward = Vector3(center_, objective_).normalize();
    Vector3 globalRef = globalUp.normalize();
    float cosAngle = dot(globalRef, forward);
    // deja orthogonaux pas besoin de calculer une projection
    if(cosAngle == 0)
    {
        up_ = globalUp;
        return;
    }
    //si les vecteur sont paralleles utiliser le vecteur right
    if(std::abs(cosAngle) == 1)
    {
        globalRef = globalRight.normalize();
        up_ = crossProduct(globalUp, globalRight);
        if (cosAngle == -1)
        {
            up_ = up_ * -1;
        }
        return;
    }
    Point3 intersect = Point3(0,0,0) + forward * cosAngle;
    up_ = Vector3(intersect, Point3(globalRef.x_, globalRef.y_, globalRef.z_)).normalize();
    if(dot(up_, globalRef) < 0)
    {
        up_ = up_ * - 1;
    }
}

Color Camera::computeColor(int x, int y, float z, const Triangle& tr)
{
    Vector3 vc = Vector3(center_, imagePlan[y * WIDTH + x]).normalize();
    Vector3 forward = Vector3(center_, objective_);
    float dist = z / dot(vc.normalize(), forward.normalize());
    Point3 intersect = imagePlan[y * WIDTH + x] + vc * dist;

    Color res = Color(0,0,0);
    Vector3 reflected = vc - tr.normal * 2.0 * dot(vc, tr.normal);
    reflected = reflected.normalize();
    for(const auto& light : lights)
    {
        Color effectiveColor = light.color_ * tr.color;
        Color ambientColor = effectiveColor * tr.ambient;
        Vector3 l = Vector3(intersect, light.center_);
        l = l.normalize();
        float angle = dot(l, tr.normal);
        if(angle < 0.)
        {
            continue;
        }
        Color diffuseColor = effectiveColor * tr.diffuse * angle;
        float factor = dot(reflected, l);
        factor = powf(factor, tr.shininess);
        if (dot(reflected, l) < 0.)
            factor = - factor;
        Color specularColor = light.color_ * tr.specular * factor;
        res = res + ((ambientColor + diffuseColor + specularColor));
    }

    return res;
}

void Camera::computeAllColors()
{
    for(int j = 0; j < HEIGHT; j++)
    {
        for(int i = 0; i < WIDTH; i++)
        {
            int index = j * WIDTH + i;
            if(depthBuffer[index] != std::numeric_limits<float>::max())
            {
                frameBuffer[index] = computeColor(i, j, depthBuffer[index], triangleHit[index]);
            }
        }
    }
}

void Camera::addShadow()
{
    Vector3 forward = Vector3(center_, objective_).normalize();
    for(int j = 0; j < HEIGHT; j++)
    {
        for(int i = 0; i <  WIDTH; i++)
        {
            int index = j * WIDTH + i;
            if(depthBuffer[index] >= std::numeric_limits<float>::max())
            {
                continue;
            }
            Vector3 v = Vector3(center_, imagePlan[index]).normalize();
            float cosAngle = dot(v, forward);
            float dist = (float)(depthBuffer[index] + zDist_) / cosAngle;
            Point3 target = center_ + v * dist;
            //FIXME: handle several lights

            Vector3 dirLight = Vector3(lights[0].center_, lights[0].objective_);
            Vector3 forwardLight = dirLight.normalize();
            Point3 projection = lights[0].projectPoint(target);
            Point2 p2LightIndex = lights[0].computePointCoordinate(projection);

            float angle = acos(dot(forwardLight, Vector3(lights[0].center_, projection).normalize()));
            if(p2LightIndex.x < 0 || p2LightIndex.x >= lights[0].width_ ||
               p2LightIndex.y < 0 || p2LightIndex.y >= lights[0].height_|| angle > lights[0].openAngleX_)
            {
                
                frameBuffer[index] = frameBuffer[index] * Color(0,0,0);
                continue;
            }
            int lightIndex = p2LightIndex.y * lights[0].width_ + p2LightIndex.x;
            Point3 imagePlanPointLight = lights[0].imagePlan[lightIndex];
            float distCamTgtLight = Vector3(lights[0].center_, target).norm();
            Vector3 vLight = Vector3(lights[0].center_, imagePlanPointLight).normalize();
            float cosAngleLight = dot(vLight, forwardLight);
            float distTgtLight = (lights[0].depthBuffer[lightIndex] + lights[0].zDist_) / cosAngleLight;


            if(distCamTgtLight - distTgtLight > 1)
            {
                frameBuffer[index] = frameBuffer[index] * Color(0.2,0.2,0.2);
            }
        }
    }
}
