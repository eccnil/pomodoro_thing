#pragma once

class Tempo {
public:
  struct Display {
    unsigned int bpm;
    unsigned int phase;
  };

private:
  unsigned int static inc, min_tempo, max_tempo, default_tempo;
  unsigned int bpm = default_tempo;
  int offset = 0;
  unsigned long current_time = 0l;
  unsigned int phase(long millis);
  long period_milis();
  long period_8th_milis();
  void adjust_tempo(int var);

public:
  Display show(long millis);
  void inc_tempo();
  void dec_tempo();
};
