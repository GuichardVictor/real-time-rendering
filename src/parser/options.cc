#include "options.hh"
#include "point3.hh"

#include <boost/program_options/errors.hpp>

void validate(boost::any& v, const std::vector<std::string>& values,
          Point3*, int)
{
    Point3 p{0.0, 0.0, 0.0};
    if (values.size() != 3)
    {
        throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value);
    }
    p.x_ = boost::lexical_cast<float>(values.at(0));
    p.y_ = boost::lexical_cast<float>(values.at(1));
    p.z_ = boost::lexical_cast<float>(values.at(1));

    v = p;
}

boost::program_options::variables_map parse_option(int& argc, char* argv[])
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("glut", "will render the scene in a glut window")
        ("time", "will print time taken to render a frame")
        ("with-shadow", "will render the scene with hard shadow (using shadow map algorithm)")
        ("with-aa", "will render the scene with an antialiasing method")
        ("width", boost::program_options::value<unsigned int>(), "window width (default: 500)")
        ("height", boost::program_options::value<unsigned int>(), "window height (default: 500)")
        ("obj-file", boost::program_options::value<std::string>()->required(), "obj file to use as scene")
        ("camera-pos", boost::program_options::value<Point3>(), "camera position (default: (1.5, 1.5, 2.5)) (usage: --camera-pos x y z)")
        ("camera-dir", boost::program_options::value<Point3>(), "camera direction (default: (1.4, 1.5, 2.4)) (usage: --camera-dir x y z)")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        std::exit(1);
    }

    try
    {
        boost::program_options::notify(vm);
    } catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        std::exit(1);
    }

    return vm;
}

