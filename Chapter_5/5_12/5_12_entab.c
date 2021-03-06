/*
Exercise 5-12. Extend entab and detab to accept the shorthand
entab -m +n
to mean tab stops every n columns, starting at column m. Choose convenient 
(for the user) default behavior.

entab - build upon 1_21 and 5_11

when testing do not forget to set your ide's tab width settings properly!
*/

#include <stdio.h>
#include <ctype.h>  // for isdigit()
#include <stdlib.h> // for atoi()

int margc;
char **margv;

int startAt = -1;       // optional parameter
int tabStops = -1;      // optional parameter
int allCols = 0;        // column count row

#define MAXERRORS 50
int errors[MAXERRORS];  // this saves all wrong parameters
int currErr = -1;

int arg = 1;            // argument counter
int deftab = 8;         // default tab size

int getnewtab(void);


int main(int argc, char *argv[])
{
    // check arguments
    if (
        argc > 4 && 
        *argv[1] == '-' && 
        *(argv[1] + 1) == 'm' && 
        *argv[3] == '+' && 
        *(argv[3] + 1) == 'n' &&
        ( (startAt = atoi(argv[2])) <= 0 || (tabStops = atoi(argv[4])) <= 0 )
        )
    {
        printf("Usage: %s -m starting_at +n tab_stops [tab_stop ..]\n", 
            argv[0]);
        return 1;
    } else
        arg = 5;

    if (startAt != -1 && tabStops != -1) {
        printf("startAt : %i\n", startAt);
        printf("tabStops: %i\n", tabStops);
    }

    margc = argc;
    margv = argv;

    int c, i;
    int col = 0;                            // column count after last tab
    int currtab = getnewtab();              // tab size

    int sp = 0;     // space counter

    while((c = getchar()) != EOF) {
        if (c == '\n') {
            putchar(c);
            col = 0;
            allCols = 0;
            sp = 0;                         // also remove trailing spaces
            arg = (tabStops == -1)? 1:5;
            currtab = getnewtab();
        } else if (c == ' ') {              // space
            sp++;
            if ((col + sp) % currtab == 0) {// tab possible?
                putchar('\t');
                for (i = 0; i < currtab; i++)
                    allCols++;
                sp = 0;
                col = 0;

                // get next tab
                currtab = getnewtab();
            }

        } else {                            // everything else
            while (sp) {                    // print buffered spaces if any
                //putchar('_');             // for testing
                putchar(' ');
                col++;
                allCols++;
                sp--;
            }
            putchar(c);                     // default print char
            col++;
            allCols++;
        }
    }

    if (currErr >= 0) {
        printf("\n\n\n");
        for (i = 0; i < currErr; i++)
            printf("Bad tab value: '%s'. Assuming default: '%i'.\n", 
                argv[errors[i]], deftab);
    }

    printf("\n\ndone\n");
    return 0;
}

// save all errors
void countErrors(int ar) {
    if (currErr + 1 == MAXERRORS)
        return;

    errors[++currErr] = ar;
}

// returns the next tab size
int getnewtab(void)
{
    int currtab = deftab;

    if (startAt > 0 && tabStops > 0 && allCols >= startAt)
        currtab = tabStops;
    else if ((arg < margc) && ((currtab = atoi(margv[arg])) <= 0)) {
        currtab = deftab;
        countErrors(arg);
    }

    arg++;

    return currtab;
}
