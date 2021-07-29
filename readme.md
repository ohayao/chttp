## c use libcurl to send https
`c语言使用libcurl库发送https请求`

1. 下载源码包 [连接](https://curl.haxx.se/download/)
2. `./configure --prefix=/path/to/curl --disable-shared --enable-static --with-ssl=/path/to/openssl --without-libidn --without-librtmp  --without-nss --without-libssh2 --without-zlib --without-winidn --without-gnutls --disable-rtsp  --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smtp --disable-gopher --disable-ldap`
3. `make && make install`
4. 编译 `gcc main.c -o main.out -lcurl`