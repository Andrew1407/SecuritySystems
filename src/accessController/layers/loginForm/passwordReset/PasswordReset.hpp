#pragma once
#include "../../Layer.hpp"
#include "../../../../dataparser/dataparser.hpp"

class PasswordReset : public Layer {
  public:
    PasswordReset(dataparser::User, uint, bool);
    bool run() override;
    void next(Layer*&) override;
  public:
    std::string input;
    dataparser::User user;
    uint necessaryLength;
    uint tries;
    bool keysExpired;

    void refreshPassword();
};
