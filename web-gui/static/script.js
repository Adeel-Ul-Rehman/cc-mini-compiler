const DEFAULT_CODE = `int main() {
    int a;
    a = 10;
    output(a);
    return 0;
}`;

const codeEditor = document.getElementById('codeEditor');
const compileBtn = document.getElementById('compileBtn');
const aiFixBtn = document.getElementById('aiFixBtn');
const clearBtn = document.getElementById('clearBtn');
const resetBtn = document.getElementById('resetBtn');

// Output areas
const outputArea = document.getElementById('outputArea');
const tacArea = document.getElementById('tacArea');
const astArea = document.getElementById('astArea');
const symArea = document.getElementById('symArea');
const errorsArea = document.getElementById('errorsArea');

// Tab buttons
const outputTab = document.getElementById('outputTab');
const tacTab = document.getElementById('tacTab');
const astTab = document.getElementById('astTab');
const symTab = document.getElementById('symTab');
const errorsTab = document.getElementById('errorsTab');

const loading = document.getElementById('loading');
const aiLoading = document.getElementById('aiLoading');

// Store last error for AI fix
let lastError = '';

// Set default code
codeEditor.value = DEFAULT_CODE;

function hideAllAreas() {
    outputArea.style.display = 'none';
    tacArea.style.display = 'none';
    astArea.style.display = 'none';
    symArea.style.display = 'none';
    errorsArea.style.display = 'none';
}

function removeActiveClass() {
    outputTab.classList.remove('active');
    tacTab.classList.remove('active');
    astTab.classList.remove('active');
    symTab.classList.remove('active');
    errorsTab.classList.remove('active');
}

// Tab click handlers
outputTab.addEventListener('click', () => {
    hideAllAreas();
    removeActiveClass();
    outputArea.style.display = 'block';
    outputTab.classList.add('active');
});

tacTab.addEventListener('click', () => {
    hideAllAreas();
    removeActiveClass();
    tacArea.style.display = 'block';
    tacTab.classList.add('active');
});

astTab.addEventListener('click', () => {
    hideAllAreas();
    removeActiveClass();
    astArea.style.display = 'block';
    astTab.classList.add('active');
});

symTab.addEventListener('click', () => {
    hideAllAreas();
    removeActiveClass();
    symArea.style.display = 'block';
    symTab.classList.add('active');
});

errorsTab.addEventListener('click', () => {
    hideAllAreas();
    removeActiveClass();
    errorsArea.style.display = 'block';
    errorsTab.classList.add('active');
});

clearBtn.addEventListener('click', () => {
    codeEditor.value = '';
    outputArea.textContent = '';
    tacArea.textContent = '';
    astArea.textContent = '';
    symArea.textContent = '';
    errorsArea.textContent = '';
    lastError = '';
    outputTab.click();
});

resetBtn.addEventListener('click', () => {
    codeEditor.value = DEFAULT_CODE;
    outputArea.textContent = '';
    tacArea.textContent = '';
    astArea.textContent = '';
    symArea.textContent = '';
    errorsArea.textContent = '';
    lastError = '';
    outputTab.click();
});

// AI Fix button handler
aiFixBtn.addEventListener('click', async () => {
    const code = codeEditor.value;
    
    if (!code.trim()) {
        errorsArea.textContent = 'Error: No code to fix';
        errorsTab.click();
        return;
    }
    
    if (!lastError) {
        errorsArea.textContent = 'Please compile the code first to detect errors.';
        errorsTab.click();
        return;
    }
    
    aiLoading.style.display = 'flex';
    aiFixBtn.disabled = true;
    
    try {
        const response = await fetch('/ai-fix', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ 
                code: code,
                error: lastError
            })
        });
        
        const result = await response.json();
        
        if (result.success) {
            codeEditor.value = result.fixed_code;
            errorsArea.textContent = 'AI fixed the code! Click "Compile & Run" to test.';
            errorsTab.click();
        } else {
            errorsArea.textContent = `AI Error: ${result.error}`;
            errorsTab.click();
        }
        
    } catch (error) {
        errorsArea.textContent = `Error: ${error.message}`;
        errorsTab.click();
    } finally {
        aiLoading.style.display = 'none';
        aiFixBtn.disabled = false;
    }
});

// Compile and Run
compileBtn.addEventListener('click', async () => {
    const code = codeEditor.value;
    
    if (!code.trim()) {
        errorsArea.textContent = 'Error: No code to compile';
        errorsTab.click();
        return;
    }
    
    loading.style.display = 'flex';
    compileBtn.disabled = true;
    aiFixBtn.disabled = true;
    
    outputArea.textContent = '';
    tacArea.textContent = '';
    astArea.textContent = '';
    symArea.textContent = '';
    errorsArea.textContent = '';
    lastError = '';
    
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
            tacArea.textContent = result.tac || 'No TAC generated.';
            astArea.textContent = result.ast || 'No AST generated.';
            symArea.textContent = result.symbol_table || 'No symbol table generated.';
            errorsArea.textContent = result.errors || 'No errors.';
            outputTab.click();
        } else {
            outputArea.textContent = result.output || 'Compilation failed.';
            tacArea.textContent = result.tac || '';
            astArea.textContent = result.ast || '';
            symArea.textContent = result.symbol_table || '';
            errorsArea.textContent = result.errors || 'Unknown error occurred.';
            lastError = result.errors;
            errorsTab.click();
        }
        
    } catch (error) {
        errorsArea.textContent = `Error: ${error.message}`;
        errorsTab.click();
    } finally {
        loading.style.display = 'none';
        compileBtn.disabled = false;
        aiFixBtn.disabled = false;
    }
});

codeEditor.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'Enter') {
        e.preventDefault();
        compileBtn.click();
    }
});

console.log('C Mini Compiler Web GUI with AI loaded');