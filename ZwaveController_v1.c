*************************************************************************************************
Description:
main code with passing structure of response & buffer values from function to main 

*************************************************************************************************

///////////////main server code///////////////


#include<stdio.h>
#include "mongoose.h"
#include "addnode.h"
#include "removenode.h"
#include "removefailed.h"
#include "neighbourUp.h"
#include "isFailed.h"

int fd,ret_bytes,n,flag=0,i,status;
char *res_buff;

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;


/*static void handle_nodeAdd_call(struct mg_connection *nc, struct http_message *hm) 
{
	//struct Add_response *m;
	res_buff = add_node_to_network();
	

	printf("%02x",*((char *) &res_buff+5));
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
	if(*((char *) &res_buff+5) == 05)
		mg_printf_http_chunk(nc, "{\n \"Message\": Node Added successfully \n \"Node ID\": %d  \n }", *((char *) &res_buff+6) );
		
	else
		mg_printf_http_chunk(nc, "%s\n", "{ \n \"Message\": Unable to Add }");
	
 	mg_send_http_chunk(nc, "", 0);  
}*/

static void handle_nodeAdd_call(struct mg_connection *nc, struct http_message *hm)  //pointer
{
	struct Add *m;
	m = add_node_to_network();
	

	printf("%d\n",m->code);
	printf("%d\n",m->swap[5]);
	printf("%s\n",m->desc);
	
	printf("%d\n",p->code);
	printf("%d\n",p->swap[5]);
	printf("%s\n",p->desc);
		
		/*printf("%ld\t",(long int)(*m).buff+1);
		printf("%ld\t",(long int)(*m).buff+2);
		printf("%ld\t",(long int)(*m).buff+3);
		printf("%ld\t",(long int)(*m).buff+4);
		printf("%ld\t",(long int)(*m).buff+5);*/
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
	if(m->code == 06)
		mg_printf_http_chunk(nc, "{ \"code\":%d \n \"Message\":%s \n}", m->code,m->desc );
	if(m->swap[5] == 5)
		mg_printf_http_chunk(nc, "{\n \"Message\": Node Added successfully \n \"Node ID\": %d  \n }", m->swap[6]);
		
	else
		mg_printf_http_chunk(nc, "%s\n", "{ \n \"Message\": Unable to Add }");
	
 	mg_send_http_chunk(nc, "", 0);  
}
static void handle_nodeRemove_call(struct mg_connection *nc, struct http_message *hm) 
{
	res_buff = remove_node_from_network();
	
	
	
	
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
	if(*((char *) &res_buff+5) == 06)
		mg_printf_http_chunk(nc, "{\n \"Message\": Node Removed successfully \n \"Node ID\": %d \n  }", *((char *) &res_buff+6));
		
	else
		mg_printf_http_chunk(nc, "%s\n", "{\n \"Message\": Unable to Remove }");

  	mg_send_http_chunk(nc, "", 0);  
	
}

static void handle_noderemovefailed_call(struct mg_connection *nc, struct http_message *hm) 
{
	res_buff = remove_failed_node();
	

	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
	if(*((char *) &res_buff+5) == 01)
	
		mg_printf_http_chunk(nc, "%s\n", "{ \n \"Message\": Failed Node Removed successfully \n \"Node ID:\": %d }", 0);
	
	else 
		mg_printf_http_chunk(nc, "%s\n", "{ \n \"Message\": Unable to Remove failed Node \n \"Node ID:\": %d}", 0);

  	mg_send_http_chunk(nc, "", 0);  
}



static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) 
{
  	struct http_message *hm = (struct http_message *) ev_data;

	switch (ev) 
	{
    		case MG_EV_HTTP_REQUEST:
      			if (mg_vcmp(&hm->uri, "/api/v1/nodeAdd") == 0) //Add Node
			{
			        handle_nodeAdd_call(nc, hm);                    // Handle RESTful call 
			}
			
			else if(mg_vcmp(&hm->uri, "/api/v1/nodeRemove") == 0) //Remove Node
			{
				handle_nodeRemove_call(nc, hm);
			}
			
			else if(mg_vcmp(&hm->uri, "/api/v1/nodeRemoveFailed") == 0) 
			{
				handle_noderemovefailed_call(nc, hm);
			}
			else if (mg_vcmp(&hm->uri, "/printcontent") == 0) 
			{
			        char buf[100] = {0};
			        memcpy(buf, hm->body.p,sizeof(buf) - 1 < hm->body.len? sizeof(buf) - 1 : hm->body.len);
			        printf("%s\n", buf);
   			} 
			else 
			{
			        mg_serve_http(nc, hm, s_http_server_opts);  /* Serve static content */
      			}
      			break;
			default:
      			break;
  	}
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i;
  char *cp;
#ifdef MG_ENABLE_SSL
  const char *ssl_cert = NULL;
#endif

  mg_mgr_init(&mgr, NULL);

  /* Process command line options to customize HTTP server */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-D") == 0 && i + 1 < argc) {
      mgr.hexdump_file = argv[++i];
    } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
      s_http_server_opts.document_root = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      s_http_port = argv[++i];
    } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
      s_http_server_opts.auth_domain = argv[++i];
    } else if (strcmp(argv[i], "-P") == 0 && i + 1 < argc) {
      s_http_server_opts.global_auth_file = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      s_http_server_opts.per_directory_auth_file = argv[++i];
    } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
      s_http_server_opts.url_rewrites = argv[++i];
#ifndef MG_DISABLE_CGI
    } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      s_http_server_opts.cgi_interpreter = argv[++i];
#endif
#ifdef MG_ENABLE_SSL
    } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      ssl_cert = argv[++i];
#endif
    }
  }

  /* Set HTTP server options */
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    fprintf(stderr, "Error starting server on port %s\n", s_http_port);
    exit(1);
  }

#ifdef MG_ENABLE_SSL
  if (ssl_cert != NULL) {
    const char *err_str = mg_set_ssl(nc, ssl_cert, NULL);
    if (err_str != NULL) {
      fprintf(stderr, "Error loading SSL cert: %s\n", err_str);
      exit(1);
    }
  }
#endif

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = "."; //path of html script
  s_http_server_opts.enable_directory_listing = "yes"; //server config file 

  /* Use current binary directory as document root */
  if (argc > 0 && ((cp = strrchr(argv[0], '/')) != NULL ||
      (cp = strrchr(argv[0], '/')) != NULL)) {
    *cp = '\0';
    s_http_server_opts.document_root = argv[0];
  }

  printf("Starting RESTful server on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}