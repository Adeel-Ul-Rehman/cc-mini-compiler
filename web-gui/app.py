from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import subprocess
import os
import tempfile
import re
import sys

app = Flask(__name__, 
            template_folder=os.path.dirname(os.path.abspath(__file__)) + '/templates',
            static_folder=os.path.dirname(os.path.abspath(__file__)) + '/static')
CORS(app)

# Use raw strings for Windows paths
SRC_PATH = r"C:\Users\ajade\Desktop\CC\project\src"
COMPILER_PATH = os.path.join(SRC_PATH, "compiler.exe")

print(f"Using compiler: {COMPILER_PATH}")
print(f"Compiler exists: {os.path.exists(COMPILER_PATH)}")

# Set environment for subprocess
ENV = os.environ.copy()
ENV['PATH'] = r"C:\msys64\ucrt64\bin;" + ENV.get('PATH', '')
ENV['CHERE_INVOKING'] = '1'

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/compile', methods=['POST'])
def compile_code():
    data = request.get_json()
    code = data.get('code', '')
    
    print("=" * 50)
    print(f"CODE RECEIVED:\n{code}")
    print("=" * 50)
    
    if not code:
        return jsonify({'error': 'No code provided'}), 400
    
    # Create temp file in src directory (same as test_compiler.py)
    temp_file = os.path.join(SRC_PATH, "temp_upload.c")
    with open(temp_file, 'w', encoding='utf-8') as f:
        f.write(code)
    
    try:
        # Run compiler - EXACT same as test_compiler.py
        result = subprocess.run(
            [COMPILER_PATH, temp_file],
            capture_output=True,
            text=True,
            timeout=10,
            cwd=SRC_PATH,
            env=ENV
        )
        
        stdout = result.stdout
        stderr = result.stderr
        
        print("=" * 50)
        print("COMPILER OUTPUT:")
        print(stdout)
        print("STDERR:", stderr)
        print("RETURN CODE:", result.returncode)
        print("=" * 50)
        
        # Clean up
        if os.path.exists(temp_file):
            os.unlink(temp_file)
        
        # Check if compilation was successful
        is_success = result.returncode == 0 or "Parsing successful" in stdout
        
        if is_success:
            # Extract program output
            program_output = ""
            lines = stdout.split('\n')
            in_output = False
            for line in lines:
                if '=== Program Output ===' in line:
                    in_output = True
                    continue
                if in_output and ('===' in line or '=========================' in line):
                    break
                if in_output and line.strip():
                    program_output += line + '\n'
            
            program_output = program_output.strip()
            
            # If no program output found in markers, return full stdout
            if not program_output:
                program_output = stdout.strip() if stdout.strip() else "Compilation successful. No output."
            
            return jsonify({
                'success': True,
                'output': program_output,
                'errors': ""
            })
        else:
            # Compilation failed
            return jsonify({
                'success': False,
                'output': "",
                'errors': stdout if stdout else (stderr if stderr else "Unknown compilation error")
            })
        
    except subprocess.TimeoutExpired:
        if os.path.exists(temp_file):
            os.unlink(temp_file)
        return jsonify({'error': 'Compilation timeout (10s)'}), 500
    except Exception as e:
        if os.path.exists(temp_file):
            os.unlink(temp_file)
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    print("=" * 50)
    print("Starting C Mini Compiler Web GUI")
    print(f"Compiler: {COMPILER_PATH}")
    print("=" * 50)
    app.run(debug=True, port=5000, host='127.0.0.1')