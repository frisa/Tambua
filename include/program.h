#pragma once

#define CHECK_TRUE(x) if (!(x)) { \
        fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
        exit(1); \
    }
