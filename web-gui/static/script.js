const DEFAULT_CODE = `int main() {
    int a;
    a = 10;
    output(a);
    return 0;
}`;

const codeEditor = document.getElementById('codeEditor');
const compileBtn = document.getElementById('compileBtn');
const clearBtn = document.getElementById('clearBtn');
const resetBtn = document.getElementById('resetBtn');
const outputArea = document.getElementById('outputArea');
const errorsArea = document.getElementById('errorsArea');
const outputTab = document.getElementById('outputTab');
const errorsTab = document.getElementById('errorsTab');
const loading = document.getElementById('loading');

codeEditor.value = DEFAULT_CODE;

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

clearBtn.addEventListener('click', () => {
    codeEditor.value = '';
    outputArea.textContent = '';
    errorsArea.textContent = '';
    outputTab.click();
});

resetBtn.addEventListener('click', () => {
    codeEditor.value = DEFAULT_CODE;
    outputArea.textContent = '';
    errorsArea.textContent = '';
    outputTab.click();
});

compileBtn.addEventListener('click', async () => {
    const code = codeEditor.value;
    
    if (!code.trim()) {
        errorsArea.textContent = 'Error: No code to compile';
        errorsTab.click();
        return;
    }
    
    loading.style.display = 'flex';
    compileBtn.disabled = true;
    outputArea.textContent = '';
    errorsArea.textContent = '';
    
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
            outputTab.click();
        } else {
            outputArea.textContent = result.output || '';
            errorsArea.textContent = result.errors || 'Compilation failed with unknown error.';
            errorsTab.click();
        }
        
    } catch (error) {
        errorsArea.textContent = `Error: ${error.message}`;
        errorsTab.click();
    } finally {
        loading.style.display = 'none';
        compileBtn.disabled = false;
    }
});

codeEditor.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'Enter') {
        e.preventDefault();
        compileBtn.click();
    }
});