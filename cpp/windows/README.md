# Readme

## How to make a new project

Use `cmake` as build tool. Create the new project by following the steps below:

- Create a new directory
    - Lower case naming e.g. `read_kernel_driver`
- Add `.gitignore` and `CMakeLists.txt` inside (you may take sample from another project)
- Create a new subdirectory `build` and cd to it
- Run `cmake ..` to make the build settings
- Run `cmake --build <path_to_build_directory>` from anywhere to build the project
- Run it from `<project_root>/bin/Debug/<project_name>.exe`
