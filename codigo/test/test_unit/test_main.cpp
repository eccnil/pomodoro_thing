#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../src/pomodoro.cpp"
#include "../../src/tempo.cpp"
#include "doctest.h"
#include "pomodoro.hpp"
#include "tempo.hpp"

TEST_SUITE_BEGIN("Pomodoro");

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
    t += 25 * 60 * 1000l + 10;
    sut.poll(t);
    CHECK(sut.status() == Pomodoro::Status::RING);
    CHECK(sut.display().num == 0000);
    SUBCASE("buttonA resetes") {
      sut.buttonA();
      CHECK(sut.status() == Pomodoro::Status::IDLE);
    }
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

TEST_SUITE_END();

TEST_SUITE("Tempo") {
  TEST_CASE("Timing 60") {
    Tempo sut;
    auto d = sut.show(0l);
    REQUIRE(d.bpm == 60);
    REQUIRE(d.phase == 0);
    SUBCASE("still 0 phase") {
      auto d0 = sut.show((1000 / 8) - 1);
      REQUIRE(d0.phase == 0);
    }
    SUBCASE("changes to 1st phase") {
      auto d1 = sut.show((1000 / 8));
      REQUIRE(d1.phase == 1);
    }
    SUBCASE("changes to 7th phase") {
      auto d1 = sut.show((7 * 1000 / 8));
      REQUIRE(d1.phase == 7);
    }
    SUBCASE("changes to 8th phase") {
      auto d1 = sut.show((8 * 1000 / 8));
      REQUIRE(d1.phase == 0);
      SUBCASE("inc during 8th phase maintains phase") {
        sut.dec_tempo();
        auto d = sut.show((8 * 1000 / 8));
        CHECK(d.phase == 0);
        CHECK(d.bpm == 58);
      }
    }
  }

  TEST_CASE("Setting tempo (fixed time)") {
    long time = 7l * 1000l / 8l;
    Tempo sut;
    REQUIRE(sut.show(time).bpm == 60);
    auto phase_at_time = sut.show(time).phase;
    REQUIRE(phase_at_time >= 0);
    REQUIRE(phase_at_time < 8);

    SUBCASE("Lowering to minimum") {
      for (int i = 0; i < 30; i++)
        sut.dec_tempo();
      auto d = sut.show(time);
      CHECK(d.bpm == 40);
      CHECK(d.phase == phase_at_time);
    }
    SUBCASE("Raising to top") {
      for (int i = 0; i < 200; i++)
        sut.inc_tempo();
      auto d = sut.show(time);
      CHECK(d.bpm == 180);
      CHECK(d.phase == phase_at_time);
    }
    SUBCASE("inc tempo") {
      sut.inc_tempo();
      auto d = sut.show(time);
      CHECK(d.bpm == 62);
      CHECK(d.phase == phase_at_time);
    }
    SUBCASE("dec tempo") {
      sut.dec_tempo();
      auto d = sut.show(time);
      CHECK(d.bpm == 58);
      CHECK(d.phase == phase_at_time);
    }
  }
}
