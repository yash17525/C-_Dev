// Use two threads, where 1st thread writes to a particular file and the second thread reads from the same file.
#include <iostream>
#include <pthread.h>
#include <fstream>

using namespace std;

void *thread_function(void* arg)
{
    cout << "This is thread 2, thread id: " << pthread_self() << endl;
    string line;
    ifstream fin;
    fin.open("sample.txt");
    while (getline(fin, line)) {
        cout << "thread (2) reading from file, line:     " << line << endl;
    }
    fin.close();
}

int main()
{
    ofstream fout;
    string lines[] = {
        "this is line number 1: gjeg",
        "this is line number 2: gdg",
        "this is line number 3: dfl",
        "this is line number 4: fef",
        "this is line number 5: efljwf",
        "this is line number 6: fefk",
        "this is line number 7: dfs",
        "this is line number 8: dfesfw",
        "this is line number 9: fwefw",
        "this is line number 10: ljlj"};

    pthread_t my_thread;
    int ret = pthread_create(&my_thread, NULL, &thread_function, NULL);
    cout << "This is thread 1, thread id: " << pthread_self() << endl;

    fout.open("sample.txt");
    int i = 0;
    while (fout && i < 10) {
        cout << "thread (1) writing to file, line:   " << lines[i] << endl;
        fout << lines[i] << endl;
        i++;
    }

    fout.close();
    return 0;
}

/*
OUTPUTS: 

yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
This is thread 1, thread id: 0x1042e0580
This is thread 2, thread id: 0x16be47000
thread (1) writing to file, line:   this is line number 1: gjeg
thread (1) writing to file, line:   this is line number 2: gdg
thread (1) writing to file, line:   this is line number 3: dfl
thread (1) writing to file, line:   this is line number 4: fef
thread (1) writing to file, line:   this is line number 5: efljwf
thread (1) writing to file, line:   this is line number 6: fefk
thread (1) writing to file, line:   this is line number 7: dfs
thread (1) writing to file, line:   this is line number 8: dfesfw
thread (1) writing to file, line:   this is line number 9: fwefw
thread (1) writing to file, line:   this is line number 10: ljlj



yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
This is thread 1, thread id: 0x1043f8580
This is thread 2, thread id: 0x16be27000
thread (1) writing to file, line:   this is line number 1: gjeg
thread (1) writing to file, line:   this is line number 2: gdg
thread (1) writing to file, line:   this is line number 3: dfl
thread (1) writing to file, line:   this is line number 4: fef
thread (1) writing to file, line:   this is line number 5: efljwf
thread (1) writing to file, line:   this is line number 6: fefk
thread (1) writing to file, line:   this is line number 7: dfs
thread (1) writing to file, line:   this is line number 8: dfesfw
thread (1) writing to file, line:   this is line number 9: fwefw
thread (2) reading from file, line:     this is line number 1: gjeg
thread (2) reading from file, line:     this is line number 2: gdg
thread (2) reading from file, line:     this is line number 3: dfl
thread (2) reading from file, line:     this is line number 4: fef
thread (2) reading from file, line:     this is line number 5: efljwf
thread (2) reading from file, line:     this is line number 6: fefk
thread (1) writing to file, line:   this is line number 10: ljlj



yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
This is thread 1, thread id: 0x1046d0580
This is thread 2, thread id: 0x16bbbf000
thread (1) writing to file, line:   this is line number 1: gjeg
thread (1) writing to file, line:   this is line number 2: gdg
thread (1) writing to file, line:   this is line number 3: dfl
thread (1) writing to file, line:   this is line number 4: fef
thread (1) writing to file, line:   this is line number 5: efljwf
thread (1) writing to file, line:   this is line number 6: fefk
thread (1) writing to file, line:   this is line number 7: dfs
thread (2) reading from file, line:     this is line number 1: gjeg
thread (2) reading from file, line:     this is line number 2: gdg
thread (2) reading from file, line:     this is line number 3: dfl
thread (2) reading from file, line:     this is line number 4: fef
thread (1) writing to file, line:   this is line number 8: dfesfw
thread (1) writing to file, line:   this is line number 9: fwefw
thread (1) writing to file, line:   this is line number 10: ljlj
*/