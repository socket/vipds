/*
 *  vds_common.c
 *  server
 *
 *  Created by Alexey Streltsow on 3/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#import "vds_common.h"
#import <stdarg.h>

void
die(const char* error, ...) {
  va_list args;
  char buffer[256];
  
  vsprintf( buffer, error, args );
  printf("fatal: %s", buffer );
  
  free( buffer );
  
  exit( -1 );
}

void
die_trace(const char* filename, int line, const char* error, ...) {
  va_list args;
  char buffer[256];
  
  vsprintf( buffer, error, args );
  printf("fatal[%s:%d]: %s", filename, line, buffer );
  
  free( buffer );
  
  exit( -1 );
}