#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

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

TEST_CASE("Timer") {
  Pomodoro sut = Pomodoro();
  long t = 1l;
  REQUIRE(sut.display().num == 2500);

  sut.poll(t);
  REQUIRE(sut.display().num == 2500);
  sut.start();
  sut.poll(++t);
  SUBCASE("instants later") {
    t += 100;
    sut.poll(t);
    CHECK(sut.display().num == 2459);
  }
  SUBCASE("a second later") {
    t += 1000 + 10;
    sut.poll(t);
    CHECK(sut.display().num == 2458);
    SUBCASE("pause") {
      sut.pause();
      t += 10000;
      sut.poll(t);
      CHECK(sut.display().num == 2458);
      SUBCASE("resume") {
        sut.resume();
        t += 10000;
        sut.poll(t);
        CHECK(sut.display().num == 2448);
      }
      SUBCASE("reset") {
        sut.reset();
        t += 10000;
        sut.poll(t);
        CHECK(sut.display().num == 2500);
      }
    }
  }
  SUBCASE("time reach limit while running") {
    t += 25 * 60 * 1000 + 10;
    sut.poll(t);
    CHECK(sut.status() == Pomodoro::Status::RING);
    CHECK(sut.display().num == 0000);
  }
}

TEST_CASE("Pomodoro button operation") {
  Pomodoro sut = Pomodoro();
  REQUIRE(sut.display().num == 2500);
  REQUIRE(sut.display().dots == 0b0100);
  REQUIRE(sut.status() == Pomodoro::Status::IDLE);

  SUBCASE("Clicking A from idle") {
    sut.buttonA();
    CHECK(sut.status() == Pomodoro::Status::RUNNING);

    SUBCASE("Pausing Pomodoro") {
      sut.buttonA();
      CHECK(sut.status() == Pomodoro::Status::PAUSED);
      SUBCASE("Reseting paused Pomodoro") {
        sut.buttonB();
        CHECK(sut.status() == Pomodoro::Status::IDLE);
      }
      SUBCASE("Resuming Pomodoro") {
        sut.buttonA();
        CHECK(sut.status() == Pomodoro::Status::RUNNING);
      }
    }
  }

  SUBCASE("entering setup") {
    sut.buttonB();
    CHECK(sut.status() == Pomodoro::Status::SET_D);
    SUBCASE("continuing setup") {
      sut.buttonB();
      CHECK(sut.status() == Pomodoro::Status::SET_U);
      SUBCASE("ending setup") {
        sut.buttonB();
        CHECK(sut.status() == Pomodoro::Status::IDLE);
      }
    }
  }
}

TEST_CASE("Setup time") {
  Pomodoro sut = Pomodoro();
  REQUIRE(sut.display().num == 2500);
  SUBCASE("setup minute x10") {
    sut.setup_dec_mins();
    CHECK(sut.display().num == 100);
    SUBCASE("increment first +10 minute (9 instead)") {
      sut.buttonA();
      CHECK(sut.display().num == 1000);
      SUBCASE("increment other +10 minute") {
        sut.buttonA();
        CHECK(sut.display().num == 2000);
      }
      SUBCASE("increment +1 minute x10") {
        sut.setup_unit_mins();
        for (int i = 10; i; i--)
          sut.buttonA();
        CHECK(sut.display().num == 1000);
      }
    }
    SUBCASE("setup minute units") {
      sut.setup_unit_mins();
      CHECK(sut.display().num == 100);
      SUBCASE("increment +1 minute") {
        sut.buttonA();
        CHECK(sut.display().num == 200);
      }
      SUBCASE("increment +1 minute x9") {
        for (int i = 9; i; i--)
          sut.buttonA();
        CHECK(sut.display().num == 100);
      }
    }
  }
}

TEST_CASE("Dots") {
  Pomodoro sut = Pomodoro();
  REQUIRE(sut.display().num == 2500);
  REQUIRE(sut.display().dots == 0b0100);
  SUBCASE("setup dec should be x x.x.x.") {
    sut.setup_dec_mins();
    CHECK(sut.display().dots == 0b0111);
    SUBCASE("setup unit should be x.x x.x.") {
      sut.setup_unit_mins();
      CHECK(sut.display().dots == 0b1011);
    }
  }
  SUBCASE("ring should have all dots") {
    sut.ring();
    CHECK(sut.display().dots == 0b1111);
  }
  SUBCASE("start should have x x.x x.") {
    sut.start();
    CHECK(sut.display().dots == 0b0101);
  }
  SUBCASE("pause should have x x.x.x") {
    sut.pause();
    CHECK(sut.display().dots == 0b0110);
  }
}
