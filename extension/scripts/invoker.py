import ctypes
import os
import time

# Proper function_argument with expected data
FUNCTION_ARGUMENT = "simpleFeedEmbed|{\"title\": \"Invoker Test\", \"description\": \"Hello Discord!\"}".encode('utf-8')
DLL_DIR = r"<DLL DIRECTORY>"
DLL_FILE = "arma3-discord-feed_x64.dll"

# Load the DLL
dll = ctypes.CDLL(os.path.join(DLL_DIR, DLL_FILE))

# Define the callback function in Python to match the expected signature
def python_callback(name, function, data):
    try:
        name_str = ctypes.cast(name, ctypes.c_char_p).value.decode('utf-8')
        function_str = ctypes.cast(function, ctypes.c_char_p).value.decode('utf-8')
        data_str = ctypes.cast(data, ctypes.c_char_p).value.decode('utf-8')
        print(f"Callback called with name: {name_str}, function: {function_str}, data: {data_str}")
    except UnicodeDecodeError as e:
        print("Decode error:", e)
        return 0  # Indicate an error in decoding
    return 1  # Indicate success

# Convert the Python function to a C function pointer
CallbackType = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p)
callback_func = CallbackType(python_callback)

# Register the callback
print("[+INVOKE+] Registering callback")
dll.RVExtensionRegisterCallback.argtypes = [CallbackType]
dll.RVExtensionRegisterCallback(callback_func)

# Set up RVExtension function argument types
dll.RVExtension.argtypes = [ctypes.c_char_p, ctypes.c_uint, ctypes.c_char_p]

# Preallocate buffer
buffer_size = 1024
output = ctypes.create_string_buffer(buffer_size)

# Call the function
print("[+INVOKE+] Calling RVExtension with argument:", FUNCTION_ARGUMENT)
dll.RVExtension(output, ctypes.c_uint(buffer_size), ctypes.c_char_p(FUNCTION_ARGUMENT))

# Print the output - jenky way to do this but it works lol
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    exit(0)