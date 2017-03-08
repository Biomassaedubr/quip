#include "catch.hpp"

#include "Signal.hpp"

using namespace quip;

TEST_CASE("Signals can connect and trigger a single listener.", "[SignalTests]") {
  Signal<void ()> signal;
  bool success = false;
  signal.connect([&] { success = true; });
  signal.transmit();
  
  REQUIRE(success);
}

TEST_CASE("Signals can connect and trigger multiple listeners.", "[SignalTests]") {
  Signal<void ()> signal;
  bool firstSuccess = false;
  bool secondSuccess = false;
  signal.connect([&] { firstSuccess = true; });
  signal.connect([&] { secondSuccess = true; });
  signal.transmit();
  
  REQUIRE(firstSuccess);
  REQUIRE(secondSuccess);
}

TEST_CASE("Signals can disconnect a listener from the front.", "[SignalTests]") {
  Signal<void ()> signal;
  bool firstSuccess = false;
  bool secondSuccess = false;
  bool thirdSuccess = false;
  std::uint32_t token = signal.connect([&] { firstSuccess = true; });
  signal.connect([&] { secondSuccess = true; });
  signal.connect([&] { thirdSuccess = true; });
  signal.disconnect(token);
  
  signal.transmit();
  REQUIRE_FALSE(firstSuccess);
  REQUIRE(secondSuccess);
  REQUIRE(thirdSuccess);
}

TEST_CASE("Signals can disconnect a listener from the middle.", "[SignalTests]") {
  Signal<void ()> signal;
  bool firstSuccess = false;
  bool secondSuccess = false;
  bool thirdSuccess = false;
  signal.connect([&] { firstSuccess = true; });
  std::uint32_t token = signal.connect([&] { secondSuccess = true; });
  signal.connect([&] { thirdSuccess = true; });
  signal.disconnect(token);
  
  signal.transmit();
  REQUIRE(firstSuccess);
  REQUIRE_FALSE(secondSuccess);
  REQUIRE(thirdSuccess);
}

TEST_CASE("Signals can disconnect a listener from the end.", "[SignalTests]") {
  Signal<void ()> signal;
  bool firstSuccess = false;
  bool secondSuccess = false;
  bool thirdSuccess = false;
  signal.connect([&] { firstSuccess = true; });
  signal.connect([&] { secondSuccess = true; });
  std::uint32_t token = signal.connect([&] { thirdSuccess = true; });
  signal.disconnect(token);
  
  signal.transmit();
  REQUIRE(firstSuccess);
  REQUIRE(secondSuccess);
  REQUIRE_FALSE(thirdSuccess);
  
  // Connect another listener to ensure the tail pointer is correctly
  // updated by the disconnection.
  signal.connect([&] { thirdSuccess = true; });
  signal.transmit();
  REQUIRE(thirdSuccess);
}

TEST_CASE("Signals can disconnect the only listener.", "[SignalTests]") {
  Signal<void ()> signal;
  std::uint32_t token = signal.connect([&] { REQUIRE(false); });
  signal.disconnect(token);
  signal.transmit();
  
  // Connect another listener to ensure the tail pointer is correctly
  // updated by the disconnection.
  bool success = false;
  signal.connect([&] { success = true; });
  signal.transmit();
  REQUIRE(success);
}

