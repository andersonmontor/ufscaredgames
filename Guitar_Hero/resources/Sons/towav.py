import os

for file in os.listdir("/home/anderson/ufscared/ufscaredgames/Guitar_Hero/resources/Sons"):
	
	os.system('ffmpeg -i "%s" "%s.wav"' % (file, file))

