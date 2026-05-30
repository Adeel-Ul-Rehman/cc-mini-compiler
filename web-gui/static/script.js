// Default example code
const DEFAULT_CODE = `int main() {
    int a;
    int b;
    int c;
    
    a = 5;
    b = 10;
    c = a + b;
    
    output(c);
    
    return 0;
}`;

// DOM Elements
const codeEditor = document.getElementById('codeEditor');
const compileBtn = document.getElementById('compileBtn');
const clearBtn = document.getElementById('clearBtn');
const resetBtn = document.getElementById('resetBtn');
const outputArea = document.getElementById('outputArea');
const errorsArea = document.getElementById('errorsArea');
const outputTab = document.getElementById('outputTab');
const errorsTab = document.getElementById('errorsTab');
const loading = document.getElementById('loading');

// Set default code
codeEditor.value = DEFAULT_CODE;

// Tab switching
outputTab.addEventListener('click', () => {
    outputTab.classList.add('active');
    errorsTab.classList.remove('active');
    outputArea.style.display = 'block';
    errorsArea.style.display = 'none';
});

errorsTab.addEventListener('click', () => {
    errorsTab.classList.add('active');
    outputTab.classList.remove('active');
    errorsArea.style.display = 'block';
    outputArea.style.display = 'none';
});

// Clear button
clearBtn.addEventListener('click', () => {
    codeEditor.value = '';
    outputArea.textContent = '';
    errorsArea.textContent = '';
    outputTab.click();
});

// Reset button
resetBtn.addEventListener('click', () => {
    codeEditor.value = DEFAULT_CODE;
    outputArea.textContent = '';
    errorsArea.textContent = '';
    outputTab.click();
});

// Compile and Run
compileBtn.addEventListener('click', async () => {
    const code = codeEditor.value;
    
    if (!code.trim()) {
        errorsArea.textContent = 'Error: No code to compile';
        errorsTab.click();
        return;
    }
    
    // Show loading
    loading.style.display = 'flex';
    compileBtn.disabled = true;
    
    try {
        const response = await fetch('/compile', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ code: code })
        });
        
        const result = await response.json();
        
        if (result.success) {
            outputArea.textContent = result.output || 'Compilation successful. No output.';
            errorsArea.textContent = result.errors || 'No errors.';
        } else {
            outputArea.textContent = result.output || 'Compilation failed.';
            errorsArea.textContent = result.errors || 'Unknown error occurred.';
        }
        
        // Show output tab by default
        outputTab.click();
        
        // If errors exist, highlight errors tab
        if (result.errors && result.errors.trim()) {
            errorsTab.classList.add('active');
            outputTab.classList.remove('active');
            errorsArea.style.display = 'block';
            outputArea.style.display = 'none';
        }
        
    } catch (error) {
        errorsArea.textContent = `Error: ${error.message}`;
        errorsTab.click();
    } finally {
        loading.style.display = 'none';
        compileBtn.disabled = false;
    }
});

// Keyboard shortcut: Ctrl+Enter to compile
codeEditor.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'Enter') {
        e.preventDefault();
        compileBtn.click();
    }
});