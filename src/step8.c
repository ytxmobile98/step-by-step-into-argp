#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_opt(int key, char *arg, struct argp_state *state) {
    int *arg_count = state->input;
    switch (key) {
    case 'd':
        {
            unsigned int i;
            unsigned int dots = 1;
            if (arg != NULL)
                dots = atoi(arg);
            for (i = 0; i < dots; i++)
                printf(".");
            break;
        }
        break;
    case 888:
        printf("-");
        break;
    case 777:
        return parse_opt('d', "3", state);
    case ARGP_KEY_ARG:
      argp_failure(state, 1, 0, "too many arguments");
      break;
    case ARGP_KEY_END:
        printf("\n");
        break;
    }
    return 0;
}

int main(int argc, char **argv) {
    struct argp_option options[] = {
        {"dot", 'd', "NUM", OPTION_ARG_OPTIONAL, "Show some dots on the screen"},
        {"ellipsis", 777, 0, OPTION_HIDDEN, "Show an ellipsis on the screen"},
        {"dash", 888, 0, 0, "Show a dash on the screen"},
        {0},
    };
    int arg_count = 4;
    struct argp argp = {options, parse_opt, "WORD [WORD [WORD [WORD]]]"};
    return argp_parse(&argp, argc, argv, 0, 0, 0);
}