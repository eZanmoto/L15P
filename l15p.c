void repl() {
    print(eval(read(stdin)));
    repl();
}

int main() {
    repl();
    return 0;
}
