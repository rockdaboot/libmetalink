/*
 * Sample for libmetalink parser.
 *
 * To compile:
 * gcc -Wall -g -O2 -o sample sample.c -lmetalink
 */
#include <stdio.h>
#include <stdlib.h>
#include <metalink/metalink_parser.h>

int main(int argc, char** argv)
{
  int r;
  metalink_t* metalink;
  metalink_file_t* file;
  metalink_checksum_t** checksums;
  
  r = metalink_parse_file("ubuntu-7_10-server-i386_iso.metalink", &metalink);

  if(r != 0) {
    fprintf(stderr, "ERROR: code=%d\n", r);
    exit(EXIT_FAILURE);
  }
 
  file = metalink->files[0];
  printf("name: %s\n", file->name);
  printf("size: %lld\n", file->size);
  printf("os  : %s\n", file->os);

  if(file->checksums) {
    checksums = file->checksums;
    while(*checksums) {
      printf("hash: %s %s\n", (*checksums)->type, (*checksums)->hash);
      ++checksums;
    }
  }
  if(file->chunk_checksum) {
    size_t count = 0;
    metalink_piece_hash_t** piece_hashes;
    printf("chunk checksum: size=%d, type=%s\n", file->chunk_checksum->length,
	   file->chunk_checksum->type);
    printf("first 5 piece hashes...\n");
    piece_hashes = file->chunk_checksum->piece_hashes;
    while(*piece_hashes && count < 5) {
      printf("piece=%d, hash=%s\n", (*piece_hashes)->piece, (*piece_hashes)->hash);
      ++piece_hashes;
      ++count;
    }
    printf("...\n");
  }
  if(file->resources) {
    size_t count = 0;
    metalink_resource_t** resources;
    printf("first 5 resources...\n");
    resources = file->resources;
    while(*resources && count < 5) {
      printf("type=%s, location=%s, preference=%d, url=%s\n",
	     (*resources)->type, (*resources)->location,
	     (*resources)->preference, (*resources)->url);
      ++resources;
      ++count;
    }
    printf("...\n");
  }

  /* delete metalink_t */
  delete_metalink(metalink);

  return EXIT_SUCCESS;
}
