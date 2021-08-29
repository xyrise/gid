/* Configuration.cpp */

#include "gid/Core/Configuration.hpp"

#include <cstdlib>
#include <fstream>


namespace Core {

  std::filesystem::path Configuration::configuration_path =
    std::filesystem::path(getenv("HOME")) / ".config/gid/configuration.gid";

  void Configuration::parse(std::filesystem::path const p) {
    if (!std::filesystem::exists(p)) return;
    std::ifstream config_file(p.string());

    Git::Configuration current_git_config;

    std::string current_line;
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

      if (val.empty()) {
        if (key == "start") {
          current_git_config = Git::Configuration();
        }
        else if (key == "default") {
          current_git_config = Git::Configuration();
          this->active_git_configuration_index =
            this->git_configurations.size();
        }
        else if (key == "end") {
          this->git_configurations.push_back(
              current_git_config
          );
        }
      }
      else {
        if (key == "user.name")
          current_git_config.user_name = val;
        else if (key == "user.email")
          current_git_config.user_email = val;
        else if (key == "commit.template")
          current_git_config.commit_template = val;
        else if (key == "user.signingkey")
          current_git_config.user_signingkey = val;
        else if (key == "ssh_key_path")
          current_git_config.private_ssh_key = std::filesystem::path(val);
      }
    }
  }

  Git::Configuration const* Configuration::active_git_configuration() const {
    if (
        this->active_git_configuration_index >=
        this->git_configurations.size()
    )
      return nullptr;
    return &(this->git_configurations[this->active_git_configuration_index]);
  }

}; /* namespace Core */
