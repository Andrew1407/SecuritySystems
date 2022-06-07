#pragma once
#include <string>
#include <vector>
#include "../../Layer.hpp"

class NameInputLog : public Layer {
  public:
    NameInputLog();
    bool run() override;
    void next(Layer*&) override;
  private:
    std::string name;
    std::vector<std::string> usernames;
    void getUsernames();
    bool nameExists();
};
