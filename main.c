/*
 * Project     : MP3-Tag-Reader-Editor
 * File        : main.c
 * Author      : Supriya Velishetty
 * Description : Command-line interface to view or edit MP3 tags.
 */

#include <stdio.h>
#include <string.h>
#include "mp3tag_reader.h"

/* Check whether filename has .mp3 extension */
int check_extension(char *filename)
{
    char *ext = strrchr(filename, '.');

    if (ext != NULL && strcmp(ext, ".mp3") == 0)
        return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    /* STEP 1: Check view command */
    if (argc == 3 && strcmp(argv[1], "-v") == 0)
    {
        /* STEP 2: Check filename extension */
        if (check_extension(argv[2]) == 0)
        {
            printf("ERROR: File must have .mp3 extension\n");
            return 1;
        }

        /* STEP 3: Call function to view MP3 details */
        if (view_mp3_tag(argv[2]) != 0)
        {
            printf("ERROR: Unable to view MP3 tag\n");
            return 1;
        }

        return 0;
    }

    /* STEP 4: Check edit command */
    if (argc == 5 && strcmp(argv[1], "-e") == 0)
    {
        /* STEP 5: Check filename extension */
        if (check_extension(argv[4]) == 0)
        {
            printf("ERROR: File must have .mp3 extension\n");
            return 1;
        }

        /* STEP 6: Call function to edit MP3 tag */
        if (edit_mp3_tag(argv[4], argv[2], argv[3]) != 0)
        {
            printf("ERROR: Unable to edit MP3 tag\n");
            return 1;
        }

        return 0;
    }

    /* STEP 7: Check help command */
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        printf("VIEW:\n");
        printf("./a.out -v filename.mp3\n");

        printf("EDIT:\n");
        printf("./a.out -e -t NewTitle filename.mp3(to edit song title)\n");
        printf("./a.out -e -a NewArtist filename.mp3(to edit Artist name)\n");
        printf("./a.out -e -A NewAlbum filename.mp3(to edit album name)\n");
        printf("./a.out -e -y 2024 filename.mp3(to edit year)\n");
        printf("./a.out -e -m NewContent filename.mp3(to edit content)\n");
        printf("./a.out -e -c NewComment filename.mp3(to edit comment)\n");

        return 0;
    }

    /* STEP 8: Display invalid argument message */
    printf("ERROR: INVALID ARGUMENTS\n");
    printf("USAGE : \n");
    printf("To  View: ./a.out -v filename.mp3\n");
    printf("To  Edit: ./a.out -e -t/-a/-A/-y/-m/-c new_data filename.mp3\n");
    printf("For Help: ./a.out --help\n");

    return 1;
}
