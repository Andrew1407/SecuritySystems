#pragma once

class Layer {
  public:
    virtual bool run() = 0;
    virtual void next(Layer *&next_ptr) = 0;
};
