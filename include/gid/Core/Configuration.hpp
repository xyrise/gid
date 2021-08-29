/* Configuration.hpp */

#ifndef GID_CORE_CONFIGURATION_HPP
#define GID_CORE_CONFIGURATION_HPP

#include <filesystem>
#include <string>
#include <vector>

#include "gid/Git/Configuration.hpp"


namespace Core {

  class Configuration {
    private:
      size_t active_git_configuration_index;
      std::vector<Git::Configuration> git_configurations;

    public:
      static std::filesystem::path configuration_path;

      /* Fills configuration class with parsed info */
      void parse(std::filesystem::path const p);

      Git::Configuration const* active_git_configuration() const;

  }; /* class Configuration */

}; /* namespace Core */


#endif /* GID_CORE_CONFIGURATION_HPP */
