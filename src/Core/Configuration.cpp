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
      if (current_line.length() >= 3u) {
        size_t split = current_line.find(":");
        if (split == std::string::npos) {
          if (current_line.find("start") != std::string::npos) {
            current_git_config = Git::Configuration();
          }
          else if (current_line.find("end") != std::string::npos) {
            this->git_configurations.push_back(current_git_config);
          }
          else if (current_line.find("default") != std::string::npos) {
            current_git_config = Git::Configuration();
            this->active_git_configuration_index =
              this->git_configurations.size();
          }
        }
        else {
          std::string key = current_line.substr(0, split);
          std::string value = current_line.substr(split + 1);

          if (key.find("user.name") != std::string::npos) {
            current_git_config.user_name = value;
          }
          else if (key.find("user.email") != std::string::npos) {
            current_git_config.user_email = value;
          }
          else if (key.find("commit.template") != std::string::npos) {
            current_git_config.commit_template = value;
          }
          else if (key.find("user.signingkey") != std::string::npos) {
            current_git_config.user_signingkey = value;
          }
          else if (key.find("ssh_key_path") != std::string::npos) {
            current_git_config.private_ssh_key = std::filesystem::path(value);
          }
        }
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
