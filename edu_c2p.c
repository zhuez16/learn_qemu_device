#include "c-call-python.h"
#include "edu_c2p_pci.h"
static PyObject* pModule_PCI;
static PyObject* edu_c2p_find_pci_module(void)
{
#if 1
    char path[8];
    char name[64];
    memset(path,0,8);
    memset(name,0,64);
    strcpy(path,"./");
    strcpy(name,"qemu_pci_config_space");
#else
    char* path="./";
	char* name="qemu_pci_config_space";
#endif
    if(pModule_PCI==NULL)
        pModule_PCI=c_call_py_init(path,name);
    return pModule_PCI;
}
uint32_t edu_c2p_pci_configure_space_read(uint32_t addr, int len)
{
    PyObject* pResult=NULL, *pModule=NULL;
    char py_func[64];
    struct c2p_argv argv[2];
    struct c2p_param param;
    pModule=edu_c2p_find_pci_module();
    if(pModule==NULL) return 0;

    memset(py_func,0,64);
    strcpy(py_func,"pci_config_space_read");
    argv[0].type = C2P_ARG_TYPE_INT;
    argv[0].value =(void*)&addr;
    argv[1].type = C2P_ARG_TYPE_INT;
    argv[1].value =(void*)&len;
    param.argc = 2;
    param.argv=argv;

    pResult=c_call_py(pModule,py_func,&param);
    return PyInt_AsLong(pResult);
}