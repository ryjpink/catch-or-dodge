# Catch or Dodge

## Building

### Game Client

The game client is a C++ executable built with CMake.

First, install the pre-requisite build tools. For example, on Mac:

`brew install cmake ninja`

Then, set up the vcpkg package manager to install a few third-party libraries:
- Box2D for physics simulation
- SFML for windowing, graphics, input, and sound
- cpr for HTTP requests
- nlohmann_json for JSON conversion
- fmt for string formatting

To configure vcpkg so that CMake will find it, clone the vcpkg repository to a path of your choice and set the VCPKG_HOME environment variable to point to that path. Here, we use the user's home directory:

```bash
$ export VCPKG_HOME=$HOME/vcpkg
$ git clone https://github.com/microsoft/vcpkg $VCPKG_HOME
```

Then, use vcpkg to install the third-party libraries mentioned above:

```bash
$ cd $VCPKG_HOME
$ ./bootstrap_vcpkg.sh
$ ./vcpkg install box2d sfml cpr nlohmann_json fmt
```

Finally, you are ready to build and run the game:

```bash
$ cd /path/to/catch-or-dodge
$ cmake -S . -B out/build/default-release --preset default-release
$ cmake --build out/build/default-release
$ ./out/build/default-release/catcher
```

### Leaderboard Service

The leaderboard service can be run as a normal flask app.

Create a new PostgreSQL database and edit the `SQLALCHEMY_DATABASE_URI` value in `service/__init__.py` to your connection string.

Then populate the table schema and start the service:

```bash
$ python3 -m venv venv
$ ./venv/bin/activate
(venv) $ pip restore -r requirements.txt
(venv) $ cd service
(venv) $ flask db init
(venv) $ flask run
```