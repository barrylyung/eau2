//lang::CwC

#pragma once
#include <stdarg.h>
#include <ctype.h>
#include "string.h"
#include "types.h"
#include "array.h"

class Util {
    public:

        static Array* buildArray(types type, int n, va_list ap) {
            switch (type) {
                case STRING: {
                    ArrOfStringArr* arr = new ArrOfStringArr();
                    for (int i = 0; i < n; i++) {
                        arr->pushBack(va_arg(ap, String*));
                    }
                    va_end(ap);
                    return arr;
                }
                case INT: {
                    ArrOfIntArray* arr = new ArrOfIntArray();
                    for (int i = 0; i < n; i++) {
                        arr->pushBack(va_arg(ap, int));
                    }
                    va_end(ap);
                    return arr;
                }
                case FLOAT: {
                    ArrOfFloatArray* arr = new ArrOfFloatArray();
                    for (int i = 0; i < n; i++) {
                        arr->pushBack(va_arg(ap, float));
                    }
                    va_end(ap);
                    return arr;
                }
                case BOOL: {
                    ArrOfBoolArray* arr = new ArrOfBoolArray();
                    for (int i = 0; i < n; i++) {
                        arr->pushBack(va_arg(ap, bool));
                    }
                    va_end(ap);
                    return arr;
                }
                default: printf("Unsupported type.\n");
                         exit(1);
                        break;
            }
            return NULL;
        }
};
