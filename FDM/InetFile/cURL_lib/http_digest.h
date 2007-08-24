/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __HTTP_DIGEST_H
#define __HTTP_DIGEST_H 

typedef enum {
  CURLDIGEST_NONE, 
  CURLDIGEST_BAD,  
  CURLDIGEST_BADALGO, 
  CURLDIGEST_NOMEM,
  CURLDIGEST_FINE, 

  CURLDIGEST_LAST  
} CURLdigest;

enum {
  CURLDIGESTALGO_MD5,
  CURLDIGESTALGO_MD5SESS
}; 

CURLdigest Curl_input_digest(struct connectdata *conn,
                             bool proxy, char *header); 

CURLcode Curl_output_digest(struct connectdata *conn,
                            bool proxy,
                            unsigned char *request,
                            unsigned char *uripath);
void Curl_digest_cleanup(struct SessionHandle *data);
void Curl_digest_cleanup_one(struct digestdata *dig);

#endif
