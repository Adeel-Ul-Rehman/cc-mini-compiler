int main() {
    int arr[5];
    int i;
    
    i = 0;
    while (i < 5) {
        arr[i] = i * 10;
        i = i + 1;
    }
    
    i = 0;
    while (i < 5) {
        output(arr[i]);
        i = i + 1;
    }
    
    return 0;
}