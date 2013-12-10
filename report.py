###
# Author: Joshua Kociemba (kociembj), Joshua.Kociemba@oregonstate.edu
# Created: 2013-12-7
# Filename: report.py
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
import subprocess as sp
from socket import gethostname

LISTENQ = 1024
MAXLINE = 4096
MAXSOCKADDR = 128
BUFFSIZE = 4096

SERV_PORT = 9879
SERV_PORT_STR = "9879"
SERV_HOST = gethostname()
CLIENT_SOCK = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
CLIENT_SOCK.connect((SERV_HOST, SERV_PORT))

def main():
	#sp.call('clear', shell = True)

	try:
		opts, args = getopt.getopt(sys.argv[1:], "khp", ["help",
			"output=", "max:", "min:"])
	except getopt.GetoptError as err:
		#print usage and exit
		print str(err)
		print "Usage: python report.py [-hkp]"
		sys.exit(2)
	for o, a in opts:
		if o in ("-h", "--help"):
			CLIENT_SOCK.send('h')
			serv_message = CLIENT_SOCK.recv(BUFFSIZE)
			print serv_message
		elif o == "-k":
			CLIENT_SOCK.send('k')
			serv_message = CLIENT_SOCK.recv(BUFFSIZE)
			print serv_message
		elif o == "-p":
			min = raw_input("Enter a min: ")
			max = raw_input("Enter a max: ")
			data = ('p' + " " + `min` + " " + `max`)
			CLIENT_SOCK.send(data)
			serv_message = CLIENT_SOCK.recv(BUFFSIZE)
			print serv_message
		else:
			assert False, "unhandled option"

	CLIENT_SOCK.close()

if __name__ == "__main__":
	main()
