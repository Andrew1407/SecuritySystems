#pragma once
#include <thread>
#include "./layers/Layer.hpp"
#include "./layers/inputMode/inputMode.hpp"

class AccessController {
  public:
    AccessController();
    ~AccessController();
    void login();
  private:
    Layer *layerPtr;
    std::thread questioningThread;
    InputMode *userInput;

    void questionsSending();
    void runVerifyListener();
};
