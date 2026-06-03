from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import subprocess
import os
import tempfile
import re
import sys

# Try to import Gemini AI
try:
    import google.generativeai as genai
    from dotenv import load_dotenv
    load_dotenv()
    GEMINI_API_KEY = os.getenv('GEMINI_API_KEY')
    if GEMINI_API_KEY:
        genai.configure(api_key=GEMINI_API_KEY)
        
        # Use a model that is confirmed to work from your list
        # models/gemini-2.0-flash is available and fast
        MODEL_NAME = 'models/gemini-2.0-flash'
        
        try:
            model = genai.GenerativeModel(MODEL_NAME)
            # Quick test to verify it works
            test_response = model.generate_content("test")
            AI_AVAILABLE = True
            print(f"✅ Gemini AI enabled with model: {MODEL_NAME}")
        except Exception as e:
            print(f"⚠️ Model {MODEL_NAME} failed: {e}")
            # Fallback to another working model
            try:
                model = genai.GenerativeModel('models/gemini-flash-latest')
                test_response = model.generate_content("test")
                AI_AVAILABLE = True
                print(f"✅ Gemini AI enabled with model: models/gemini-flash-latest")
            except:
                AI_AVAILABLE = False
                print("⚠️ No Gemini model available. AI fix disabled.")
    else:
        AI_AVAILABLE = False
        print("⚠️ Gemini AI not configured. Set GEMINI_API_KEY in .env file")
except ImportError:
    AI_AVAILABLE = False
    print("⚠️ Google Generative AI not installed. Run: pip install google-generativeai python-dotenv")

app = Flask(__name__, 
            template_folder=os.path.dirname(os.path.abspath(__file__)) + '/templates',
            static_folder=os.path.dirname(os.path.abspath(__file__)) + '/static')
CORS(app)

# Paths
if os.name == 'nt':  # Windows
    PROJECT_ROOT = r"C:\Users\ajade\Desktop\CC\project"
    SRC_PATH = os.path.join(PROJECT_ROOT, "src")
    COMPILER_PATH = os.path.join(SRC_PATH, "compiler.exe")
else:
    BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    SRC_PATH = os.path.join(BASE_DIR, "src")
    COMPILER_PATH = os.path.join(SRC_PATH, "compiler")

print(f"OS: {os.name}")
print(f"Compiler path: {COMPILER_PATH}")
print(f"Compiler exists: {os.path.exists(COMPILER_PATH)}")

def extract_section(text, start_marker, end_marker=None):
    """Extract a section between two markers."""
    if not text:
        return ""
    start_idx = text.find(start_marker)
    if start_idx == -1:
        return ""
    start_idx += len(start_marker)
    
    if end_marker:
        end_idx = text.find(end_marker, start_idx)
        if end_idx == -1:
            return text[start_idx:].strip()
        return text[start_idx:end_idx].strip()
    else:
        end_idx = text.find("===", start_idx)
        if end_idx == -1:
            return text[start_idx:].strip()
        return text[start_idx:end_idx].strip()

def create_ai_prompt(code, error_message):
    """Create a prompt for Gemini AI to fix the code."""
    return f"""You are a C-like language compiler assistant. Fix the following code based on the error.

OUR LANGUAGE GRAMMAR RULES:
- Data types: int, float, bool, void
- Arrays: int arr[5]; or int arr[3] = {{10, 20, 30}};
- Array access: arr[index]
- Control structures: if (cond) {{ }} else {{ }}, while (cond) {{ }}, for (init; cond; inc) {{ }}
- Functions: return_type name(parameters) {{ statements }}
- Input: input(var);
- Output: output(expr);
- Statements end with semicolon ;
- Blocks use {{ }}
- Comments: // single line

ERROR MESSAGE:
{error_message}

CODE WITH ERROR:
{code}

TASK:
1. Analyze the error and fix the code according to our grammar rules
2. Return ONLY the corrected code (no explanations, no markdown formatting)
3. Keep the same functionality as intended
4. Make sure all statements end with semicolons
5. Ensure variable declarations come before use

CORRECTED CODE:"""

