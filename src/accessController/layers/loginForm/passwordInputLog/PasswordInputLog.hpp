#pragma once
#include <string>
#include "../../Layer.hpp"
#include "../../../../dataparser/dataparser.hpp"

class PasswordInputLog : public Layer {
  public:
    PasswordInputLog(dataparser::User);
    bool run() override;
    void next(Layer*&) override;
  private:
    dataparser::User user;
    std::string passwordInput;
    uint tries;
    bool passwdExpired;
    bool keysExpired;
    uint necessaryLength;
};
