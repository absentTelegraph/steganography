#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <byteswap.h>

class PNG_Chunk
{
    char length[4];
    char type[4];
    char data_crc[0];

public:

    int get_data_length ();
    int get_chunk_length ();
    char *get_type ();
    PNG_Chunk *get_next_chunk ();
};

class PNG
{
public:
    char magic_numbers[8];
    PNG_Chunk chunk_IHDR;
    
};

int PNG_Chunk::get_data_length () {
    return __bswap_32(*(int *)length);
}

int PNG_Chunk::get_chunk_length () {
    return __bswap_32(*(int *)length) + 12;
}

char *PNG_Chunk::get_type () {
    return type;
}

PNG_Chunk *PNG_Chunk::get_next_chunk () {
    if (strncmp(this->type, "IEND", 4) == 0) 
    {
        return 0;
    }
    return (PNG_Chunk*)&this->data_crc[this->get_chunk_length() -  sizeof(PNG_Chunk)];
}

// is_png
//
// check that beginning of the file matches the PNG magic numbers
bool is_png (char *map) {
    char magic_numbers[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
    int i;

    // loop through the first 8 bytes and compare them the the magic numbers
    for (i = 0; i < 8; i++)
    {
        // if they do not match, return false
        if (map[i] != magic_numbers[i]) 
        {
            return false;
        }
    }

    // if we got here, then they all match and return true
    return true;
}


main (int argc, char *argv[])
{
    int fd;        // file descriptor
    int file_size; // size of the file
    char *map;     // pointer to memory mapped file
    PNG *png;      // instance of PNG class
    int offset;    // off set into the mmap (should be pointing to beginning of a chunk)
    PNG_Chunk *cur_chunk; // current chunk in the loop
    struct stat buf;

    // make sure the usage is correct
    if (argc != 2)
    {
        printf("\tUsage: %s <filename>\n", argv[0]);
        return 0;
    }
    printf("Opening file: %s\n", argv[1]);

    // open the file
    fd = open(argv[1], O_RDONLY);
    // make sure we opened the file
    if (fd == -1)
    {
        printf( "Could not open file.\n" );
        return -1;
    }

    // get the size of the file
    fstat(fd, &buf);
    file_size = buf.st_size;
    printf("File size: %i bytes.\n", file_size);

    // map the file
    map = (char *)(mmap(0, file_size, PROT_READ, MAP_SHARED, fd, 0));
    if (map == MAP_FAILED)
    {
        close(fd);
        printf("Error mmapping the file");
        return -1;
    }

    // see if its a PNG
    if (is_png(map))
    {
        printf("This is a PNG!\n");
    }
    else
    {
        printf("This NOT is a PNG.\n");
        close(fd);
        return -1;
    }

    // lets try to jam map into a clas
    png = (PNG *)(map);

    offset = 8;
    cur_chunk = &png->chunk_IHDR;
    do
    {
        printf("Chunk type: %.4s\n", cur_chunk->get_type());
        printf("Chunk length: %i\n", cur_chunk->get_data_length());
    }
    while (cur_chunk = cur_chunk->get_next_chunk());

    //
    // close everything up
    //

    if (munmap(map, file_size) == -1) 
    {
        printf("Error un-mmapping the file");
    }
    close(fd);
    return 1;
}
