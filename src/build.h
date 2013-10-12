#ifndef _BUILD_H_
#define _BUILD_H_

// Some nasty macro definitions are and will be defined here.
// Keep your children away from this file.

#define OBSERVABLE_COLLECTION_SIZE 512

// TODO: setup logging level to work with DEBUG, WARNING, INFO macros

#define DEBUG(Args...)                                                  \
    do {                                                                \
        fprintf(stderr, "[ DEBUG ]: ");                                 \
        fprintf(stderr, ##Args);                                        \
        fprintf(stderr, "\n");                                          \
    } while(0)

#define ERROR(Args...)                                                  \
    do {                                                                \
        fprintf(stderr, "[ ERROR ]: ");                                 \
        fprintf(stderr, ##Args);                                        \
        fprintf(stderr, "\n");                                          \
    } while(0)

#define INFO(Args...)                                                   \
    do {                                                                \
        fprintf(stderr, "[ INFO ]: ");                                  \
        fprintf(stderr, ##Args);                                        \
        fprintf(stderr, "\n");                                          \
    } while(0)

#define WARNING(Args...)                                                \
    do { fprintf(stderr, "[ WARNING ]: ");                              \
        fprintf(stderr, ##Args);                                        \
        fprintf(stderr, "\n");                                          \
    } while(0)

// TODO: create macro for file logging

//log = fopen("log.txt", "a");
//fprintf(log, "%ld\n", bp->event);
//fclose(log);

// TODO: User configurable stuff

// depends on keyboard layout, its [ALT] or [WIN] key
#define MODKEY XCB_MOD_MASK_1

#define MOUSEMODKEY XCB_MOD_MASK_1

#endif /* _BUILD_H_ */
