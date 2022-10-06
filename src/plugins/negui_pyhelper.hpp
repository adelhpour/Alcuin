#ifndef __NE_PYHELPER_HPP
#define __NE_PYHELPER_HPP
#pragma once

#include <Python.h>

class CPyInstance {
public:
    CPyInstance()
    {
        Py_Initialize();
    }

    ~CPyInstance()
    {
        Py_Finalize();
    }
    
    void appendPath(const char* path) {
        PyList_Append(PySys_GetObject("path"), PyUnicode_FromString(path));
    }
    
    PyObject* importModule(const char* module) {
        return PyImport_Import(PyUnicode_FromString(module));
    }
    
    bool runFile(const char* path) {
        FILE* fp = fopen(path, "r" );
        if ( fp == NULL )
            return false;
        
        int re = PyRun_SimpleFile(fp, path);
        fclose( fp );

        return (re == 0);
    }
};

class CPyObject {
public:
    CPyObject() : p(NULL) {
        
    }

    CPyObject(PyObject* _p) : p(_p) {
        
    }

    ~CPyObject() {
        Release();
    }

    PyObject* getObject() {
        return p;
    }

    PyObject* setObject(PyObject* _p) {
        return (p = _p);
    }

    PyObject* AddRef() {
        if (p) {
            Py_INCREF(p);
        }
        return p;
    }

    void Release() {
        if (p) {
            Py_DECREF(p);
        }

        p = NULL;
    }

    PyObject* operator ->() {
        return p;
    }

    bool is() {
        return p ? true : false;
    }

    operator PyObject*() {
        return p;
    }

    PyObject* operator = (PyObject* pp) {
        p = pp;
        return p;
    }

    operator bool() {
        return p ? true : false;
    }
    
private:
    PyObject *p;
};

#endif
