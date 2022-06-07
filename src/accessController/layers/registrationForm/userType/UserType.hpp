#pragma once
#include "../../Layer.hpp"
#include "../../../../tools.hpp"

class UserType : public Layer {
  public:
    UserType();
    bool run() override;
    void next(Layer*&) override;
  private:
    Privilege type;
};
