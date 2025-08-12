# IndustrialBoar

## Setup

### 1. Install Dependencies
This step will download the dependency libraries
```
sudo apt-get clang-format install build-essential pkg-config libsdl2-dev libsdl2-image-dev
```

### 2. Run init
This step will clone vcpkg, download and build dependencies from vpkg
```
./init.sh
```

## Build
Build as Debug 
```
./build.sh Debug
```

## Run
Run as Debug 
```
./run.sh Debug
```

### Utilities
#### Format code
```
./format.sh
```
