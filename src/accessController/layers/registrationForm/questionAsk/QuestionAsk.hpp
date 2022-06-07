#pragma once
#include <string>
#include <string>
#include "../../Layer.hpp"
#include "../../../../tools.hpp"
#include "../../../../dataparser/dataparser.hpp"

class QuestionAsk : public Layer {
  public:
    QuestionAsk(Privilege, std::string, std::string);
    bool run() override;
    void next(Layer*&) override;
  private:
    Privilege userType;
    std::string name;
    std::string password;
    dataparser::User user;
    dataparser::questionsVec querstions;
    void makeUser();
};