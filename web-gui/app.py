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

# Dynamic path detection for Linux vs Windows
if os.name == 'nt':  # Windows
    PROJECT_ROOT = r"C:\Users\ajade\Desktop\CC\project"
    SRC_PATH = os.path.join(PROJECT_ROOT, "src")
    COMPILER_PATH = os.path.join(SRC_PATH, "compiler.exe")
else:  # Linux (Render)
    # On Render, we're in /opt/render/project/src/web-gui
    # Need to go up two levels to find src
    BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    SRC_PATH = os.path.join(BASE_DIR, "src")
    COMPILER_PATH = os.path.join(SRC_PATH, "compiler")

print(f"OS: {os.name}")
print(f"Compiler path: {COMPILER_PATH}")
print(f"Compiler exists: {os.path.exists(COMPILER_PATH)}")

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/compile', methods=['POST'])
def compile_code():
    data = request.get_json()
    code = data.get('code', '')
    
    if not code:
        return jsonify({'error': 'No code provided'}), 400
    
    if not os.path.exists(COMPILER_PATH):
        return jsonify({
            'success': False,
            'output': '',
            'errors': f'Compiler not found at: {COMPILER_PATH}\nBuild may still be in progress.'
        }), 500
    
    # Create temp file
    temp_file = os.path.join(SRC_PATH, "temp_upload.c")
    with open(temp_file, 'w', encoding='utf-8') as f:
        f.write(code)
    
    try:
        result = subprocess.run(
            [COMPILER_PATH, temp_file],
            capture_output=True,
            text=True,
            timeout=10,
            cwd=SRC_PATH
        )
        
        stdout = result.stdout
        stderr = result.stderr
        
        # Clean up
        if os.path.exists(temp_file):
            os.unlink(temp_file)
        
        # Parse output
        if "Parsing successful" in stdout:
            program_output = ""
            lines = stdout.split('\n')
            in_output = False
            for line in lines:
                if '=== Program Output ===' in line:
                    in_output = True
                    continue
                if in_output and ('===' in line or '========================' in line):
                    break
                if in_output and line.strip():
                    program_output += line + '\n'
            
            program_output = program_output.strip()
            
            return jsonify({
                'success': True,
                'output': program_output if program_output else "Compilation successful. No output.",
                'errors': "No errors."
            })
        else:
            return jsonify({
                'success': False,
                'output': "",
                'errors': stdout if stdout else stderr
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
    app.run(debug=True, port=5000, host='0.0.0.0')