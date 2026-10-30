/*
 * Project     : MP3-Tag-Reader-Editor
 * File        : mp3tag_edit.c
 * Author      : Supriya Velishetty
 * Description : Safely replace existing text within an ID3v2.3 frame.
 */

#include <stdio.h>
#include <string.h>
#include "mp3tag_reader.h"

int edit_mp3_tag(const char *filename, const char *option, const char *new_text)
{
    MP3Info info;
    char id[5];
    const char *wanted;
    unsigned int size, used = 0, i, start, length;
    unsigned char data[MAX_TEXT];
    int result;
    long data_position;

    if (strcmp(option, "-t") == 0) wanted = "TIT2";
    else if (strcmp(option, "-a") == 0) wanted = "TPE1";
    else if (strcmp(option, "-A") == 0) wanted = "TALB";
    else if (strcmp(option, "-y") == 0) wanted = "TYER";
    else if (strcmp(option, "-m") == 0) wanted = "TCON";
    else if (strcmp(option, "-c") == 0) wanted = "COMM";
    else
    {
        printf("ERROR: Invalid edit option\n");
        return -1;
    }
    length = (unsigned int)strlen(new_text);
    if (open_mp3(&info, filename, "rb+") != 0)
        return -1;

    while (used + 10 <= info.tag_size)
    {
        result = read_frame_header(info.file, id, &size);
        if (result == 0) break;
        if (result < 0 || size > info.tag_size - used - 10)
        {
            printf("ERROR: Invalid frame\n");
            close_mp3(&info);
            return -1;
        }
        used += 10 + size;
        if (strcmp(id, wanted) != 0)
        {
            if (fseek(info.file, (long)size, SEEK_CUR) != 0) break;
            continue;
        }
        if (size == 0 || size > MAX_TEXT || fread(data, 1, size, info.file) != size)
        {
            printf("ERROR: Unsupported frame size\n");
            close_mp3(&info);
            return -1;
        }
        if (data[0] != 0)
        {
            printf("ERROR: Not supported\n");
            close_mp3(&info);
            return -1;
        }
        start = 1;
        if (strcmp(wanted, "COMM") == 0)
        {
            start = 4;
            while (start < size && data[start] != 0) start++;
            if (start < size) start++;
        }
        if (start >= size || length > size - start)
        {
            printf("ERROR: New text is too long for this frame\n");
            printf("Use shorter text this beginner editor does not resize frames.\n");
            close_mp3(&info);
            return -1;
        }
        /* Keep the same frame size so following frames and audio are untouched. */
        for (i = start; i < size; i++) data[i] = 0;
        for (i = 0; i < length; i++) data[start + i] = (unsigned char)new_text[i];
        data_position = ftell(info.file) - (long)size;
        if (fseek(info.file, data_position, SEEK_SET) != 0 ||
            fwrite(data, 1, size, info.file) != size)
        {
            printf("ERROR: Could not save updated frame\n");
            close_mp3(&info);
            return -1;
        }
        close_mp3(&info);
        printf("MP3 tag updated successfully\n");
        return 0;
    }
    close_mp3(&info);
    printf("ERROR: Requested frame not found\n");
    return -1;
}
