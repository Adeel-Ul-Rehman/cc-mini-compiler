import subprocess
import os

COMPILER_PATH = r"C:\Users\ajade\Desktop\CC\project\src\compiler.exe"
SRC_PATH = r"C:\Users\ajade\Desktop\CC\project\src"
TEST_CODE = 'int main() { int a; a=5; output(a); return 0; }'

temp_file = os.path.join(SRC_PATH, "test_direct.c")
with open(temp_file, 'w') as f:
    f.write(TEST_CODE)

print(f"Running: {COMPILER_PATH} {temp_file}")
result = subprocess.run(
    [COMPILER_PATH, temp_file],
    capture_output=True,
    text=True,
    cwd=SRC_PATH
)

print("RETURN CODE:", result.returncode)
print("STDOUT:")
print(result.stdout)
print("STDERR:")
print(result.stderr)