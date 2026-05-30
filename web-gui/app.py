from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import subprocess
import os
import tempfile
import re

app = Flask(__name__)
CORS(app)

# Path to compiler executable
COMPILER_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'compiler.exe')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/compile', methods=['POST'])
def compile_code():
    data = request.get_json()
    code = data.get('code', '')
    
    if not code:
        return jsonify({'error': 'No code provided'}), 400
    
    # Create temporary file
    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        f.write(code)
        temp_file = f.name
    
    try:
        # Run compiler
        result = subprocess.run(
            [COMPILER_PATH, temp_file],
            capture_output=True,
            text=True,
            timeout=10
        )
        
        # Parse output
        output = result.stdout
        errors = result.stderr
        
        # Clean up
        os.unlink(temp_file)
        
        return jsonify({
            'success': result.returncode == 0,
            'output': output,
            'errors': errors
        })
        
    except subprocess.TimeoutExpired:
        os.unlink(temp_file)
        return jsonify({'error': 'Compilation timeout (10s)'}), 500
    except Exception as e:
        os.unlink(temp_file)
        return jsonify({'error': str(e)}), 500

@app.route('/run', methods=['POST'])
def run_code():
    """Compile and run in one step"""
    return compile_code()

if __name__ == '__main__':
    app.run(debug=True, port=5000)