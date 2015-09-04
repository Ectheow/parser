#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define MAXTOKENLEN 512
#define TOKENSTACKSIZE 1024

enum type_tag {
    FUNCTION,
    ADDRESS,
    ADDRESS6,
    NUMBER,
    VALUE,
    STRING,
    INVALID,
};

struct token {
    char type;
    char string[MAXTOKENLEN];
    size_t valuelen;
    void *value;
};

struct token token_stack[TOKENSTACKSIZE];
/*
void
parse_error(const char *info, ...) {
    fprintf(stderr, "Parse ERROR: %s!!!\n", info);
}

int
validate_num(char *str)
{
    while(isdigit(*str++));
    if(*str != '\0') return 0;
    return 1;
}

int
validate_quoted_string(char *str)
{
    while(*str++ != '"' || *str++ != '\0');
    if (*str == '\0') return 0;
    return 1;
}

int
parse_ip6_str(struct token *tok)
{
    size_t count;
    char *str = tok->string;
get_nums:
    while (isdigit(*str) || (tolower(*str) > 'a' && tolower(*str) < 'f')) {++count; ++str;}
    if (*str == ':') goto get_nums;
    else if(*str != '\0')  parse_error("bad ipv6 address");
    
    tok->type = ADDRESS6;
    tok->valuelen = sizeof(struct in6_addr);
    tok->value = malloc(sizeof(struct in6_addr));
    
    if (inet_pton(AF_INET6, tok->string, tok->value) <= 0)
	parse_error("bad ipv6 addresss");
    return 0;
}


int
parse_ip4_str(struct token *tok)
{
  
}


int
parse_token(struct token *tok)
{
    char *s = tok->string;
    

     * We do some of the easy parsing up front.

    if (isdigit(*s)) {
	return parse_num(tok);
    } else if (*s == '"' && validate_quoted_string(s)) {
	tok->type = STRING;
	tok->valuelen = 0;
	tok->value = NULL;
    } else {
	if (isdigit(*s)) return parse_num(tok);
	if (*s == '"') return parse_string(tok);
	if (matches_ip6_addr(s)) return parse_ip6(tok);
	if (matches_value_arg(s)) return parse_value_arg(tok);
	parse_error("parse_token can't do this\n");
    }
    return 0;
}
*/

int
matches_number(char **str_ptr, struct token *tok)
{

    char *str = *str_ptr;
    while(isdigit(*str++));

    if (*str == ' ' || str == '\0') {
	tok->type = NUMBER;
	tok->valuelen = sizeof(int);
	tok->value = malloc(sizeof(int));
	*((int *) tok->value) = strtol(beginptr, &endptr, 10);
	if (endptr != '\0') {
	    free(token->value);
	    token->valuelen = 0;
	    return 0;
	}
	*str_ptr = str;
	return 1;
    }

    return 0;
}

int
matches_ip4_addr(char **str_ptr, struct token *tok)
{
    char *str = *str_ptr;
    size_t count;
    size_t octet_ct = 0;
    unsigned char octets[4];
    char buf[3];
    char *str = tok->string;
    
    bzero(octets, 4);
get_nums:
    bzero(buf, 3);
    count =0;
    while (isdigit(*str)) {
	buf[count] = *str;
	++count;
	++str;
    }
    if (*str == '.' && count <= 3 && octet_ct <= 3) {
	octets[octet_ct] = (unsigned char)atoi(buf);
	if (octets[octet_ct] > 255)
	    return 0;
	++octet_ct;
	++str;
	goto get_nums;
    } else if(*str == '\0' && octet_ct == 3) {
	tok->type = ADDRESS;
	tok->valuelen = sizeof(struct in_addr);
	tok->value = malloc(sizeof(struct in_addr));
	memcpy(tok->value, octets, sizeof(struct in_addr));
	*str_ptr = str;
	return 1;
    } 
    return 0;
    
}

struct token *
parse_string(char *str)
{

    while(*str != '\0') {
	while(isspace(*str++));
	--str;

	if (matches_number(&str, tok)) {
	    queue_token(tok);
	} else if (matches_ip4_addr(&str, tok)) {
	    queue_token(tok);
	} /*else if (matches_quoted_string(&str, tok)) {
	    queue_token(tok);
	} else if (matches_ip6_addr(&str, tok)) {
	    queue_token(tok);
	} else if (matches_value_arg(&str, tok)) {
	    queue_token(tok);
	} else if (make_function(&str, tok)) {
	    queue_token(tok);
	    }*/
    }
}

void
get_word(char **start, char **end)
{
    char *ptr = *start, *endptr = *end;
    
    while(isspace(*ptr++));
    ptr--;
    endptr = ptr;
    while(!isspace(*endptr++));
    --endptr;
    
    *end = endptr;
    *start = ptr;
}


#define BUFLEN 256
int
main(int argc, char **argv)
{
    char buf[MAXTOKENLEN];
    struct token tok;
    bzero(buf, MAXTOKENLEN);
    fprintf(stdout, "token > ");
    while (fgets(buf, MAXTOKENLEN, stdin) != NULL) {
	buf[strlen(buf)-1] = 0;
	memcpy(tok.string, buf, MAXTOKENLEN);
	parse_token(&tok);
	switch(tok.type) {
	case FUNCTION:
	    fprintf(stdout, "FUNCTION");
	    break;
	case ADDRESS:
	    fprintf(stdout, "ADDRESS");
	    break;
	case NUMBER:
	    fprintf(stdout, "NUMBER");
	    break;
	case VALUE:
	    fprintf(stdout, "VALUE");
	    break;
	case STRING:
	    fprintf(stdout, "STRING");
	    break;
	case ADDRESS6:
	    fprintf(stdout, "ADDRESS6");
	    break;
	default:
	    fprintf(stdout, "INVALID");
	    break;
	}
	fprintf(stdout, "\ntoken > ");
	bzero(buf, MAXTOKENLEN);
    }
    return 0;
}
