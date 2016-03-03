##Introduction
Record is a program to record microphone and system audio at the same time, into two tracks.
##How to build

1. Build libobs.
2. Build record:
```
mkdir build
cd build
set LIBOBS_INCLUDE_DIR=path to obs-studio\libobs
set obspath=path to obs-studio
cmake -G "Visual Studio 12 2013" ..
cmake --build . --config RelWithDebInfo
```

To run, go to obs-studio\build\rundir\config\bin\32bit and run record from there with its path.

##Usage
Run something like this:
```
record test.mkv pcm_s16_le
```
Which will record 2 wavs to test.mkv. Use mkvextract to pull them back out again.
