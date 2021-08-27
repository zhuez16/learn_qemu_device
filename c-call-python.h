//
// Created by mskhana on 2021/8/26.
//

#ifndef QEMU_C_CALL_PYTHON_H
#define QEMU_C_CALL_PYTHON_H
#define C2P_ARG_TYPE_INT 0
#define C2P_ARG_TYPE_STR 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "/usr/include/python3.8/Python.h"

struct c2p_argv{
    int type;
    void *value;
};
struct c2p_param{
    int argc;
    struct c2p_argv* argv;
};

PyObject* c_call_py_init(char *path, char *file);
PyObject* c_call_py(void *pyobj, char *func, struct c2p_param *param);
PyObject* c2p_parse_arg(struct c2p_argv *argv);
PyObject* c2p_parse_args(struct c2p_param *param);


#endif //QEMU_C_CALL_PYTHON_H
