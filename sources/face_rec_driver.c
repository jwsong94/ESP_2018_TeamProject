#include <Python.h>
#include <string.h>
#include <time.h>
#include "face_rec_driver.h"

static PyObject *p_name, *p_module, *p_func;
static PyObject *p_value;
static FILE *fp_name;

int init_face_recognizer(void)
{
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    p_name = PyUnicode_DecodeFSDefault("face_rec");

    if ((p_module = PyImport_Import(p_name)) == NULL) {
        PyErr_Print();
        Py_DECREF(p_name);
        fprintf(stderr, "Failed to load .py file\n"); 
        return -1;
    }

    p_func = PyObject_GetAttrString(p_module, "recognize_face");
    if (!((p_func != NULL) && PyCallable_Check(p_func)))  {
        PyErr_Print();
        Py_DECREF(p_name);
        Py_DECREF(p_module);
        return -1;
    }
/*
    if ((fp_name = fopen("./name/name_list", "r")) == NULL) {
        fprintf(stderr, "file open error on %s\n", __func__);
        return -1;
    }
    */

    return 0;
}

int close_face_recognizer(void)
{
    Py_DECREF(p_name);
    Py_DECREF(p_module);
    Py_DECREF(p_func);
    Py_DECREF(p_value);

    return 0;
}



struct log face_recognition(char *path)
{
    struct log result;

    memset(&result, 0, sizeof(result));

    p_value = PyObject_CallFunction(p_func, "s", path);
    if (p_value != NULL) {
        time_t now;
        size_t len;

        result.index = (int) PyLong_AsLong(p_value);
   //   fseek(fp_name, NAME_LEN * result.index, SEEK_SET); 
    //  fread(result.name, NAME_LEN, 1, fp_name); 

        now = time(NULL);
        strncpy(result.access_time, ctime(&now), sizeof(result.access_time) - 1);
        len = strlen(result.access_time);
        result.access_time[len-1] = '\0';

        printf("%d\n", result.index);
        printf("%s\n", result.access_time);
     // printf("%s\n", result.name);
    }
    else  {
        fprintf(stderr, "Call failed\n");
        return result;
    }
    Py_DECREF(p_value);

    return result;
}
