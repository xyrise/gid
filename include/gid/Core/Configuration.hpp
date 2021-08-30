/* Configuration.hpp */

#ifndef GID_CORE_CONFIGURATION_HPP
#define GID_CORE_CONFIGURATION_HPP

#include <filesystem>
#include <string>
#include <unordered_map>

#include "gid/Git/Configuration.hpp"


namespace Core {

  class Configuration {
    private:
      std::string active_git_configuration = "";
      std::unordered_map<std::string, Git::Configuration> git_configurations;

    public:
      static std::filesystem::path configuration_path;

      /* Fills configuration class with parsed info */
      void parse(std::filesystem::path const& p);

      void setActiveGitConfiguration(std::string const&);
      Git::Configuration const* getActiveGitConfiguration() const;

  }; /* class Configuration */

}; /* namespace Core */


#endif /* GID_CORE_CONFIGURATION_HPP */
