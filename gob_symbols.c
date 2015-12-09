#include <bfd.h>
#include <stdio.h>
#include <stdlib.h>

int gob_dump_symbols(char *const path) {
    
    bfd_init();

    bfd *abfd = bfd_openr(path, NULL);
    if (!abfd)
        return -1;

    if(!bfd_check_format(abfd, bfd_object))
        return -1;

    int symtab_size = bfd_get_symtab_upper_bound(abfd);

    asymbol **asymtab = malloc(symtab_size);
    int numsymbols = bfd_canonicalize_symtab(abfd, asymtab);

    printf("%d symbols found\n", numsymbols);

    for (int i=0; i<numsymbols; ++i) {
        char* name = bfd_asymbol_name(asymtab[i]);
        int value  = bfd_asymbol_value(asymtab[i]);

        printf("> %s @ %d\n", name, value);
    }

    bfd_close(abfd);
    free(asymtab);

    return 0;
}
