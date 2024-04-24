import sys
import os
import shutil

if len(sys.argv) == 1:
    raise Exception("No arguments provided")

print ("[Build.py] Running build script...")
print ("[Build.py] sys.argv: " + str(sys.argv))

LINUX_A3_PATH   = os.path.normpath(sys.argv[1])
WINDOWS_A3_PATH = os.path.normpath(sys.argv[2])
BIN_PATH        = os.path.normpath(sys.argv[3])
MOD_NAME        = "arma3-discord-feed"
INSTALL_INFO    = f"""
[Installation]
To install the required dependencies, move all of the files in Dependencies/<your operating system> to the root of your Arma 3 server folder.

[Setup]
1. Run your Arma 3 server at least once to generate the necessary files then immediately shutdown the server. This is a crucial step to ensure that the extension is bootstrapped correctly.
1b. Now, you should see a new folder called {MOD_NAME} in the root of your Arma 3 server folder. This is the extension's folder.
2. You should see a file inside the {MOD_NAME} folder called config.json. Open this file and fill in the required fields.
3. Start your Arma 3 server and the extension should be running. You can verify this by checking the log files in the extension's folder.
4. If you encounter any issues, please refer to the documentation or reach out to me on Discord: https://isrc.wtf/discord
5. Enjoy!
"""

# output (mod) path is 4 levels up from the bin path
MOD_EXT_PATH = os.path.join(os.path.abspath(os.path.join(BIN_PATH, "../../../..")), "mod", "out")
if not os.path.exists(MOD_EXT_PATH):
    os.makedirs(MOD_EXT_PATH)

MOD_EXT_DEPS_PATH = os.path.join(MOD_EXT_PATH, "Dependencies", "linux-x64" if 'nt' not in os.name else "windows-x64")
if not os.path.exists(MOD_EXT_DEPS_PATH):
    os.makedirs(MOD_EXT_DEPS_PATH)

print ("[Build.py] BIN_PATH: " + BIN_PATH)
print ("[Build.py] MOD_EXT_PATH: " + MOD_EXT_PATH)
print ("[Build.py] MOD_EXT_DEPS_PATH: " + MOD_EXT_DEPS_PATH)

if os.name == "nt":
    A3_PATH = WINDOWS_A3_PATH
    DEPS = [
        (r"_deps\zlib-build", "zlib.dll"),
        (r"_deps\cpr-build\cpr", "cpr.dll"),
        (r"_deps\curl-build\lib", "libcurl-d.dll"),
        (f"{MOD_NAME}_x64.dll"),
    ]
else:
    A3_PATH = LINUX_A3_PATH
    DEPS = [
        ("_deps/zlib-build", "libz.so"),
        ("_deps/cpr-build/cpr", "libcpr.so"),
        ("_deps/curl-build/lib", "libcurl-d.so"),
        (f"{MOD_NAME}_x64.so"),
    ]

# all dependencies should be copied to mod/out/Dependencies besides the main extension
# the main extension should be copied to mod/out
# also, the dependencies should be copied to the root of the arma 3 directory, besides the main extension
for dep in DEPS:
    dep_path = os.path.join(BIN_PATH, *dep) if len(dep) == 2 else os.path.join(BIN_PATH, dep)
    if not os.path.exists(dep_path):
        print(f"[build.py|Critical Warning] Dependency not found: {dep_path}")
        continue
    if len(dep) == 2:
        shutil.copy(dep_path, A3_PATH)
        print (f"[Build.py] Copied {dep_path} to {A3_PATH}")
        shutil.copy(dep_path, MOD_EXT_DEPS_PATH)
        print (f"[Build.py] Copied {dep_path} to {MOD_EXT_DEPS_PATH}")
    else:
        shutil.copy(dep_path, MOD_EXT_PATH)
        print (f"[Build.py] Copied {dep_path} to {MOD_EXT_PATH}")
