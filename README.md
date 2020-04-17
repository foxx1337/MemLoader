## Installation

### Prerequisites

1. `cmake`
2. `vcpkg`
   1. `googletest`

      ```shell
      vcpkg install gtest
      ```

      `gtest` from `vcpkg` needs to be made available to the project by passing a variable at
      `cmake` generation time:

      ```shell
      cmake -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
      ```

      or, for msvc under Windows, by adding the variable to `CMakeSettings.json` as specified
      in the
      [`vcpkg` docs](https://vcpkg.readthedocs.io/en/latest/examples/installing-and-using-packages/#cmake-toolchain-file):
      
      ```json
      "variables": [
          {
              "name": "CMAKE_TOOLCHAIN_FILE",
              "value": "d:\\src\\C\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"
          },
          {
              "name": "CMAKE_VERBOSE_MAKEFILE",
              "value": "TRUE"
          },
          {
              "name": "BUILD_EXPERIMENTS",
              "value": "TRUE"
          }
      ]
      ```
3. (optional) QT5 libraries, when specifying `-DBUILD_MEMLOADER_GUI=True` to `cmake`.

To include the `experiments` project also pass `-DBUILD_EXPERIMENTS=True` to `cmake`.
