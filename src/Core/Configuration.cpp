/* Configuration.cpp */

#include "gid/Core/Configuration.hpp"

#include <cstdlib>
#include <fstream>


namespace Core {

  std::filesystem::path Configuration::configuration_path =
    std::filesystem::path(getenv("HOME")) / ".config/gid/configuration.gid";

  void Configuration::parse(std::filesystem::path const& p) {
    if (!std::filesystem::exists(p)) return;
    std::ifstream config_file(p.string());

    std::string current_line, current_config;
    while (std::getline(config_file, current_line)) {
      bool split = false;
      int key_start, key_end, val_start, val_end;
      key_start = key_end = val_start = val_end = -1;
      for (int i = 0; i < current_line.size(); ++i) {
        if (current_line[i] != ' ') {
          if (current_line[i] == ':') split = true;
          else {
            if (!split) {
              if (key_start == -1) key_start = i;
              else key_end = i;
            }
            else {
              if (val_start == -1) val_start = i;
              else val_end = i;
            }
          }
        }
      }
      std::string key, val;
      if (key_start != key_end) key =
        current_line.substr(key_start, key_end + 1);
      else continue;
      if (val_start != val_end) val =
        current_line.substr(val_start, val_end + 1);

      if (!split) {
        if (key == "end") current_config = "";
      }
      else {
        if (key == "start") {
          this->git_configurations.emplace(val, Git::Configuration());
          current_config = val;
        }
        else if (key == "default") {
          this->git_configurations.emplace(val, Git::Configuration());
          current_config = val;
          this->active_git_configuration = val;
        }
        else if (!current_config.empty()) {
          if (key == "user.name")
            this->git_configurations[current_config].user_name = val;
          else if (key == "user.email")
            this->git_configurations[current_config].user_email = val;
          else if (key == "commit.template")
            this->git_configurations[current_config].commit_template = val;
          else if (key == "user.signingkey")
            this->git_configurations[current_config].user_signingkey = val;
          else if (key == "ssh_key_path")
            this->git_configurations[current_config].private_ssh_key =
              std::filesystem::path(val);
        }
      }
    }
  }

  bool Configuration::gitConfigurationExists(std::string const& name) {
    return this->git_configurations.count(name);
  }

  void Configuration::setActiveGitConfiguration(std::string const& name) {
    this->active_git_configuration = name;
  }
  Git::Configuration const* Configuration::getActiveGitConfiguration() const {
    return &(this->git_configurations.at(this->active_git_configuration));
  }

}; /* namespace Core */
