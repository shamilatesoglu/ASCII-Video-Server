//
// Created by MSA on 28/05/2021.
//

#ifndef STREAMING_SERVER_VIDEO_H
#define STREAMING_SERVER_VIDEO_H

#define VIDEO_WIDTH 67
#define VIDEO_HEIGHT 13
#define MAX_FRAME_COUNT 3500

#define MAX_VIDEO_BYTES ((VIDEO_WIDTH + 2) * (VIDEO_HEIGHT + 1) * MAX_FRAME_COUNT + 1)  // 67 x 13 resolution, 20 fps, 20 seconds.

#define CHANNEL_COUNT 3

void
parse_frames(char *video_buffer, char **video_map);

int
get_number_of_distinct_frames(char *video_buffer);

int
get_number_of_frames(char **video_map, int distinct);

void
get_time_to_display(char **video_map, int frame_idx, int *time_to_display);

void
framecast(char **video_map, int frame_idx, char *frame);

#endif //STREAMING_SERVER_VIDEO_H
