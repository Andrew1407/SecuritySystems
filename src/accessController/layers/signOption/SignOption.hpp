#pragma once
#include "../Layer.hpp"
#include "../../../tools.hpp"

class SignOption : public Layer {
  public:
    SignOption();
    bool run() override;
    void next(Layer*&) override;
  private:
    bool registration;
    uint signedUsers;
    void checkSignLimit();
};
