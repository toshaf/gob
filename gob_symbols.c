#include <bfd.h>
#include <stdio.h>

int gob_dump_symbols(char *const path) {
    
    bfd_init();

    bfd *abfd = bfd_openr(path, NULL);
    if (!abfd)
        return -1;

    if(!bfd_check_format(abfd, bfd_object))
        return -1;

    int symtab_size = bfd_get_symtab_upper_bound(abfd);

    printf("%lu symbols found\n", symtab_size / sizeof(asymbol));

    bfd_close(abfd);

    return 0;
}
