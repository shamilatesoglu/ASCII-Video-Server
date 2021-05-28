//
// Created by MSA on 28/05/2021.
//

#include <string.h>
#include <stdio.h>
#include "video.h"
#include "string_utils.h"

void
parse_frames(char *video_buffer, char **video_map)
{
    int count = 0;
    int len = 1;
    int row_count = 0;

    char *p = video_buffer;
    video_map[count] = video_buffer;

    while (*p != '\0')
    {
        if (strncmp(p++, "\n", len))
            continue;
        row_count++;
        if (row_count % (VIDEO_HEIGHT + 1) == 0)
        {
            video_map[++count] = p;
        }
    }
}

int
get_number_of_frames(char *video_buffer)
{
    int line_count = count_substring(video_buffer, "\n");
    return line_count / (VIDEO_HEIGHT + 1);
}

void
get_time_to_display(char **video_map, int frame_idx, int *time_to_display)
{
    sscanf(video_map[frame_idx], "%d", time_to_display);
}

void
framecast(char **video_map, int frame_idx, char *frame)
{
    char *start = strstr(video_map[frame_idx], "\n") + 1;
    char *end = video_map[frame_idx + 1];
    long size = end - start;
    strncpy(frame, start, size - 1);
    frame[size] = '\0';
}