#include <string>
#include <cstdio>
#include <vector>

int main(int argc, char** argv) {
    const size_t BUF = 4096;

    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments: %d\nUsage: FILE PATTERN", argc - 1);
        return -1;
    }

    FILE *fin;
    if ((fin = fopen(argv[1], "r")) == nullptr) {
        fprintf(stderr, "File does not exist at specified path: \"%s\"", argv[1]);
        return -2;
    }

    std::string str = argv[2];
    str += "$";
    size_t str_len = str.length();

    std::vector<unsigned> prefix(str_len);
    for (size_t i = 1; i < str_len; i++) {
        unsigned k = prefix[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = prefix[k - 1];
        }
        if (str[i] == str[k]) {
            k++;
        }
        prefix[i] = k;
    }

    char tmp[BUF];

    unsigned previous = 0;
    while (!feof(fin)) {
        if (ferror(fin)) {
            fclose(fin);
            fprintf(stderr, "Can't read file \"%s\"", argv[1]);
            return -3;
        }
        size_t cnt = fread(tmp, sizeof(char), BUF, fin);
        for (size_t pos = 0; pos < cnt; pos++) {
            unsigned j = previous;
            while (j > 0 && tmp[pos] != str[j]) {
                j = prefix[j - 1];
            }
            if (tmp[pos] == str[j]) {
                j++;
            }
            if (j == str_len - 1) {
                fclose(fin);
                fprintf(stdout, "Yes");
                return 0;
            }
            previous = j;
        }
    }

    fclose(fin);
    fprintf(stdout, "No");
}
