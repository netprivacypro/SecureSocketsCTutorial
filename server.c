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
SSL_CTX *create_context() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

/**
 * Configure the SSL contect to use the key and cert
 * 
 * @param ctx The SSL context to configure
 */
void configure_context(SSL_CTX *ctx) {
    SSL_CTX_set_ecdh_auto(ctx, 1);

    // Set the key and cert
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

/**
 * Main entry point for this application
 */
int main() {
    int32_t sock;
    struct sockaddr_in addr;
    SSL_CTX* ctx;

    init_openssl();
    ctx = create_context();

    configure_context(ctx);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 1) < 0) {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct sockaddr_in addr;
        uint32_t len = sizeof(addr);
        SSL* ssl;
        const char reply[] = "Pong";

        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            char buf[256] = {0};
            SSL_read(ssl, buf, sizeof(buf));
            printf("Received: %s\n", buf);

            SSL_write(ssl, reply, strlen(reply));
        }

        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}
