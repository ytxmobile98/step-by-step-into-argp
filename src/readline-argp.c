#include "readline-argp.h"
#include <unistd.h>

char *default_history_file = ".history";

static int parse_opt(int key, char *arg, struct argp_state *state) {
    struct readline_arguments *args = state->input;
    switch (key) {
    case 411:
        if (access(arg, R_OK))
            args->history_file = arg;
        else if (access(arg, W_OK))
            args->history_file = arg;
        else
            argp_failure(state, 1, 0, "Cannot open `%s' for reading", arg);
        break;
    case 511:
        args->history_file = NULL;
        break;
    case ARGP_KEY_INIT:
        args->history_file = default_history_file;
        break;
    }
    return 0;
}

static struct argp_option options[] = {
    {"history-file", 411, "FILE", 0,
     "Specify a FILE to store the interactive history"},
    {"no-history", 511, 0, 0, "Do not record a history in interactive mode"},
    {0},
};

struct argp readline_argp = {options, parse_opt, 0, 0, 0};