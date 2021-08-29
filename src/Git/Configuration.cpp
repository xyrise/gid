/* Configuration.cpp */

#include "gid/Git/Configuration.hpp"


namespace Git {

  /* class Configuration */
  bool Configuration::operator==(Configuration const& other) const {
    return (
        this->user_name == other.user_name
        && this->user_email == other.user_email
        && this->commit_template == other.commit_template
        && this->user_signingkey == other.user_signingkey
        && this->private_ssh_key == other.private_ssh_key
    );
  }

  std::string Configuration::command() const {
    std::string result = "git";
    if (!this->user_name.empty())
      result.append(
          std::string(" -c user.name=")
          + this->user_name
      );
    if (!this->user_email.empty())
      result.append(
          std::string(" -c user.email=")
          + this->user_email
      );
    if (!this->commit_template.empty())
      result.append(
          std::string(" -c commit.template=\"")
          + this->commit_template
          + std::string("\"")
      );
    if (!this->user_signingkey.empty())
      result.append(
          std::string(" -c user.signingkey=")
          + this->user_signingkey
      );
    if (!this->private_ssh_key.empty())
      result.append(
          std::string(" -c core.sshCommand=\"ssh -i ")
          + this->private_ssh_key.string()
          + std::string("\" ")
      );
    return result;
  }

}; /* namespace Gid */
