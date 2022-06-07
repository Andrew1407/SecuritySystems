#pragma once
#include <string>
#include "../Layer.hpp"
#include "../../../tools.hpp"
#include "../../../dataparser/dataparser.hpp"

class DiskReader : public Layer {
  public:
    DiskReader(dataparser::User);
    bool run() override;
    void next(Layer*&) override;
  private:
    dataparser::User user;
    uint16_t modifiers;
    void handlePathAccess(std::string*);
};
