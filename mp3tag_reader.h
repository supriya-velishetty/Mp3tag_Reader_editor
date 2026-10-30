/*
 * Project     : MP3-Tag-Reader-Editor
 * File        : mp3tag_reader.h
 * Author      : Supriya Velishetty
 * Description : Simple ID3v2.3 MP3 metadata reader and editor.
 */

#ifndef MP3TAG_READER_H
#define MP3TAG_READER_H
#include <stdio.h>
#define MAX_TEXT 1024

typedef struct
{
    FILE *file;
    unsigned int tag_size;
} MP3Info;

int check_extension(char *filename);
int open_mp3(MP3Info *info, const char *filename, const char *mode);
void close_mp3(MP3Info *info);
int read_frame_header(FILE *file, char id[5], unsigned int *size);
int view_mp3_tag(const char *filename);
int edit_mp3_tag(const char *filename, const char *option, const char *new_text);
#endif
