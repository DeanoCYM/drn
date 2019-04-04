/* DRN_DLSYM.C
 *
 * DRN funtions that operate on a dynamically loaded shared object
 *
 */

#include "drn_dlsym.h"
#include "ert_log.h"
#include <dlfcn.h>
#include <unistd.h>

/* Returns the shared object at the file path 'libpathname' */
void *
open_library(char *libpathname)
{
    if (access(libpathname, R_OK)) {
	log_err("Cannot read shared object at %s", libpathname);
    } else {
	log_info("Read access OK for shared object at %s", libpathname);
    }

    void *so = dlopen(libpathname, RTLD_LAZY);

    if (!so) {
	log_err("Shared object could not be opened at %s", libpathname);
	return NULL;
    }

    return so;
}

/* Close the share object pointed to by 'so' */
void
close_library(void *so)
{
    if (dlclose(so))
	log_warn("Failed to close shared object");

    return;
}

/* Returns the symbol that matches the string 'libfn' in the shared object
   pointed at by 'so' */
getstr
read_cb(void *so, const char *libfn)
{
    if (strcmp(libfn, "") == 0) {
	log_warn("Empty string passed as command line arguement");
	return NULL;
    }

    getstr cb = dlsym(so, libfn);
    if (!cb) {
	log_warn("Failed to read %s() from %s", libfn, "lib");
	return NULL;
    }

    return cb;
}

