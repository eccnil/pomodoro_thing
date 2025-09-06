#pragma once

class Tempo {
public:
  struct Display {
    int bpm;
    int phase;
  };

private:
  int static inc, min_tempo, max_tempo, default_tempo;
  int bpm = default_tempo;
  int offset = 0;
  long current_time = 0l;
  int phase(long millis);
  long period_milis();
  long period_8th_milis();
  void adjust_tempo(int var);

public:
  Display show(long millis);
  void inc_tempo();
  void dec_tempo();
};
