###
# Author: Joshua Kociemba (kociembj), Joshua.Kociemba@oregonstate.edu
# Created: 2013-12-7
# Filename: manage.py
#
# Description:
#
#
###

import os
import sys
import socket
import signal
import getopt
import string
import subprocess as sp
from socket import gethostname

LISTENQ = 1024
MAXLINE = 4096
MAXSOCKADDR = 128
BUFFSIZE = 4096

SERV_PORT = 9879
SERV_PORT_STR = "9879"
SERV_HOST = gethostname()
SERV_SOCK = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
SERV_SOCK.bind((SERV_HOST, SERV_PORT))
SERV_SOCK.listen(5)

def main():
	sp.call('clear', shell = True)
	print "Wait for client..."

	while True:
		CLIENT_COMPUTE, CLIENT_ADDR = SERV_SOCK.accept()
		print "Connection from: ", CLIENT_ADDR
		CLIENT_SOCK, CLIENT_ADDR = SERV_SOCK.accept()
		print "Connection from: ", CLIENT_ADDR
		CLIENT_DATA = CLIENT_SOCK.recv(BUFFSIZE)
		print CLIENT_DATA
		str = CLIENT_DATA.replace("'", "")
		CLIENT_DATA = str.split()
		OPT = CLIENT_DATA[0]

		if (OPT == ""):
			print "Data was not received"
			continue
		elif (OPT == 'h'):
			CLIENT_SOCK.send("Usage: python report.py [-hkp]")
		elif (OPT == 'k'):
			CLIENT_SOCK.send("Killing server and server processes now...")
			exit(1)
		elif (OPT == 'p'):
			MIN = CLIENT_DATA[1]
			MAX = CLIENT_DATA[2]
			print ("Sending min: " + MIN + " max: " + MAX)
			CLIENT_COMPUTE.send(MIN + " " + MAX)
			count = CLIENT_COMPUTE.recv(BUFFSIZE)
			output = "There are " + count + " perfect numbers in the range " + MIN + " - " + MAX
			CLIENT_SOCK.send(output)


if __name__ == "__main__":
	main()
