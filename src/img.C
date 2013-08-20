#include <stdio.h>
#include <stdlib.h>

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

    // see if it is a PNG
    char magic_numbers[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
    char * buffer = (char*) malloc(8);
    bool is_png = true;
    // read the first 8 bytes
    int result = fread(buffer, 1, 8, file);
    // make sure we were able to read
    if (result != 8)
    {
        printf( "Could not read file.\n" );
        return -1;
    }
    // loop through the first 8 bytes and compare them the the magic numbers
    for (int i = 0; i < 8; i++)
    {
        if (buffer[i] != magic_numbers[i]) 
        {
            is_png = false;
            break;
        }
    }

    // compare the first 8 bytes to the magic numbers
    if (is_png)
    {
        printf("This is a PNG.\n");
    }
    else
    {
        printf("This NOT is a PNG.\n");
    }

    // close the file
    fclose(file);
}
