

#include "c-call-python.h"



PyObject* c_call_py_init(char* path,char* file)
{
    PyObject *pName, *pModule=NULL;
    Py_Initialize();

    PySys_SetPath(path);  // path to the module to import
    pName = PyString_FromString(file);
    if(pName==NULL){
        printf("module name is NULL\n");
    }
    pModule = PyImport_Import(pName);
    if(pModule==NULL){
        printf("can't import python file\n");
    }
    return pModule;
}

PyObject* c2p_parse_arg(struct c2p_argv* argv)
{
    PyObject *pValue=NULL;
    switch(argv->type)
    {
        case C2P_ARG_TYPE_INT:
            pValue = PyInt_FromLong(*(int *)(argv->value));
            break;
        case C2P_ARG_TYPE_STR:
            pValue = PyString_FromString((char*)(argv->value));
            break;
        default:
            break;
    }
    return pValue;
}
PyObject* c2p_parse_args(struct c2p_param* param)
{
    PyObject *pythonArgument=NULL;
    PyObject *pValue=NULL;
    struct c2p_argv* argv=NULL;
    int i;
    if(param->argc == 0) return NULL;
    pythonArgument = PyTuple_New(param->argc);
    argv=param->argv;
    for(i=0;i<param->argc;i++){
        pValue = c2p_parse_arg(&argv[i]);
        if (pValue == NULL) {
            return NULL;
        }
        PyTuple_SetItem(pythonArgument, i, pValue);
    }
    return pythonArgument;
}
PyObject* c_call_py(void* pyobj,char* func,struct c2p_param* param)
{
    PyObject *pResult=NULL, *pFunc=NULL;
    PyObject *pythonArgument=NULL;
    pFunc = PyObject_GetAttrString(pyobj, func);
    if(pFunc==NULL || !PyCallable_Check(pFunc)){
        return NULL;
    }
    pythonArgument = c2p_parse_args(param);
    if(pythonArgument==NULL) return NULL;
    pResult = PyObject_CallObject(pFunc, pythonArgument);
    return pResult;
}

#ifdef TEST_C_CALL_PYTHON
int c_call_python(int argc, char *argv[])
{
    int ret=0;
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
 
    Py_Initialize();
 
    PySys_SetPath("./");  // path to the module to import
    pName = PyString_FromString(argv[1]);
 
    pModule = PyImport_Import(pName);
    if (pModule != NULL) {
        PyObject *pythonArgument;
        pythonArgument = PyTuple_New(1);
        pValue = PyString_FromString(argv[3]);
 
        if (pValue == NULL) {
            ret=-1;
            goto out;
        }
        PyTuple_SetItem(pythonArgument, 0, pValue);
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        if (pFunc && PyCallable_Check(pFunc)) {
            pValue = PyObject_CallObject(pFunc, pythonArgument);
            if (pValue != NULL) {
                printf("Value returuend from the function %s", PyString_AsString(pValue));
            } else {
                PyErr_Print();
		ret= -1;
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
            ret=-1;
        }
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        ret=-1;
    }
out:
    Py_Finalize();
    return ret;
}
 
 
int main(int argc,char** argv)
{
	uint32_t reg;
	if(argc < 3){
		printf("command line is: c-python [python-file-name] [python-function-name]\n");
		return -1;
	}
	//c_call_python(argc,argv);
	reg=c2p_pci_configure_space_read(0x0,4);
	printf("reg value is 0x%x\n",reg);
#if 0
	printf("c calls python %s\n",argv[1]);
	Py_Initialize();
	PySys_SetPath("./");
 
	const char* kModuleName = argv[1];//"mymain";
	PyObject* module_name = PyString_FromString(kModuleName);
	PyObject* module = PyImport_Import(module_name);
 
	PyObject* dic = PyModule_GetDict(module);
	const char* kFuncName = argv[2];//"main";
	PyObject* main_func = PyDict_GetItemString(dic, kFuncName);
	assert(PyCallable_Check(main_func));
 
	PyObject* main_args = PyTuple_New(1);
	PyObject* main_args_0 = PyString_FromString("Hello, Python!");
	PyTuple_SetItem(main_args, 0, main_args_0);
	PyObject_CallObject(main_func, main_args);
#endif
	Py_Finalize();
	return 0;
}
#endif