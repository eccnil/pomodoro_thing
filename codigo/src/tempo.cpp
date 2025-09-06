#include <tempo.hpp>

int Tempo::inc = 2;
int Tempo::min_tempo = 40;
int Tempo::max_tempo = 180;
int Tempo::default_tempo = 60;

Tempo::Display Tempo::show(long t) {
  current_time = t;
  Display result{.bpm = bpm, .phase = phase(t)};
  return result;
}

void Tempo::inc_tempo() {
  if (!(bpm + inc > max_tempo))
    adjust_tempo(inc);
}
void Tempo::dec_tempo() {
  if (!(bpm - inc < min_tempo))
    adjust_tempo(-inc);
}
void Tempo::adjust_tempo(int delta) {
  auto current_phase = phase(current_time);
  bpm += delta;
  auto new_phase = phase(current_time);
  offset += (current_phase - new_phase);
}

int Tempo::phase(long t) {
  auto total_periods = t / period_8th_milis();
  auto adjusted_periods = total_periods + offset;
  return adjusted_periods % 8;
}

long Tempo::period_milis() { return (60 * 1000l) / bpm; }
long Tempo::period_8th_milis() { return period_milis() / 8; }
