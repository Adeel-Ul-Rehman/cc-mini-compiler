from http.server import HTTPServer, BaseHTTPRequestHandler
import subprocess
import json
import os
import urllib.parse

SRC_PATH = r"C:\Users\ajade\Desktop\CC\project\src"
COMPILER_PATH = os.path.join(SRC_PATH, "compiler.exe")

HTML = '''<!DOCTYPE html>
<html>
<head><title>C Mini Compiler</title>
<style>
body { font-family: monospace; background: #1a1a2e; color: #eee; margin: 0; padding: 20px; }
.container { max-width: 1200px; margin: 0 auto; }
textarea { width: 100%; height: 400px; background: #0a0a0f; color: #00ff00; border: 1px solid #333; padding: 10px; font-family: monospace; font-size: 14px; }
button { background: #00d4ff; color: #000; padding: 10px 20px; border: none; cursor: pointer; margin: 10px 0; }
pre { background: #0a0a0f; padding: 20px; overflow: auto; }
</style>
</head>
<body>
<div class="container">
<h1>⚡ C Mini Compiler</h1>
<textarea id="code">int main() {
    int a;
    a = 10;
    output(a);
    return 0;
}</textarea><br>
<button onclick="compile()">Compile & Run</button>
<button onclick="clearOutput()">Clear</button>
<h3>Output:</h3>
<pre id="output"></pre>
</div>
<script>
async function compile() {
    const code = document.getElementById('code').value;
    const response = await fetch('/compile', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({code: code})
    });
    const result = await response.json();
    document.getElementById('output').textContent = result.output || result.error;
}
function clearOutput() {
    document.getElementById('output').textContent = '';
}
</script>
</body>
</html>
'''

class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write(HTML.encode())
        else:
            self.send_response(404)
            self.end_headers()
    
    def do_POST(self):
        if self.path == '/compile':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            data = json.loads(post_data.decode())
            code = data.get('code', '')
            
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
                
                # Extract program output
                program_output = ""
                lines = stdout.split('\n')
                in_output = False
                for line in lines:
                    if '=== Program Output ===' in line:
                        in_output = True
                        continue
                    if in_output and ('===' in line or '====' in line):
                        break
                    if in_output and line.strip():
                        program_output += line + '\n'
                
                program_output = program_output.strip()
                
                if os.path.exists(temp_file):
                    os.unlink(temp_file)
                
                self.send_response(200)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                
                if program_output:
                    self.wfile.write(json.dumps({'output': program_output}).encode())
                else:
                    self.wfile.write(json.dumps({'output': stdout if stdout else 'Compilation successful. No output.'}).encode())
                
            except Exception as e:
                if os.path.exists(temp_file):
                    os.unlink(temp_file)
                self.send_response(200)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                self.wfile.write(json.dumps({'error': str(e)}).encode())
        else:
            self.send_response(404)
            self.end_headers()

if __name__ == '__main__':
    print(f"Server starting on http://localhost:8080")
    print(f"Using compiler: {COMPILER_PATH}")
    server = HTTPServer(('localhost', 8080), Handler)
    server.serve_forever()