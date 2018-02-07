#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <elf.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/stat.h>
#include <sys/mman.h>


typedef struct handle
{
    Elf32_Ehdr *ehdr;
    Elf32_Phdr *phdr;
    Elf32_Shdr *shdr;
    uint8_t *mem;
    char *strtab;
    char *exec;
}handle_t;

int main(int argc, char **argv)
{
    int fd, i;
    handle_t h;
    struct stat st;

    if (argc < 2)
    {
        printf("Usage: %s <executable>\n", argv[0]);
        exit(0);
    }

    if ( (fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("open");
        exit(-1);
    }

    if (fstat(fd, &st) < 0)
    {
        perror("fstat");
        exit(-1);
    }

    h.mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (h.mem == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }

    if (h.mem[0] != 0x7f && strcmp(&h.mem[1], "ELF"))
    {
        fprintf(stderr, "%s is not an ELF file\n", argv[1]);
        exit(-1);
    }

    h.ehdr = (Elf32_Ehdr *)h.mem;
    h.phdr = (Elf32_Phdr *)(h.mem + h.ehdr->e_phoff);
    h.shdr = (Elf32_Shdr *)(h.mem + h.ehdr->e_shoff);
    h.strtab = (char *)(h.mem + h.shdr[h.ehdr->e_shstrndx].sh_offset);

    if (h.ehdr->e_type != ET_EXEC)
    {
        fprintf(stderr, "%s is not an executable\n", argv[1]);
        exit(-1);
    }

    printf("Section String Table list:\n");
    for (i=0; i<h.ehdr->e_shnum; i++)
    {
        printf("Str[%02d]: %16s,\tStart:0x%016x,\tEnd:0x%016x\n", \
            i, &h.strtab[h.shdr[i].sh_name], \
            h.shdr[i].sh_offset, \
            h.shdr[i].sh_offset + h.shdr[i].sh_size);
    }

    close(fd);
    exit(0);
}