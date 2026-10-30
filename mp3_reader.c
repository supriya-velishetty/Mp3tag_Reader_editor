/*
 * Project     : MP3-Tag-Reader-Editor
 * File        : mp3_reader.c
 * Author      : Supriya Velishetty
 * Description : Shared file opening and ID3v2.3 frame reading functions.
 */

#include <stdio.h>
#include "mp3tag_reader.h"

int open_mp3(MP3Info *info, const char *filename, const char *mode)
{
    unsigned char header[10];
    info->file = fopen(filename, mode);
    if (info->file == NULL)
    {
        perror("Cannot open MP3 file");
        return -1;
    }
    if (fread(header, 1, 10, info->file) != 10 ||
        header[0] != 'I' || header[1] != 'D' || header[2] != '3')
    {
        printf("ERROR: ID3 tag not found\n");
        close_mp3(info);
        return -1;
    }
    if (header[3] != 3 || header[4] != 0 || header[5] != 0)
    {
        printf("ERROR: Only ID3v2.3 without special tag flags is supported\n");
        close_mp3(info);
        return -1;
    }
    info->tag_size = ((unsigned int)(header[6] & 127) << 21) |
                     ((unsigned int)(header[7] & 127) << 14) |
                     ((unsigned int)(header[8] & 127) << 7) |
                     (unsigned int)(header[9] & 127);
    return 0;
}

void close_mp3(MP3Info *info)
{
    if (info->file != NULL)
    {
        fclose(info->file);
        info->file = NULL;
    }
}

int read_frame_header(FILE *file, char id[5], unsigned int *size)
{
    unsigned char header[10];
    int i;
    if (fread(header, 1, 10, file) != 10)
        return -1;
    if (header[0] == 0)
        return 0; /* padding */
    for (i = 0; i < 4; i++)
        id[i] = (char)header[i];
    id[4] = '\0';
    *size = ((unsigned int)header[4] << 24) |
            ((unsigned int)header[5] << 16) |
            ((unsigned int)header[6] << 8) |
            (unsigned int)header[7];
    if (header[8] != 0 || header[9] != 0)
    {
        printf("ERROR: Special frame flags are not supported\n");
        return -1;
    }
    return 1;
}
