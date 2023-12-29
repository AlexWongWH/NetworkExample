// #include "../lib/asio.hpp"
#include <asio.hpp>
#include <functional>
#include <iostream>
#include <memory>

// More timers

void firstasiofunction() {
  std::cout << "Entered  first function\n";
  // creates asio contex
  asio::io_context io;
  asio::steady_timer t(io, asio::chrono::seconds(5));
  t.wait(); // blocking wait on the timer
  std::cout << " synchronous timer expired\n";
}

// multiple composition mechanisms
//-> pass to an async operation (completion tockens)

// using asio functionilty -> supplying completion token
//(final argument to an asynchronous operation )
void asynchronoustimerprint(const asio::error_code & /*e*/) {
  std::cout << "Timer is done, Print asynchronous\n";
}

void AsynchronousExample() {
  std::cout << "asynchronously timer\n";
  asio::io_context io;
  asio::steady_timer t{io, asio::chrono::seconds{2}};
  t.async_wait(&asynchronoustimerprint); // async wait, passing the function

  // // passing in lamda
  //  t.async_wait([](const asio::error_code &e) {
  //    std::cout << "lam\n";
  //  }); // async wait, passing the function

  // try passing it to lambda to do somethin
  io.run(); // if there is no work to do, `run()` returns immediately
}

// // Tutorial 2

// recursive function which calls itself
void asyncprint(const asio::error_code & /*e*/, asio::steady_timer *t,
                int *count) {
  if (*count < 5) {
    std::cout << "This function has been called " << (*count)++ << " times "
              << std::endl;

    // adds one second from the previous expiry time
    t->expires_at(t->expiry() + asio::chrono::seconds(1));

    // signature should be: void(const asio::error_code&).
    // std::bind(fnc, args1, args2);
    t->async_wait(
        [t, count](const asio::error_code &e) { asyncprint(e, t, count); });
  }
}

// // Tutorial 3 (passing additional arugment to completion handler)

// Timer fires once a second with count
void AsyncExample2() {
  asio::io_context io;
  int count{0};
  asio::steady_timer t{io, asio::chrono::seconds{1}}; // creates 'expiry' timer

  // // using lamda function
  //  t.async_wait(
  //      [&t, &count](const asio::error_code &e) { asyncprint(e, &t, &count);
  //      });

  // trying using std::function
  std::function<void(const asio::error_code &)> fnc =
      std::bind(&asyncprint, std::placeholders::_1, &t, &count);
  t.async_wait(fnc);

  io.run();
}

// Tutorial 4, using member function as completion handler
class printer {
public:
  // need to ensure that the io_context is not const, to which it is able to
  // modify the state
  printer(asio::io_context &io)
      : io_{io}, // holds the reference
        timer_{io, asio::chrono::seconds{1}}
        // initialize the timer using io_context
        // creates a timer which expires in 1 sec
        ,
        count_{0} {
    timer_.async_wait([this](const asio::error_code &e) { this->print(); });
  }

  void print() {
    if (count_ < 5) {
      std::cout << "print() called " << count_++ << " times \n" << std::endl;
      timer_.expires_at(timer_.expiry() + asio::chrono::seconds{1});
      timer_.async_wait(std::bind(&printer::print, this));
      // // using lambda
      // timer_.async_wait([this](const asio::error_code &e) { this->print();
      // });
    }
  }

  ~printer() { std::cout << "Exiting, Final count: " << count_ << "\n"; }

private:
  // can't use shared_ptr, copy constuctor is deleted
  // std::shared_ptr<asio::io_context> ioptr_{};
  int count_;
  asio::io_context &io_; // create reference, not a good practise. Not required
  asio::steady_timer timer_;
};

// using a class to call
void AsyncExample3_Classes() {
  asio::io_context io;
  printer p(io);
  io.run();
}

// running two timers in parallel
class printerMul {
public:
  printerMul(asio::io_context &io)
      : strand_{asio::make_strand(io)}, count_{0},
        timer1_{io, asio::chrono::seconds{1}},
        timer2_{io, asio::chrono::seconds{1}} {
    timer1_.async_wait([this](const asio::error_code &e) { this->print1(); });
    timer2_.async_wait([this](const asio::error_code &e) { this->print2(); });
  }

  void print1() {
    if (count_ < maxcount_) {
      std::cout << "Print 1 calling the function " << count_++ << std::endl;

      // // without mutex
      // timer1_.async_wait([this](const asio::error_code &e) { this->print1();
      // });

      // concept simillar to mutex, locks the critical section prevents other
      // function from running conncurrently
      timer1_.async_wait(asio::bind_executor(
          strand_, [this](const asio::error_code &e) { this->print1(); }));

      // using bind
      // timer1_.async_wait(
      //     asio::bind_executor(strand_, std::bind(&printer::print1, this)));

      timer1_.expires_at(timer1_.expiry() + asio::chrono::seconds{1});
    }
  }

  void print2() {
    if (count_ < maxcount_) {
      std::cout << "Print 2 calling the function " << count_++ << std::endl;
      timer2_.async_wait(asio::bind_executor(
          strand_, [this](const asio::error_code &e) { this->print2(); }));

      timer2_.expires_at(timer2_.expiry() + asio::chrono::seconds{2});
    }
  }

  ~printerMul() { std::cout << "final count is " << count_ << std::endl; }

private:
  // method of synchronisation, similar to mutex?
  // strand_ member, an object of type
  // asio::strand<asio::io_context::executor_type>.
  asio::strand<asio::io_context::executor_type> strand_;
  asio::steady_timer timer1_;
  asio::steady_timer timer2_;
  int count_{0};           // shared data
  const int maxcount_{10}; // I'm lazy
};

void AsyncExample4_Classes() {
  asio::io_context io;
  printerMul p(io);

  // asio::thread(Function f);
  asio::thread t([&io]() { io.run(); });
  t.join();
  io.run();
}

// examples of timers
void basics() {

  // one I/O execution context -> the link between program and os I/O system
  // asio::io_context io; // creates asio context

  // // have to create sockets for I/o
  // asio::ip::tcp::socket sock{asio::io_context};
  // sock.connect();

  // //synchronous function
  // firstasiofunction();

  // //Asynchronous function, timer call function once
  // AsynchronousExample();

  // // using lamdas, repeated timer calls using lamdas
  // AsyncExample2();

  // // testing using a class
  // AsyncExample3_Classes();

  // Multi thread
  AsyncExample4_Classes();
}
