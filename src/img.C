#include <stdio.h>
#include <stdlib.h>

// is_png
//
// 
bool is_png (FILE * file) {
    char magic_numbers[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
    char * buffer;
    int result;

    // 
    // read the first 8 bytes
    //
    // create the buffer
    buffer = (char*) malloc(8);
    // make sure we got the chunk of memory
    if (buffer == NULL)
    {
        return false;
    }
    // seek to the beginning
    fseek(file, 0, SEEK_SET);
    // read
    result = fread(buffer, 1, 8, file);
    // error check the read
    if (result != 8)
    {
        free(buffer);
        return false;
    }

    // 
    // check that beginning of the file matches the magic numbers
    //
    // loop through the first 8 bytes and compare them the the magic numbers
    for (int i = 0; i < 8; i++)
    {
        // if they do not match, return false
        if (buffer[i] != magic_numbers[i]) 
        {
            free(buffer);
            return false;
        }
    }
    // if we got here, then they all match and return true
    free(buffer);
    return true;
}


main (int argc, char *argv[])
{
    // make sure the usage is correct
    if (argc != 2)
    {
        printf("usage: %s filename", argv[0]);
        return 0;
    }

    printf("Opening file: %s\n", argv[1]);

    // open the file
    FILE *file = fopen(argv[1], "r");
    // make sure we opened the file
    if (file == 0)
    {
        printf( "Could not open file.\n" );
        return -1;
    }

    // print the file size
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    printf("File size: %i bytes.\n", size);
    // seek back to the beginning
    fseek(file, 0, SEEK_SET);

    // see if its a PNG
    if (is_png(file))
    {
        printf("This is a PNG!\n");
    }
    else
    {
        printf("This NOT is a PNG.\n");
        fclose(file);
        return -1;
    }

    // go to the sart of the first chunk
    fseek(file, 8, SEEK_SET);
    char* buffer = (char*) malloc(4);
    int results = fread(buffer, 1, 4, file);
    printf("%hhx\n", buffer[0]);
    printf("%hhx\n", buffer[1]);
    printf("%hhx\n", buffer[2]);
    printf("%hhx\n", buffer[3]);
    printf("Chunk length: %i\n", *(int*)(buffer));
    results = fread(buffer, 1, 4, file);
    printf("Chunk type: %s\n", buffer);
    free(buffer);

    int test;
    printf("%lu\n", sizeof(test));

    // close the file
    fclose(file);

    return 1;
}
