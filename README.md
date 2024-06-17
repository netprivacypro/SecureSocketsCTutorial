Secure Sockets Tutorial (C)
===

This repository holds the source code for the Net Privacy Pro Secure Sockets tutorial in C. Here, you can find a server and a client application, both using OpenSSL to establish a connection, and securely transmit data back and forth before closing the connection cleanly again. You can find the accompanying tutorial [here](https://netprivacypro.com/how-to-use-secure-sockets-in-c-on-linux/).

Dependencies
---

For this project to work, you need to install the necessary dependencies. If you use Debian or Ubuntu, install ```libssl-dev```:
```bash
sudo apt-get install libssl-dev
```

For other distributions, please refer to your respective package manager documentation.

Generating SSL Certificates
---

The applications require a valid SSL certificate to work. You can easily generate this yourself locally with the following command:
```bash
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
```

This certificate will be valid for 365 days.

Building the Applications
---

To build the project, simply invoke its ```Makefile``` on a command shell:
```bash
make
```

This should build the required ```server``` and ```client``` binaries.

Running the Server and Client
---

The server listens for socket connections on port 4910. Just run the server on one console, and run the client on a separate console on the same host. The client will automatically connect to the server.

The client will send a "Ping" message to the server, which will respond with "Pong". The client will terminate right after receiving the response, while the server will keep running until manually quit. It can answer multiple subsequent requests from clients.

License
---

This project is distributed under a BSD-3 license. Please credit [Net Privacy Pro](https://netprivacypro.com) if you use it or refer to it. Please refer to the LICENSE file in this repository for more information.

Contact
---

If you have questions or would like to get in touch for other reasons, please reach out to [info@netprivacypro.com](info@netprivacypro.com).
