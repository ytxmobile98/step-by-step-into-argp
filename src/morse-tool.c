#define _GNU_SOURCE
#include "dotdash.h"
#include "morse.h"
#include "readline-argp.h"
#include <argp.h>
#include <argz.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

const char *argp_program_bug_address = "samuel@morse.net";
const char *argp_program_version = "version 1.0";

struct arguments {
    int capitalize;
    char *argz;
    size_t argz_len;
    struct readline_arguments readline;
};

static struct argp_option options[] = {
    {"capitalize", 'c', 0, 0, "Show morse translations in capital letters"},
    {0},
};
static struct argp_child children_parsers[] = {
    {&readline_argp, 0, 0, 0},
    {0},
};

static int parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *a = (struct arguments *)state->input;

    switch (key) {
    case 'c':
        a->capitalize = 1;
        break;
    case ARGP_KEY_ARG:
        argz_add(&a->argz, &a->argz_len, arg);
        break;
    case ARGP_KEY_INIT:
        a->argz = NULL;
        a->argz_len = 0;
        a->capitalize = 0;
        state->child_inputs[0] = &a->readline;
        break;
    }
    return 0;
}

static char* help_filter(int key, const char *text, void *input) {
    if (key == ARGP_KEY_HELP_POST_DOC) {
        char *new_text = NULL;
        if (asprintf(&new_text, text, default_history_file) != -1)
            return new_text;
    }
    return (char*)text;
}

struct argp argp = {
    options,
    parse_opt,
    "[PHRASE]",
    "Translates to and from morse code.\vThis program starts in interactive mode when PHRASE is not supplied on the command line. The history for interactive mode is stored in a file called `%s' by default.",
    children_parsers,
    help_filter,
};

static void interactive_mode(char *prompt, int capitalize) {
    char *line;
    char *argz = NULL;
    size_t argz_len = 0;

    while ((line = readline(prompt))) {
        if (strncmp(line, "tap ", 4) == 0 || strcmp(line, "tap") == 0) {
            /* parse the tap command with the dotdash options. */
            if (argz_create_sep(line, ' ', &argz, &argz_len) == 0) {
                int flags = ARGP_NO_EXIT | ARGP_NO_ARGS;
                int argc = argz_count(argz, argz_len);
                char *argv[argc + 1];
                argz_extract(argz, argz_len, argv);
                argv[argc] = 0;

                if (argp_parse(&dotdash_argp, argc, argv, flags, 0, 0) == 0) {
                    add_history(line);
                    printf("\n");
                }
            }
        } else if (strcmp(line, "quit") == 0)
            break;
        else {
            if (argz_create_sep(line, ' ', &argz, &argz_len) == 0) {
                if (morse_process_line(argz, argz_len, capitalize) == 0)
                    add_history(line);
            }
        }
    }

    return;
}

int main(int argc, char **argv) {
    struct arguments arguments;
    default_history_file = ".morse-tool.history";
    int retval = argp_parse(&argp, argc, argv, 0, 0, &arguments);
    if (retval != 0)
        return retval;

    /* silence --version and bug address in the tap command */
    argp_program_version = 0;
    argp_program_bug_address = 0;

    if (arguments.argz_len > 0) {
        morse_process_line(arguments.argz, arguments.argz_len,
                           arguments.capitalize);
        free(arguments.argz);
    } else if (arguments.argz_len == 0) {
        if (arguments.readline.history_file)
            read_history(arguments.readline.history_file);
        interactive_mode("morse> ", arguments.capitalize);
        if (arguments.readline.history_file)
            write_history(arguments.readline.history_file);
    }

    return retval;
}