def ai_fix_code(code, error_message):
    """Send code and error to Gemini AI and get fixed code."""
    if not AI_AVAILABLE:
        return None, "Gemini AI is not configured. Please check API key."
    
    try:
        prompt = create_ai_prompt(code, error_message)
        response = model.generate_content(prompt)
        fixed_code = response.text.strip()
        
        # Remove markdown code blocks if present
        if fixed_code.startswith('```c'):
            fixed_code = fixed_code[5:]
        if fixed_code.startswith('```'):
            fixed_code = fixed_code[3:]
        if fixed_code.endswith('```'):
            fixed_code = fixed_code[:-3]
        fixed_code = fixed_code.strip()
        
        return fixed_code, None
    except Exception as e:
        return None, f"AI Error: {str(e)}"

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
            'tac': '',
            'ast': '',
            'symbol_table': '',
            'errors': f'Compiler not found at: {COMPILER_PATH}\nPlease run "make" in src folder first.'
        }), 500
    
    temp_file = os.path.join(SRC_PATH, "temp_upload.c")
    with open(temp_file, 'w', encoding='utf-8') as f:
        f.write(code)
    
    try:
        if os.name == 'nt':
            result = subprocess.run(
                f'"{COMPILER_PATH}" "{temp_file}"',
                capture_output=True,
                text=True,
                timeout=10,
                cwd=SRC_PATH,
                shell=True,
                encoding='utf-8',
                errors='replace'
            )
        else:
            result = subprocess.run(
                [COMPILER_PATH, temp_file],
                capture_output=True,
                text=True,
                timeout=10,
                cwd=SRC_PATH,
                encoding='utf-8',
                errors='replace'
            )
        
        stdout = result.stdout
        stderr = result.stderr
        
        if os.path.exists(temp_file):
            os.unlink(temp_file)
        
        if not stdout and not stderr:
            return jsonify({
                'success': False,
                'output': '',
                'tac': '',
                'ast': '',
                'symbol_table': '',
                'errors': 'Compiler produced no output.'
            })
        
        program_output = extract_section(stdout, "=== Program Output ===", "========================")
        tac_section = extract_section(stdout, "=== Three Address Code (TAC) ===", "==============================")
        ast_section = extract_section(stdout, "Program", "--- Symbol Table ---")
        if ast_section:
            lines = ast_section.split('\n')
            ast_section = '\n'.join([line for line in lines if line.strip()])
        symbol_table_section = extract_section(stdout, "--- Symbol Table ---", None)
        
        if "Parsing successful" in stdout or "=== Program Output ===" in stdout:
            return jsonify({
                'success': True,
                'output': program_output if program_output else "Compilation successful. No output.",
                'tac': tac_section if tac_section else "No TAC generated.",
                'ast': ast_section if ast_section else "No AST generated.",
                'symbol_table': symbol_table_section if symbol_table_section else "No symbol table generated.",
                'errors': "No errors."
            })
        else:
            error_output = stdout if stdout else stderr
            if not error_output or error_output.strip() == "":
                error_output = "Parsing failed. Check your code syntax."
            
            return jsonify({
                'success': False,
                'output': "",
                'tac': "",
                'ast': "",
                'symbol_table': "",
                'errors': error_output
            })
        
    except subprocess.TimeoutExpired:
        if os.path.exists(temp_file):
            os.unlink(temp_file)
        return jsonify({'error': 'Compilation timeout (10s)'}), 500
    except UnicodeDecodeError as e:
        if os.path.exists(temp_file):
            os.unlink(temp_file)
        return jsonify({
            'success': False,
            'output': '',
            'tac': '',
            'ast': '',
            'symbol_table': '',
            'errors': f'Encoding error: {str(e)}'
        }), 500
    except Exception as e:
        if os.path.exists(temp_file):
            os.unlink(temp_file)
        return jsonify({'error': str(e)}), 500

@app.route('/ai-fix', methods=['POST'])
def ai_fix():
    """AI endpoint to fix code based on compilation errors."""
    data = request.get_json()
    code = data.get('code', '')
    error_message = data.get('error', '')
    
    if not code:
        return jsonify({'success': False, 'error': 'No code provided'}), 400
    
    if not error_message:
        return jsonify({'success': False, 'error': 'No error message provided'}), 400
    
    fixed_code, ai_error = ai_fix_code(code, error_message)
    
    if ai_error:
        return jsonify({'success': False, 'error': ai_error}), 500
    
    return jsonify({'success': True, 'fixed_code': fixed_code})

if __name__ == '__main__':
    print("=" * 50)
    print("C Mini Compiler Web GUI with AI")
    print("=" * 50)
    print(f"Compiler: {COMPILER_PATH}")
    print(f"Compiler exists: {os.path.exists(COMPILER_PATH)}")
    print(f"AI Available: {AI_AVAILABLE}")
    if AI_AVAILABLE:
        print(f"AI Model: {MODEL_NAME}")
    print("=" * 50)
    app.run(debug=True, port=5000, host='127.0.0.1')