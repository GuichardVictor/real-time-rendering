#include "light.hh"
#include "camera.hh"


Color DirectionalLight::computeColor(int x, int y, float z, const Triangle& tr)
{
    Vector3 vc = Vector3(center_, imagePlan[y * Camera::WIDTH + x]).normalize();
    Vector3 forward = Vector3(center_, objective_);
    float dist = z / dot(vc.normalize(), forward.normalize());
    Point3 intersect = imagePlan[y * Camera::WIDTH + x] + vc * dist;

    Color res = Color(0,0,0);
    Vector3 reflected = vc - tr.normal * 2.0 * dot(vc, tr.normal);
    reflected = reflected.normalize();
    Color effectiveColor = color_ * tr.color;
    Color ambientColor = effectiveColor * tr.ambient;
    Vector3 l = Vector3(intersect, center_);
    l = l.normalize();
    float angle = dot(l, tr.normal);
    if(angle < 0.)
    {
    return res;
    }
    Color diffuseColor = effectiveColor * tr.diffuse * angle;
    float factor = dot(reflected, l);
    factor = powf(factor, tr.shininess);
    if (dot(reflected, l) < 0.)
    factor = - factor;
    Color specularColor = color_ * tr.specular * factor;
    res = res + ((ambientColor + diffuseColor + specularColor));
    return res;
}

void DirectionalLight::computeAllColors()
{
    for(int j = 0; j < Camera::HEIGHT; j++)
    {
        for(int i = 0; i < Camera::WIDTH; i++)
        {
            int index = j * Camera::WIDTH + i;
            if(depthBuffer[index] != std::numeric_limits<float>::max())
            {
                frameBuffer[index] = computeColor(i, j, depthBuffer[index], triangleHit[index]);
            }
        }
    }
}
