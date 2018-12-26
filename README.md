# libredeo

Trying to make an intervall timer in C++ is was than I imagined. Taking different use cases into account; many timers firing simultaneous, timers created within another timer event, canceling timers, and so on and so forth. 

After many attempts and reworks (just look at the git history), I'm fairly happy with this implementation, it still depends on multiple threads and will most probably segfault if it's pushed it to hard.

## Compiling

Made on macOS, should compile on Linux as well. I'm not interested in Windows so I have no idea if it compiles or not (I guess it will not).

1. Clone the repo
2. run ```make``` to build the test/demo app or, use ```make lib``` to build the lib 
3. ```make lib-install``` installs the lib into ```/usr/local/lib```

## Using

### Intervall timer

To create a repeating timer we can do like so:

```C++
#include <iostream>
#include <intervall.h>

using namespace std::chrono_literals;

void main(in argc, char* argv[]) {
  auto intervall = 5s;
  auto repeats = 7;

  auto id = IntervallTimer::make_intervall(intervall, [](){ std::clog << "Timer event"; }, repeats);
  IntervallTimer::wait();

  return 0;
}
```

The timer will fire 7 times, every five-seconds.

## Once shot timer

A one shot timer can be created with ```IntervallTimer::make_timer(...)```:

```C++
#include <iostream>
#include <intervall.h>

using namespace std::chrono_literals;

void main(in argc, char* argv[]) {
  auto intervall = 5s;

  auto id = IntervallTimer::make_timer(intervall, [](){ std::clog << "Timer event"; });
  IntervallTimer::wait();

  return 0;
}
```

### Cancel timer(s)

A timer can be cancelled by calling ```IntervallTimer::cancel_timer(id)```, we can cancel all timers with a call to ```IntervallTimer::cancel_timer()```.

Note: if the timer callback is currently running, it will not be terminated when cancelling the timer.

## Contributing

Contributions are always welcome!

When contributing to this repository, please first discuss the change you wish to make via the issue tracker, email, or any other method with the owner of this repository before making a change.

Please note that we have a code of conduct, you are required to follow it in all your interactions with the project.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/frklan/libredeo/tags).

## Authors

* **Fredrik Andersson** - *Initial work* - [frklan](https://github.com/frklan)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
