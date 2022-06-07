#pragma once
#include "../../Layer.hpp"
#include "../../../../dataparser/dataparser.hpp"

class KeysRefreshSubmission : public Layer {
  public:
    KeysRefreshSubmission(dataparser::User);
    bool run() override;
    void next(Layer*&) override;
  public:
    dataparser::User user;
    char answer;
    void handleKeysRefreshment();
};

