#ifndef __NE_PYHELPER_HPP
#define __NE_PYHELPER_HPP
#pragma once

#include <string>
#include <vector>
#include <iostream>
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
    
    bool runFile(const std::string& path) {
        FILE* fp = fopen( path.c_str(), "r" );
        if ( fp == NULL )
        {
            std::cout << "[PyScript] Error opening file: " << path << std::endl;
            return false;
        }
        int re = PyRun_SimpleFile( fp, path.c_str() );
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

static CPyObject callAttributeOfPythonObject1(CPyObject pModule, const std::string& pFuncName, std::vector<std::string> pArgStrings) {
    CPyObject pFunc = PyObject_GetAttrString(pModule.getObject(), (char*)pFuncName.c_str());
    if (pFunc) {
        CPyObject pArgs = PyTuple_New(pArgStrings.size());
        for (int i = 0; i < pArgStrings.size(); ++i)
            PyTuple_SetItem(pArgs.getObject(), i, PyUnicode_FromString(pArgStrings.at(i).c_str()));
        return PyObject_CallObject(pFunc.getObject(), pArgs.getObject());
    }
    
    return NULL;
}

static CPyObject callAttributeOfPythonObject2(CPyObject pModule, const std::string& pFuncName, std::vector<std::string> pArgStrings) {
    CPyObject pFunc = PyObject_GetAttrString(pModule.getObject(), (char*)pFuncName.c_str());
    if (pFunc) {
        CPyObject pArgs = PyTuple_New(pArgStrings.size());
        for (int i = 0; i < pArgStrings.size(); ++i)
            PyTuple_SetItem(pArgs.getObject(), i, PyLong_FromString(pArgStrings.at(i).c_str(), NULL, 0));
        return PyObject_CallObject(pFunc.getObject(), pArgs.getObject());
    }
    
    return NULL;
}

#endif
