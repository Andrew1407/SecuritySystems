#pragma once
#include <string>
#include "../../Layer.hpp"
#include "../../../../tools.hpp"

class PasswordInputReg : public Layer {
  public:
    PasswordInputReg(Privilege, std::string);
    bool run() override;
    void next(Layer*&) override;
  private:
    Privilege userType;
    std::string name;
    std::string password;
};
