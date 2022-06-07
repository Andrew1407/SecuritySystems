#pragma once
#include <string>
#include <vector>
#include "../../Layer.hpp"
#include "../../../../tools.hpp"

class NameInputReg : public Layer {
  public:
    NameInputReg(Privilege);
    bool run() override;
    void next(Layer*&) override;
  private:
    Privilege userType;
    std::string name;
    std::vector<std::string> usernames;
    void getUsernames();
    bool nameExists();
};
