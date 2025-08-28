class Pomodoro {
public:
  struct PomodoroDisplay {
    int num;
    char dots;
  };
  enum Status { IDLE, RUNNING, PAUSED, SET_D, SET_U, RING };

private:
  long time_duration = 25 * 60 * 1000, time_counting_to = 0, current_time = 0;
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
    time_duration = 60 * 1000;
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

Pomodoro::Pomodoro() : activity_status(IDLE) {}

Pomodoro::PomodoroDisplay Pomodoro::display() {
  PomodoroDisplay result;
  switch (activity_status) {

  case IDLE:
    result.num = time_left_to_display(time_duration);
    result.dots = 0b0100;
    break;
  case PAUSED:
    result.num = time_left_to_display(time_left());
    result.dots = 0b0110;
    break;
  case RUNNING:
    result.num = time_left_to_display(time_left());
    result.dots = 0b0101;
    break;
  case RING:
    result.num = 0;
    result.dots = 0b1111;
    break;
  case SET_D:
    result.num = time_left_to_display(time_duration);
    result.dots = 0b0111;
    break;
  case SET_U:
    result.num = time_left_to_display(time_duration);
    result.dots = 0b1011;
    break;
  default:
    result.num = 9999;
    result.dots = activity_status;
  }

  return result;
}

long Pomodoro::time_left() {
  if (time_counting_to && current_time) {
    return time_counting_to - current_time;
  } else {
    return 0;
  }
}

void Pomodoro::setup_inc_dec_mins() {
  bool is_a_minute = time_duration == 1 * 60 * 1000l;
  long incr = (is_a_minute ? 9 : 10) * 60 * 1000l;
  bool is_90_minute = time_duration >= 90 * 60 * 1000l;

  if (is_90_minute) {
    time_duration = 1 * 60 * 1000l;
  } else {
    time_duration += incr;
  }
}

void Pomodoro::setup_inc_unit_mins() {
  int minutes = time_duration / 60000l;
  int minute_units = minutes % 10;
  int incr_minutes;

  if (minute_units == 9) {
    incr_minutes = minutes >= 10 ? -9 : -8; // dont let be 0
  } else {
    incr_minutes = 1;
  }

  time_duration += incr_minutes * 60 * 1000l;
}

int Pomodoro::time_left_to_display(long t) {
  long secsl = t / 1000l;
  int minutes = secsl / 60;
  int secs = secsl % 60;
  return minutes * 100 + secs;
}

void Pomodoro::poll(long milis) {
  // time update
  auto time_diff = milis - current_time;
  current_time = milis;

  // trigger things
  switch (activity_status) {
  case PAUSED:
    extend_time_counting_to(time_diff);
    break;

  case RUNNING:
    bool reached_end_time = milis >= time_counting_to;
    if (reached_end_time)
      ring();
    break;
  }
}

void Pomodoro::buttonA() {
  switch (activity_status) {
  case IDLE:
    start();
    break;
  case RUNNING:
    pause();
    break;
  case PAUSED:
    resume();
    break;
  case SET_D:
    setup_inc_dec_mins();
    break;
  case SET_U:
    setup_inc_unit_mins();
    break;
  }
}

void Pomodoro::buttonB() {
  switch (activity_status) {
  case IDLE:
    setup_dec_mins();
    break;
  case SET_D:
    setup_unit_mins();
    break;
  case SET_U:
    exit_setup();
    break;
  case PAUSED:
    reset();
    break;
  }
}
