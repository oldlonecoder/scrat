//
// Created by oldlonecoder on 1/11/23.
//



#pragma once


#include <scrat/result>

// ------- copied from, intellectual proprietary to:
// https://schneegans.github.io/tutorials/2015/09/20/signal-slot :
#include <functional>
#include <map>

namespace scrat
{

// A signal object may call multiple slots with the
// same signature. You can connect functions to the signal
// which will be called when the emit() method on the
// signal object is invoked. Any argument passed to emit()
// will be passed to the given functions.
/*
 class Person {
public:
  Person(std::string const &name) : name_(name) {}

  Signal<std::string const&> say;

  result<> listen(std::string const& message) {
    std::cout << name_ << " received: " << message << std::endl;
    return rem::accepted;
  }

private:
  std::string name_;
};

int main() {
  Person alice("Alice"), bob("Bob");

  alice.say.connect_member(&bob, &Person::listen);
  bob.say.connect_member(&alice, &Person::run);

  //alice.say.emit("Have a nice day!");
  auto R = alice.say("Have a nice day!");
  if(R && *R == rem::accepted)
      (void)bob.say("Thank you!");
  else
      (void)bob.say("Sorry!");
  return 0;
}
 */

template <typename... Args> class signal {

public:
    signal()  = default;
    ~signal() = default;

    // Copy constructor and assignment create a new signal.
    signal(signal const& /*unused*/) {}

    signal& operator=(signal const& other) {
        if (this != &other) {
            disconnect_all();
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    signal(signal&& other) noexcept:
        _slots(std::move(other._slots)),
        _current_id(other._current_id) {}

    signal& operator=(signal&& other) noexcept {
        if (this != &other) {
            _slots     = std::move(other._slots);
            _current_id = other._current_id;
        }

        return *this;
    }


    // Connects a std::function to the signal. The returned
    // value can be used to disconnect the function again.
    int connect(std::function<result<>(Args...)> const& slot) const {
        _slots.insert(std::make_pair(++_current_id, slot));
        return _current_id;
    }

    // Convenience method to connect a member function of an
    // object to this signal.
    template <typename T>
    int connect_member(T *inst, result<> (T::*func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
        });
    }

    // Convenience method to connect a member function of an
    // object to this signal.
    template <typename T>
    int connect(T *inst, result<> (T::*func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
        });
    }


    // Convenience method to connect a const member function
    // of an object to this signal.
    template <typename T>
    int connect(T *inst, result<> (T::*func)(Args...) const) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
        });
    }

    // Disconnects a previously connected function.
    void disconnect(int id) const {
        _slots.erase(id);
    }

    // Disconnects all previously connected functions.
    void disconnect_all() const {
        _slots.clear();
    }

    // Calls all connected functions.
    result<> emit(Args... p) {
        result<> R;
        for (auto const& [f,fn] : _slots) {
            R =  fn(p...);
            if(!R || *R == rem::rejected) return R;
        }
        return R;
    }
    // Calls all connected functions.
    result<> operator()(Args... p) {
        result<> R;
        for (auto const& [f,fn] : _slots) {
            R = fn(p...);
            if(!R || *R == rem::rejected) return R;
        }
        return R;
    }


    // Calls all connected functions except for one.
    result<> emit_for_all_but_one(int excludedConnectionID, Args... p) {
        result <> R;
        for (auto const& it : _slots) {
            if (it.first != excludedConnectionID) {
                R = it.second(p...);
                if(!R || *R == rem::rejected) return R;
            }
        }
        return R;
    }

    // Calls only one connected function.
    result<> emit_for(int connectionID, Args... p) {
        result<> R;
        auto const& it = _slots.find(connectionID);
        if (it != _slots.end()) {
            R = it->second(p...);
            if(!R || *R == rem::rejected) return R;
        }
        return R;
    }

private:
    mutable std::map<int, std::function<result<>(Args...)>> _slots;
    mutable int _current_id{0};
};

}