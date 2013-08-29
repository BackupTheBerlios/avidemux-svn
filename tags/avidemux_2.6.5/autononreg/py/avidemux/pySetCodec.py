#PY  <- Needed to identify#
#--automatically built--
#--Project: /home/fx/earl.py

adm=Avidemux()
#** Video **
# 01 videos source 
adm.loadVideo("/work/samples/avi/2mn.avi")
#** Video Codec conf **
adm.videoCodec("x264","params=AQ=20","MaxRefFrames=2","MinIdr=10","MaxIdr=150","threads=99","_8x8=True","_8x8P=True","_8x8B=True","_4x4=True","_8x8I=True","_4x4I=True","MaxBFrame=2","profile=30","CABAC=True","Trellis=True")

adm.setContainer("MKV","forceDisplayWidth=False","displayWidth=1280")

#End of script
