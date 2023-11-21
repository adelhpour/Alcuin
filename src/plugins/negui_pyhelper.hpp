#ifndef __NE_PYHELPER_HPP
#define __NE_PYHELPER_HPP
#pragma once

#include <Python.h>
#include "iostream"

class CPyInstance {
public:
    CPyInstance(const char* appPath)
    {

#if EMBEDDABLE_PYTHON_DEVS_ARE_USED && defined(Q_OS_MAC)
        std::string pythonVersion = std::to_string(EMBEDDABLE_PYTHON_DEVS_VERSION_MAJOR) + "." + std::to_string(EMBEDDABLE_PYTHON_DEVS_VERSION_MINOR);
        std::string pythonHomeDirectory = std::string(appPath) + "/../Frameworks/Python.framework/Versions/" + pythonVersion;
        std::wstring ws(pythonHomeDirectory.begin(), pythonHomeDirectory.end());
        Py_SetPythonHome(ws.c_str());
#endif

        Py_Initialize();
    }

    ~CPyInstance()
    {
        Py_Finalize();
    }
    
    void appendPath(const char* pluginsPath) {
        PyList_Append(PySys_GetObject("path"), PyUnicode_FromString(pluginsPath));
    }
    
    PyObject* importModule(const char* module) {
        return PyImport_Import(PyUnicode_FromString(module));
    }
    
    bool runFile(const char* pluginsPath) {
        FILE* fp = fopen(pluginsPath, "r" );
        if ( fp == NULL )
            return false;
        
        int re = PyRun_SimpleFile(fp, pluginsPath);
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
