#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../src/pomodoro.cpp"
#include "doctest.h"

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
