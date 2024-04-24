# Arma 3 Discord Feed

![](https://github.com/a3r0id/arma3-discord-feed/blob/main/logo.png?raw=true)

## Installation
[Windows] To install the required dependencies, move all of the files in `Dependencies/windows-x64` to the root of your Arma 3 server folder.
[Linux] No installation necessary - all dependencies are amalgamated into the extension.

## Setup
- Run your Arma 3 server at least once to generate the necessary files then immediately shutdown the server. This is a crucial step to ensure that the extension is bootstrapped correctly.
- Now, you should see a new folder called "arma3-discord-feed" in the root of your Arma 3 server folder. This is the extension's folder.
- You should see a file inside the "arma3-discord-feed" folder called config.json. Open this file and fill in the required fields.
- Start your Arma 3 server and the extension should be running. You can verify this by checking the log files in the extension's folder.
- If you encounter any issues, please refer to the documentation or reach out to me on Discord: https://isrc.wtf/discord
- Enjoy!

## Build Extension From Source
The extension is entirely cross-platform (64-bit only) and thoroughly tested on Windows 10 (22H2) as well as Ubuntu 22.04.1 LTS.
All dependencies are fetched & built via the CMake workflow, requiring little-to-no setup upon building.

### CMake
<details>
  <summary>See More</summary>
  Simply open the `extension` directory in your favorite IDE with CMake support and build from there OR run the following against the directory via CLI:
  
  ```bash
  mkdir build
  cd build/
  cmake ..
  cmake --build . --config Release
  ```
  
  NOTE: You'll need to make a couple of quick changes to [`CMakeLists.txt`](https://github.com/a3r0id/arma3-discord-feed/blob/main/extension/CMakeLists.txt) in order to automatically install required dependencies to your local Arma 3
  directory as well as build the mod. The only thing you will need to do manually after this is building the PBO, which can be done with [Addon Builder](https://community.bistudio.com/wiki/Addon_Builder) or [Mikero Tools](https://community.bistudio.com/wiki/Mikero_Tools). I plan to add some sort of automation for this process in the future.
  
  Requirements:
  - CMake (>= 3.8)
  - A Python3 installation - the post-build script will attempt move the extension and it's dependencies into the `mod` directory upon completing a successful build.
</details>
