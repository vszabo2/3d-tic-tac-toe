#include <boost/program_options.hpp>

#include "ofApp.h"
#include "ofMain.h"

namespace po = boost::program_options;

void ParseConfigFile(const char* config_file_path,
                     vszabo2ttt::GameConfig* config) {
    po::options_description desc;
    desc.add_options()
        ("sideLength", po::value<unsigned short>(&(config->side_length)),
         "The number of slots per line")
        ("playerCount", po::value<unsigned short>(&(config->player_count)),
         "The number of players")
        ("playerIndex", po::value<unsigned short>(&(config->player_index)),
         "The index of this player")
        ("myPort", po::value<unsigned short>(&(config->my_port)),
         "The port on which to listen")
        ("nextAddress", po::value<std::string>(&(config->next_address)),
         "The IP address of the next player")
        ("nextPort", po::value<unsigned short>(&(config->next_port)),
         "The port on which the next player is listening");

    auto parsed_options = po::parse_config_file(config_file_path, desc, true);
    po::variables_map vmap;
    po::store(parsed_options, vmap);
    po::notify(vmap);
}

//========================================================================
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config file name>" << std::endl;
        return 3;
    }

    vszabo2ttt::GameConfig config;
    ParseConfigFile(argv[1], &config);

    ofSetupOpenGL(1024, 768, OF_WINDOW);
    ofRunApp(new vszabo2ttt::ofApp(config));
}
