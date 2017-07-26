# Stops script upon any error
set -e

# Executes "make" to build the project
cd Build
make

# Runs the executable
cd bin
./SpaceGameExe
