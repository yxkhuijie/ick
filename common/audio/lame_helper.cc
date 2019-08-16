#include "common/audio/lame_helper.h"

#include <cstring>
#include <sstream>

int LameHelper::Encode(const std::string& pcm_data, std::string* mp3_data, settings_t settings) {
  lame_global_flags* gfp = NULL;
  gfp = lame_init();
  lame_set_in_samplerate(gfp, settings.in_samplerate);

  // init the id3 tag structure
  id3tag_init(gfp);
  id3tag_v2_only(gfp);
  id3tag_set_year(gfp, settings.year);
  id3tag_set_genre(gfp, settings.genre);
  id3tag_set_artist(gfp, settings.artist);
  id3tag_set_album(gfp, settings.album);
  id3tag_set_title(gfp, settings.title);
  id3tag_set_track(gfp, settings.track);
  id3tag_set_comment(gfp, settings.comment);

  // id3tag_set_albumart(gfp, settings.albumart, size);

  // setting Channels
  switch(settings.channels) {
    case EC_MONO:
      lame_set_mode(gfp, MONO);
      lame_set_num_channels(gfp, 1);
      break;
    case EC_STEREO:
      lame_set_mode(gfp, JOINT_STEREO);
      lame_set_num_channels(gfp, 2);
      break;
  }

  int abr_bitrate = settings.abr_bitrate;
  // VbrTag
  lame_set_bWriteVbrTag(gfp, 1);  
  switch(settings.enc_mode) {
    case EM_ABR:
      if(abr_bitrate >= 8000) {
        abr_bitrate = (abr_bitrate + 500) / 1000;
      }
      if(abr_bitrate > 320) {
        abr_bitrate = 320;
      }
      if(abr_bitrate < 8) {
        abr_bitrate = 8;
      }
      lame_set_VBR(gfp, vbr_abr);
      lame_set_VBR_mean_bitrate_kbps(gfp, abr_bitrate);
      break;
    case EM_CBR:
      lame_set_VBR(gfp, vbr_off);
      lame_set_quality(gfp, settings.quality);
      lame_set_brate(gfp, settings.cbr_bitrate);
      break;
    case EM_VBR:
      lame_set_VBR(gfp, vbr_mtrh);
      lame_set_VBR_q(gfp, settings.quality);
      break;
  }

  lame_set_out_samplerate(gfp, settings.resample_frequency);

  lame_set_findReplayGain(gfp, 1);
  lame_set_write_id3tag_automatic(gfp, 0); // dont write id3tag, will write it myself

  int lResult = 0;
  if(lame_init_params(gfp) == -1) {
    // lame initialization failed
    printf("FATAL ERROR: parameters failed to initialize properly in lame. Aborting!\n");
    return -2;
  } else {
    int read = 0;
    int write = 0;
    long PCM_total_size = pcm_data.length(); 
    long cumulative_read = 0;

    unsigned char* buffer = new unsigned char[LAME_MAXMP3BUFFER];

    short int pcm_buffer[PCM_SIZE * 2];
    unsigned char mp3_buffer[MP3_SIZE];

    std::stringstream result;
    int imp3 = lame_get_id3v2_tag(gfp, buffer, LAME_MAXMP3BUFFER);
    result << std::string((char*)buffer, imp3);
    // fwrite(buffer, sizeof(char), imp3, mp3);  // write the id3v2 tag
    // long audio_pos=ftell(mp3);  // store beginning of audio data

    do {
      // read = fread(pcm_buffer, 2 * sizeof(short int), PCM_SIZE, pcm);
      const char* audio_data = pcm_data.c_str() + cumulative_read;
      read = PCM_SIZE;
      if (cumulative_read + 2 * sizeof(short int) * PCM_SIZE  >= pcm_data.length()) {
        read = (pcm_data.length() - (cumulative_read + 2 * sizeof(short int) * PCM_SIZE)) / (2 * sizeof(short int));
      }
      for (int i = 0; i < read; ++i) {
        memcpy(pcm_buffer, audio_data, 2 * sizeof(short int) * read);
      }
      cumulative_read += (long)read * 2 * sizeof(short int);
      if(read == 0) {
        write = lame_encode_flush(gfp, mp3_buffer, MP3_SIZE);
      } else {
        write = lame_encode_buffer_interleaved(gfp, pcm_buffer, read, mp3_buffer, MP3_SIZE);
      }
      // fwrite(mp3_buffer, write, sizeof(char), mp3);
      result << std::string((char*)mp3_buffer, write);
      // Percentage done
      int percentage = ((float)cumulative_read / PCM_total_size)*100;
    } while (read != 0);

    imp3 = lame_get_id3v1_tag(gfp, buffer, LAME_MAXMP3BUFFER);  
    // fwrite(buffer, sizeof(char), imp3, mp3);
    result << std::string((char*)buffer, imp3);

    imp3 = lame_get_lametag_frame(gfp, buffer, LAME_MAXMP3BUFFER);
    // if(fseek(mp3, audio_pos, SEEK_SET) != 0) {
    //   errorHandler("WARNING: can't seek back to update LAME-tag frame!\n");
    // }
    delete [] buffer;
  }

  lame_close(gfp);
  return lResult;
}
