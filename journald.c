#include <Python.h>
#include <systemd/sd-journal.h>

static PyObject *
journald_send(PyObject *self, PyObject *args) {
    struct iovec *iov = NULL;
    int argc = PyTuple_Size(args);
    int i;

    // Allocate sufficient iovector space for the arguments.
    iov = malloc(argc * sizeof(struct iovec));
    if (!iov) {
        return PyErr_NoMemory();
    }

    // Iterate through the Python arguments and fill the iovector.
    for (i = 0; i < argc; ++i) {
        PyObject *item = PyTuple_GetItem(args, i);
        char * stritem = PyString_AsString(item);
        iov[i].iov_base = stritem;
        iov[i].iov_len = strlen(stritem);
    }

    // Send the iovector to journald.
    sd_journal_sendv(iov, argc);

    // Free the iovector. The actual strings
    // are already managed by Python.
    free(iov);

    // End with success.
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef journaldMethods[] = {
    {"send",  journald_send, METH_VARARGS,
     "Send an entry to journald."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initjournald(void)
{
    (void) Py_InitModule("journald", journaldMethods);
}