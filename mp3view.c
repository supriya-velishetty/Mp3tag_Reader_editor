/*
 * Project     : MP3-Tag-Reader-Editor
 * File        : mp3view.c
 * Author      : Supriya Velishetty
 * Description : Display MP3 text metadata from ID3v2.3 frames.
 */

#include <stdio.h>
#include <string.h>
#include "mp3tag_reader.h"

int view_mp3_tag(const char *filename)
{
    MP3Info info;
    char id[5];
    unsigned int size, used = 0, i, start;
    unsigned char data[MAX_TEXT];
    int result;
    const char *label;
    if (open_mp3(&info, filename, "rb") != 0)
        return -1;
    printf("\n----------------------------------------------------------------------\n");    
    printf("\t\t MP3 TAG READER AND EDITOR DETAILS \t\t");
    printf("\n----------------------------------------------------------------------\n");    
    while (used + 10 <= info.tag_size)
    {
        result = read_frame_header(info.file, id, &size);
        if (result == 0)
            break;
        if (result < 0 || size > info.tag_size - used - 10)
        {
            printf("ERROR: Invalid frame\n");
            close_mp3(&info);
            return -1;
        }
        used += 10 + size;
        label = NULL;
        if (strcmp(id, "TIT2") == 0) label = "Title";
        else if (strcmp(id, "TPE1") == 0) label = "Artist";
        else if (strcmp(id, "TALB") == 0) label = "Album";
        else if (strcmp(id, "TYER") == 0) label = "Year";
        else if (strcmp(id, "TCON") == 0) label = "Genre";
        else if (strcmp(id, "COMM") == 0) label = "Comment";
        if (label == NULL)
        {
            if (fseek(info.file, (long)size, SEEK_CUR) != 0) break;
            continue;
        }
        if (size == 0 || size > MAX_TEXT || fread(data, 1, size, info.file) != size)
        {
            printf("ERROR: Unsupported or incomplete text frame\n");
            close_mp3(&info);
            return -1;
        }
        if (data[0] != 0)
        {
            printf("%s : [text encoding not supported]\n", label);
            continue;
        }
        start = 1;
        if (strcmp(id, "COMM") == 0)
        {
            /* Comment format: encoding + 3-byte language + description + NUL + text */
            start = 4;
            while (start < size && data[start] != 0) start++;
            if (start < size) start++;
        }
        printf("%-8s\t:\t ", label);
        for (i = start; i < size && data[i] != 0; i++)
            putchar(data[i]);
        printf("\n");
    }
    printf("------------------------------------------------------------------------\n");    
    close_mp3(&info);
    return 0;
}
