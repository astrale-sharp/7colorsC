from http.server import HTTPServer, SimpleHTTPRequestHandler, test
import os, sys, logging
import argparse

class CORSRequestHandler (SimpleHTTPRequestHandler):
    def end_headers (self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        SimpleHTTPRequestHandler.end_headers(self)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', default=8060, type=int)
    parser.add_argument('-r', '--root', default='', type=str)
    args = parser.parse_args()
    if args.root:
        os.chdir(args.root)
    test(CORSRequestHandler, HTTPServer, port=args.port)