#include "options.hh"

boost::program_options::variables_map parse_option(int& argc, char* argv[])
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("sdl", "will render the scene in a sdl window")
        ("time", "will print time taken to render a frame")
        ("with-shadow", "will render the scene with hard shadow (using shadow map algorithm)")
        ("obj-file", boost::program_options::value<std::string>()->required(), "obj file to use as scene")
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

