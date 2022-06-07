#pragma once
#include "../Layer.hpp"
#include "../../../dataparser/dataparser.hpp"

class InputMode : public Layer {
  public:
    InputMode(dataparser::User);
    bool run() override;
    void next(Layer*&) override;
    void sendQuestion();
    void checkAnswer();
  private:
    dataparser::User user;
    std::string input;
    std::string expression;
    bool fnChech;
    bool isQuestioning;
    std::string answer;

    void getJournalData();
    void onFailure();
    std::string getStatus();
    void notifyOfExeption();
    void monitorUsers();
};
