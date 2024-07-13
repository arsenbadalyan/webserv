#!/usr/bin/env python3

import cgi
import datetime

def main():
    # Output HTML content directly
    print("<!DOCTYPE html>")
    print("<html lang='en'>")
    print("<head>")
    print("<meta charset='UTF-8'>")
    print("<meta name='viewport' content='width=device-width, initial-scale=1.0'>")
    print("<title>Python CGI Output</title>")
    print("</head>")
    print("<body>")
    print("<h1>Hello from Python CGI!</h1>")
    print("<p>The current date and time is: {}</p>".format(datetime.datetime.now()))
    print("<p><a href='/index.html'>Back to main page</a></p>")
    print("</body>")
    print("</html>")

if __name__ == "__main__":
    main()
