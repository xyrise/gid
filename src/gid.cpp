/* gid.cpp */

#include <iostream>

#include "gid/Core/Configuration.hpp"
#include "gid/Git/Configuration.hpp"


int main(int argc, char * argv[]) {
  /* Parse user arguments */
  std::string user_args = "";
  for (int i = 1; i < argc; ++i) {
    user_args += " " + std::string(argv[i]);
  }

  /* Parse gid configuration */
  Core::Configuration config;
  config.parse(Core::Configuration::configuration_path);

  Git::Configuration const* active = config.active_git_configuration();
  if (active) {
    /* Call Git */
    system((active->command() + user_args).c_str());
  }
  return 0;
}
