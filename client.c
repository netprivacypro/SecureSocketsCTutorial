// Copyright (2024) Net Privacy Pro; see README.md and LICENSE for more details

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 4910

/**
 * Initialize the OpenSSL library
 */
void init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

/**
 * Cleanup the OpenSSL library
 */
void cleanup_openssl() {
    EVP_cleanup();
}

/**
 * Create and return an SSL context
 */
SSL_CTX* create_context() {
    const SSL_METHOD* method;
    SSL_CTX* ctx;

    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

/**
 * Main entry point for this application
 */
int main() {
    int32_t sock;
    struct sockaddr_in addr;
    SSL_CTX* ctx;
    SSL* ssl;
    const char* hostname = "127.0.0.1";
    const char* message = "Ping";
    char buf[256] = {0};

    init_openssl();
    ctx = create_context();

    ssl = SSL_new(ctx);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, hostname, &addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to connect");
        exit(EXIT_FAILURE);
    }

    SSL_set_fd(ssl, sock);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        SSL_write(ssl, message, strlen(message));
        SSL_read(ssl, buf, sizeof(buf));
        printf("Received: %s\n", buf);
    }

    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}