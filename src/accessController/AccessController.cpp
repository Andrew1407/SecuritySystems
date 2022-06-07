#include <iostream>
#include <chrono>
#include "AccessController.hpp"
#include "./layers/signOption/SignOption.hpp"
#include "../tools.hpp"

using namespace std;

AccessController::AccessController(): layerPtr(nullptr), userInput(nullptr) { }

AccessController::~AccessController() {
  if (this->layerPtr) delete this->layerPtr;
}

void AccessController::login() {
  this->layerPtr = new SignOption();
  while (true) {
    bool succeed = this->layerPtr->run();
    if (!succeed) continue;
    Layer *next = nullptr;
    this->layerPtr->next(next);
    delete this->layerPtr;
    this->layerPtr = next;

    if (!this->layerPtr) return;
    this->runVerifyListener();
  }

  this->questioningThread.detach();
}

void AccessController::runVerifyListener() {
  if (this->userInput) return;
  if (this->userInput = dynamic_cast<InputMode*>(this->layerPtr))
    this->questioningThread = thread(&AccessController::questionsSending, this);
}

void AccessController::questionsSending() {
  auto period = chrono::minutes(ANSWER_TIME_MIN);
  while (true) {
    this_thread::sleep_for(period);
    this->userInput->checkAnswer();
    this->userInput->sendQuestion();
  }
}
