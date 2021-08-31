/* gid.cpp */

#include <iostream>
#include <string>

#include "gid/Core/Configuration.hpp"
#include "gid/Git/Configuration.hpp"


int main(int argc, char * argv[]) {
  /* Parse user arguments */
  std::string user_args = "";
  std::string config_name = "";
  /* Check for configuration name */
  if (argc > 1) {
    config_name = std::string(argv[1]);
  }
  for (int i = 2; i < argc; ++i) {
    user_args += " " + std::string(argv[i]);
  }

  /* Parse gid configuration */
  Core::Configuration config;
  config.parse(Core::Configuration::configuration_path);

  if (config.gitConfigurationExists(config_name))
    config.setActiveGitConfiguration(config_name);
  else
    user_args = config_name + user_args;

  Git::Configuration const* active = config.getActiveGitConfiguration();
  // active is not null-checked as an exception will be raised if not found

  /* Call Git */
  system((active->command() + user_args).c_str());
  return 0;
}
