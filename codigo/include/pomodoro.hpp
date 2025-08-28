#pragma once

class Pomodoro {
public:
  struct PomodoroDisplay {
    int num;
    char dots;
  };
  enum Status { IDLE, RUNNING, PAUSED, SET_D, SET_U, RING };

private:
  long time_duration = 25 * 60 * 1000l, time_counting_to = 0, current_time = 0;
  long time_left();
  Status activity_status;

  int time_left_to_display(long t);
  void extend_time_counting_to(long diff) { time_counting_to += diff; }

public:
  Pomodoro();
  void buttonA();
  void buttonB();
  void poll(long milis);
  void start() {
    activity_status = RUNNING;
    time_counting_to = current_time + time_duration;
  }
  void pause() { activity_status = PAUSED; }
  void resume() { activity_status = RUNNING; }
  void setup_dec_mins() {
    activity_status = SET_D;
    time_duration = 60 * 1000l;
  }
  void setup_inc_dec_mins();
  void setup_unit_mins() { activity_status = SET_U; }
  void setup_inc_unit_mins();
  void exit_setup() { activity_status = IDLE; }
  void reset() {
    activity_status = IDLE;
    time_counting_to = 0;
  }
  void ring() { activity_status = RING; }
  PomodoroDisplay display();
  Status status() { return activity_status; }
};
