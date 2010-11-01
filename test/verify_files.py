# verify_files.py

import os

files = os.listdir(os.getcwd())

formats = ("bmp", "pgm")
sbuf = []

for x in range(1, len(files)+1):
	sbuf = files[x-1].split('.')
	
	if sbuf[0] != ("%.3d" % x):
		print(sbuf[0])
		print("--! file is not %.3u compliant.")
	
	if sbuf[-1] not in formats:
		print(sbuf[-1])
		print("--! file is not in a recognized image format")

os.system("pause")
