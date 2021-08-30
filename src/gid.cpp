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

  Git::Configuration const* active = config.getActiveGitConfiguration();
  // active is not null-checked as an exception will be raised if not found

  /* Call Git */
  system((active->command() + user_args).c_str());
  return 0;
}
