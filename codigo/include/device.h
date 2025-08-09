#pragma once

class Device {
public:
  virtual void init();
  virtual void poll();
};
