int cnt = 0;

void set_count(int c){
    cnt = c;
}

int get_count() {
    return cnt;
}

int count(){
    int c = get_count();
    sleep(5);
    set_count(++c);

    return c;
}
