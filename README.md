All possible directives of webserv

[server] - global directive
[listen] - inside only [server] // default settings -> 127.0.0.1, 8080 port
[server_name] - inside only [server]
[root] - inside only [server] // default settings -> /var/
[upload_dir] - inside only [server] // default upload
[autoindex] - inside [server], [location], accepts 2 values "on" or "off"
[cgi_set] - inside only [server], valid input is "cgi_path .cgi_extension"
[location] - inside only [server]
[client_max_body_size] - inside [server], [location]
[index] - inside [server], [location]
[return] - inside only [location]
[allow_methods] - inside only [location]
[error_page] - inside [server], [location], valid input -> errors list, error file