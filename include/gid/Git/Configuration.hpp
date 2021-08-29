/* Configuration.hpp */

#ifndef GID_GIT_CONFIGURATION_HPP
#define GID_GIT_CONFIGURATION_HPP

#include <filesystem>
#include <string>


namespace Git {

  class Configuration {
    public:
      /* Git Configuration */
      std::string user_name;
      std::string user_email;
      std::string commit_template;
      std::string user_signingkey;

      /* Relevant System Configuration */
      std::filesystem::path private_ssh_key;

      bool operator==(Configuration const&) const;

      std::string command() const;
  }; /* class Configuration */

}; /* namespace Gid */


#endif /* GID_GIT_CONFIGURATION_HPP */
