#include<iostream>
#include <limits>

#include "vector3.hh"
#include "camera.hh"


int Camera::HEIGHT = 500;
int Camera::WIDTH = 500;

Color mix(Color a, Color b, float ratio)
{
    auto red =   (1 - ratio) * a.red   + ratio * b.red;
    auto green = (1 - ratio) * a.green + ratio * b.green;
    auto blue =  (1 - ratio) * a.blue  + ratio * b.blue;

    return Color(red, green, blue);
}

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
        auto l = Vector3(intersect, light.center_).normalize();
        float diff = dot(tr.normal,l);
        if (diff < 0)
            diff = 0;
        auto reflection = Vector3(-l.x_, -l.y_, -l.z_) - tr.normal;
        auto tmp = dot(reflection, l);
        if (tmp < 0)
            tmp = 0;
        float spec = powf(tmp, 100);

        float ambient_coef = 0.75;

        Color ambient = light.color_ * tr.color * ambient_coef;
        Color diffuse = light.color_ * tr.color * diff;
        Color specular = light.color_ * tr.color * spec;

        res = res + ambient + diffuse + specular;
    }

    return res;
}

void Camera::computeAllColors(bool with_antialiasing)
{
    float inf = std::numeric_limits<float>::max();
    for(int j = 0; j < HEIGHT; j++)
    {
        for(int i = 0; i < WIDTH; i++)
        {
            int index = j * WIDTH + i;
            frameBuffer[index] = Color(0.0, 0.0, 0.0);
            if(depthBuffer[index] != inf)
            {
                frameBuffer[index] = computeColor(i, j, depthBuffer[index], triangleHit[index]);
            }
        }
    }

    if (!with_antialiasing)
        return;

    std::vector<Color> post_frameBuffer = std::vector<Color>{frameBuffer.size()};

    for (int j = 0; j < HEIGHT; j++)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            int index = j * WIDTH + i;

            if (depthBuffer[index] == inf)
                continue;

            int prev = j * WIDTH + (i - 1);
            int next = j * WIDTH + (i + 1);
            int top = (j - 1) * WIDTH + i;
            int bot = (j + 1) * WIDTH + i;

            Color prev_c = Color(0, 0, 0);
            if (i - 1 >= 0 && depthBuffer[prev] != inf)
                prev_c = frameBuffer[prev];

            Color next_c = Color(0, 0, 0);
            if (i + 1 < WIDTH && depthBuffer[next] != inf)
                next_c = frameBuffer[next];

            Color top_c = Color(0, 0, 0);
            if (j - 1 >= 0 && depthBuffer[top] != inf)
                top_c = frameBuffer[top];

            Color bot_c = Color(0, 0, 0);
            if (j + 1 < HEIGHT && depthBuffer[bot] != inf)
                bot_c = frameBuffer[bot];

            auto row_c = mix(prev_c, next_c, 0.5);
            auto col_c = mix(top_c, bot_c, 0.5);
            auto avg_c = mix(row_c, col_c, 0.5);

            post_frameBuffer[index] = mix(frameBuffer[index], avg_c, 0.4);
        }
    }

    frameBuffer = post_frameBuffer;
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
               p2LightIndex.y < 0 || p2LightIndex.y >= lights[0].height_)
            {
                continue;
            }
            if( angle > lights[0].openAngleX_)
            {
                frameBuffer[index] = frameBuffer[index] * Color(0,0,0);
            }
            int lightIndex = p2LightIndex.y * lights[0].width_ + p2LightIndex.x;
            Point3 imagePlanPointLight = lights[0].imagePlan[lightIndex];
            float distCamTgtLight = Vector3(lights[0].center_, target).norm();
            Vector3 vLight = Vector3(lights[0].center_, imagePlanPointLight).normalize();
            float cosAngleLight = dot(vLight, forwardLight);
            float distTgtLight = (lights[0].depthBuffer[lightIndex] + lights[0].zDist_) / cosAngleLight;


            if(lights[0].triangleHit[lightIndex] != triangleHit[index] && distCamTgtLight - distTgtLight > 1)
            {
                frameBuffer[index] = frameBuffer[index] * Color(0.2,0.2,0.2);
            }

        }
    }
}
