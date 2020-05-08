#include <vector>
#include <iostream>
#include <memory>

#include "camera.hh"
#include "triangle.hh"
#include "obj-parser.hh"
#include "options.hh"

#include "program.hh"
#include "shader.hh"
#include "texture.hh"
#include "gui.hh"


#include <chrono> 

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>

boost::program_options::variables_map OPTIONS;

Vector3 globalUp = Vector3(0,1,0);
Vector3 globalRight = Vector3(1,0,0);

std::vector<DirectionalLight> lights;

Camera c = Camera(Point3(1.5,2.5,1.5), Point3(1.4,2.5,1.5), globalUp, globalRight, 2.04, 2.04, 0.14);

bool init_done = false;

ObjParser objs = ObjParser();

std::shared_ptr<Texture> texture;
std::shared_ptr<GUI> gui;
std::shared_ptr<Program> program;

void handle_key(unsigned char key, int x, int y)
{
   
    float speed = 0.2;

    x = x;
    y = y;

    float x_translation = 0;
    float y_translation = 0;
    float z_translation = 0;

    if (key == 'd') { x_translation = speed; }
    if (key == 's') { z_translation = speed; }
    if (key == 'w') { z_translation = -speed; }
    if (key == 'a') { x_translation -= speed; }

    auto pos = c.center_;
    auto obj = c.objective_;

    pos.x_ += x_translation;
    pos.y_ += y_translation;
    pos.z_ += z_translation;

    obj.x_ += x_translation;
    obj.y_ += y_translation;
    obj.z_ += z_translation;

    c = Camera(pos, obj, globalUp, globalRight, 2.04, 2.04, 0.14);
    c.initObserver();
    c.lights = lights;
    c.lights[0].initObserver();

    glutPostRedisplay();
}


unsigned char* color_to_char(std::vector<Color>& cols)
{
    unsigned char* frame = new unsigned char[HEIGHT * WIDTH * 3];
    
    int i = 0;
    for (auto it = cols.rbegin(); it != cols.rend(); it++)
    {
        auto col = *it;
        col.clamp();
        frame[i++] = col.red * 255;
        frame[i++] = col.green * 255;
        frame[i++] = col.blue * 255;
    }

    return frame;
}

void renderScene(Camera& c, std::vector<Triangle>& objects)
{
    auto start = std::chrono::high_resolution_clock::now();

    c.initObserver();
    c.lights = lights;

    for(auto& obj : objects)
    {
        c.updateBuffer(obj);
    }
    for(auto& obj : objects)
    {
        c.lights[0].updateBuffer(obj);
    }
    c.lights[0].computeAllColors();

    c.computeAllColors(OPTIONS.count("with-aa") != 0);
    if (OPTIONS.count("with-shadow") != 0)
        c.addShadow();


    if (OPTIONS.count("time") != 0)
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "[FRAME][time] " << duration_ms.count() << " ms" << std::endl;
    }

    if (!init_done)
    {
        Image img(WIDTH, HEIGHT);
        img.pixels = c.frameBuffer;
        img.save("test.ppm");
    }

}
void display()
{
    renderScene(c, objs.triangles);

    auto buffer = c.frameBuffer;
    auto pixels = color_to_char(buffer);

    program->use();
    texture->update(HEIGHT, WIDTH, pixels);

    if (!init_done)
    {
        gui->attach_texture(*texture);

        gui->use_vertices(*program, "aPos");
        gui->use_texture_coords(*program, "aTexCoords");

        init_done = true;
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    texture->use();
    gui->draw();

    delete[] pixels;
    glutSwapBuffers();
}


void init_glut(int& argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(4,5);
  glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition ( 100, 100 );
  glutCreateWindow("Shader Programming");
  glutDisplayFunc(display);
  //glutMotionFunc(mouse_move);
  glutKeyboardFunc(handle_key);
}

bool init_glew()
{
  if (glewInit())
  {
    std::cerr << " Error while initializing glew";
    return false;
  }
  return true;
}

void init_GL() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glClearColor(0.2,0.2,0.2,1.0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);
}

int main(int argc, char* argv[])
{
    OPTIONS = parse_option(argc, argv);

    auto ret = objs.parse(OPTIONS["obj-file"].as<std::string>());
    if (!ret)
    {
        std::cout << "something went wrong\n";
    }

    lights.push_back(DirectionalLight(Color(1,1,1), {2.5,1.5, 2.5}, {2.4,1.5,2.4},
                                      Vector3(0,1,0), 2.04, WIDTH, HEIGHT, 0.14));

    auto position = Point3(1.5, 1.5, 2.5);
    auto objective = Point3(1.4, 1.5, 2.4);
    if (OPTIONS.count("camera-pos"))
        position = OPTIONS["camera-pos"].as<Point3>();
    if (OPTIONS.count("camera-dir"))
        objective = OPTIONS["camera-dir"].as<Point3>();

    c = Camera(position, objective, globalUp, globalRight, 2.04, 2.04, 0.14);

    c.initBuffer();
    c.lights = lights;
    c.lights[0].initBuffer();

    if (OPTIONS.count("glut") == 0)
    {
        renderScene(c, objs.triangles);
        return 0;
    }

    init_glut(argc, argv);
    init_glew();
    init_GL();


    auto gui_vertex = Shader("../src/shaders/gui_vertex.shd", GL_VERTEX_SHADER);
    auto gui_fragment = Shader("../src/shaders/gui_fragment.shd", GL_FRAGMENT_SHADER);

    texture = std::make_shared<Texture>();
    gui = std::make_shared<GUI>();

    program = std::make_shared<Program>(Program({gui_vertex, gui_fragment}));
    program->link();
    program->use();

    glutMainLoop();
}

